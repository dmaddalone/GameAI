/*
    Copyright 2015 Dom Maddalone

    This file is part of GameAI.

    GameAI is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GameAI is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GameAI.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "NetworkPlayer.h"

/**
  * Construct a NetworkPlayer class.
  */

NetworkPlayer::NetworkPlayer(PlayerType ecPlayerType) : Player(ecPlayerType)
{
    // Supporting the main game play loop.  If player is number 1, begin by receiving
    // a message.  If not, begin by sending a message.
    if (m_nPlayerNumber == 1)
        SetToReceiving();
    else
        SetToSending();
}

/**
  * Send and receive moves between networked players.
  *
  * Send the last move made to the networked player and receive her move back.
  *
  * \param cGame The game.
  *
  * \return True, if a move has been made or no move to make.
  */

bool NetworkPlayer::Move(Game &cGame)
{
    if (m_nPlayerNumber == 2)  // Network Player #2
    {
        // Show game board at proper logging level.
        if (m_cLogger.Level() >= 1)
        {
            cGame.Display();
        }

        if (!SendLastMove(cGame)) // Send Player #1 last move
            return false;

        // Wait for opponent's (Player #2) move
        return RecvLastMove(cGame); // Receive opponent's (Player #2) last move
    }
    else // Network Player #1
    {
        if (Sending())
        {
            // Show game board at proper logging level.
            if (m_cLogger.Level() >= 1)
            {
                cGame.Display();
            }

            if (!SendLastMove(cGame)) // Send Player #2 last move
                return false;

            // Wait for opponent's (Player #2) move
            return RecvLastMove(cGame); // Receive opponent's (Player #2) last move
        }
        else // Receiving
        {
            SetToSending(); // For next turn

            return RecvLastMove(cGame); // Receive opponent's (Player #2) last move
        }
    }
}

/**
  * Send last move made to the networked player.
  *
  * Send the last move made to the networked player.
  *
  * \param cGame The game.
  *
  * \return True, if a move has been sent successfully.
  */

bool NetworkPlayer::SendLastMove(Game &cGame)
{
    std::string sMessage {};

    //
    // Sync game environment, if required
    //
    if (cGame.Sync())
    {
        std::cout << "Sending synchronization information" << std::endl;

        Send(GameVocabulary::SYNC);
        RecvConfirmation();

        // Receive commands or JSON to send to opponent
        while (cGame.GetSyncInfo(sMessage))
        {
            Send(sMessage);
            RecvConfirmation();
        }

        Send(GameVocabulary::END_SYNC);
        RecvConfirmation();

        // Set sync to false
        cGame.SetSync(false);
    }

    m_cLogger.LogInfo("Sending move to opponent", 2);

    // Get last move made and create JSON of game move
    sMessage = cGame.LastMove().JsonSerialization().toStyledString();

    std::string sLogMessage = "Sending move->\n" + sMessage + "<-to opponent";
    m_cLogger.LogInfo(sLogMessage, 3);

    //if (!Socket::Send(cGameMove.JsonSerialization().toStyledString()))
    if (!Socket::Send(sMessage))
    {
        std::string sErrorMessage = "Could not send command: " + sMessage;
        Socket::Send(GameVocabulary::FATAL_EXIT);
        throw SocketException(sErrorMessage);
    }

    // Receive confirmation from networked opponent
    RecvConfirmation();

    return true;
}

void NetworkPlayer::Send(const std::string &sMessage)
{
    if (!Socket::Send(sMessage))
    {
        std::string sErrorMessage = "Could not send command: " + sMessage;
        Socket::Send(GameVocabulary::FATAL_EXIT);
        throw SocketException(sErrorMessage);
    }
}

/**
  * Receive last move made from the networked player.
  *
  * Receive the last move made by the networked player.
  *
  * \param cGame The game.
  *
  * \return True, if a move has been received successfully.
  */

bool NetworkPlayer::RecvLastMove(Game &cGame)
{
    GameMove cGameMove;
    std::string sCommand;
    std::string sErrorMessage;
    std::string sMessage;

    if (m_cLogger.Level() >= 1)
    {
        std::cout << "Waiting for opponent's move or message" << std::endl;
    }

    // Receive the last move made from the networked player
    if (!Socket::Recv(sMessage) < 0)
        throw SocketException("Did not receive move or message");

    // Evaluate for SYNC
    if (sMessage.compare(GameVocabulary::SYNC) == 0)
    {
        m_cLogger.LogInfo("Received Sync from network opponent.", 2);
        RecvSyncInfo(cGame);
        return RecvLastMove(cGame);
    }
    // Evaluate for DECLARE_WIN
    else if (sMessage.compare(GameVocabulary::DECLARE_WIN)== 0)
    {
        m_cLogger.LogInfo("Received Declared Win from network opponent.", 2);
        return true;
    }

    // Evaluate for FATAL_EXIT
    else if (sMessage.compare(GameVocabulary::FATAL_EXIT) == 0)
    {
        sErrorMessage = "Received Fatal Exit from network opponent.";
        std::cerr << sErrorMessage << std::endl;
        std::cout << "Exiting." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Assume JSON game move and parse into GameMove
    else if (!cGameMove.JsonDeserialization(sMessage, sErrorMessage))
    {
         // else report Json parse error
        std::cerr << sErrorMessage << std::endl;
        std::cout << "Exiting." << std::endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        std::string sLogMessage = "Received move->\n" + sMessage + "<-from opponent";
        m_cLogger.LogInfo(sLogMessage, 3);
    }

    // Announce move
    std::cout << cGame.AnnounceMove(m_nPlayerNumber, cGameMove) << std::endl;

    // Test GameMove for validity.
    if (cGame.ApplyMove(m_nPlayerNumber, cGameMove))
    {
        // Send confirmation of received JSON game move
        sCommand = GameVocabulary::CONFIRM;
        if (!Socket::Send(sCommand))
        {
            sErrorMessage = "Could not send command: " + sCommand;
            throw SocketException(sErrorMessage);
        }

        // If the last move ended the game, send a DECLARE_WIN message
        if (cGame.GameEnded(2 - m_nPlayerNumber + 1))
        {
            sCommand = GameVocabulary::DECLARE_WIN;
            if (!Socket::Send(sCommand))
            {
                sErrorMessage = "Could not send command: " + sCommand;
                throw SocketException(sErrorMessage);
            }
        }

    }
    else // Game move is not valid
    {
        sErrorMessage = "Move " + sMessage + " is invalid";
        std::cerr << sErrorMessage << std::endl;
        std::cout << "Exiting" << std::endl;
        Socket::Send(GameVocabulary::UNCONFIRM);
        Socket::Send(GameVocabulary::FATAL_EXIT);
        throw GameAIException(sErrorMessage);
    }

    return true;
}

void NetworkPlayer::RecvConfirmation()
{
    std::string sCommand;

    // Receive networked player's confirmation
    // Limit received message to UNCONFIRM
    if (!Socket::Recv(sCommand) < 0)
        throw SocketException("Did not receive move confirmation");

    // Evaluate CONFIRM command
    std::string sToken = GameVocabulary::ParseCommand(sCommand);
    if (sToken.compare(GameVocabulary::CONFIRM) != 0)
    {
        std::string sErrorMessage  = "Expected command " + GameVocabulary::CONFIRM + ", but received " + sCommand;
        std::cerr << sErrorMessage << std::endl;
        std::cout << "Exiting" << std::endl;
        Socket::Send(GameVocabulary::FATAL_EXIT);
        throw GameAIException(sErrorMessage);
    }
}

void NetworkPlayer::RecvSyncInfo(Game &cGame)
{
    std::string sMessage {};
    std::string sErrorMessage {};

    std::cout << "Receiving synchronization information" << std::endl;

    // Alert game to synchronization
    cGame.SetSync(true);

    // Confirm initial SYNC message
    Send(GameVocabulary::CONFIRM);

    // Loop through all messages until END_SYNC received
    while (true)
    {
        // Receive the synchronization information
        if (!Socket::Recv(sMessage) < 0)
            throw SocketException("Did not receive sync information");

        if (sMessage.compare(GameVocabulary::END_SYNC) == 0)
        {
            m_cLogger.LogInfo("Received End Sync from network opponent.", 2);

            // Alert game to synchronization end
            cGame.SetSync(false);

            Send(GameVocabulary::CONFIRM);
            return;
        }

        if (cGame.ApplySyncInfo(sMessage, sErrorMessage))
        {
            Send(GameVocabulary::CONFIRM);
        }
        else
        {
            std::cerr << "Error in ApplySyncInfo(): " << sErrorMessage << std::endl;
            std::cout << "Exiting" << std::endl;
            Socket::Send(GameVocabulary::FATAL_EXIT);
            throw GameAIException(sErrorMessage);
        }
    }
}
