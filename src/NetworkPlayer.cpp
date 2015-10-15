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
    GameMove cGameMove;
    if (m_nPlayerNumber == 2)  // Network Proxy for Player #2
    {
        // If game needs to be synchronized, do it.
        if (cGame.Sync())
        {
            Synchronize(cGame);
            return true;
        }
    }

    // If expecting to send, send last game move
    if (Sending())
    {
        // Show game board at proper logging level.
        if (m_cLogger.Level() >= 1)
        {
            cGame.Display();
        }

        // Send last game move to opponent
        if (!SendLastMove(cGame))
            return false;

        // If the last game move's Another Turn is set to true and the player number is
        // NOT EQUAL to this proxy's player number, do not wait to receive next game move from opponent
        cGameMove = cGame.LastMove();
        if (cGameMove.AnotherTurn() && (cGameMove.PlayerNumber() != m_nPlayerNumber))
            return true;
    }
    // else expecting to receive without sending
    else
    {
        // Set expectation to send on next move
        SetToSending();
    }

    // If game ended, nothing to receive
    if (cGame.GameOver())
        return true;

    // Wait for opponent's move and apply it to the game
    if (RecvLastMove(cGame))
    {
        // If the last game move's Another Turn is set to true and the player number is
        // EQUAL to this proxy's player number, do not wait to receive next game move from opponent
        cGameMove = cGame.LastMove();
        if (cGameMove.AnotherTurn() && (cGameMove.PlayerNumber() == m_nPlayerNumber))
        {
            SetToReceiving(); // For next turn, expect to receive without sending first
        }

        return true;
    }
    else
    {
        return false;
    }
}

void NetworkPlayer::Synchronize(Game &cGame)
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

    m_cLogger.LogInfo("Sending move to opponent", 2);

    // Get last move made and create JSON of game move
    sMessage = cGame.LastMove().JsonSerialization().toStyledString();

    std::string sLogMessage = "Sending move->\n" + sMessage + "<-to opponent";
    m_cLogger.LogInfo(sLogMessage, 3);

    Send(sMessage);

    // Receive confirmation from networked opponent
    RecvConfirmation();

    return true;
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
    std::string sErrorMessage;
    std::string sMessage;

    if (m_cLogger.Level() >= 1)
    {
        std::cout << "Waiting for opponent's move or message" << std::endl;
    }

    // Receive the last move made from the networked player
    if (Socket::Recv(sMessage) < 0)
        throw SocketException("Did not receive move or message");

    // Evaluate for SYNC
    if (sMessage.compare(GameVocabulary::SYNC) == 0)
    {
        m_cLogger.LogInfo("Received Sync from network opponent.", 2);
        RecvSyncInfo(cGame);
        return RecvLastMove(cGame);
    }
    // Evaluate for DECLARE_WIN
    else if (sMessage.compare(GameVocabulary::DECLARE_WIN) == 0)
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

    // Announce game move
    m_cLogger.LogInfo(cGame.AnnounceMove(m_nPlayerNumber, cGameMove),1);

    // Test GameMove for validity.
    if (cGame.ApplyMove(m_nPlayerNumber, cGameMove))
    {
        // Send confirmation of received JSON game move
        Send(GameVocabulary::CONFIRM);

        // If the last move ended the game, send a DECLARE_WIN message
        if (cGame.GameEnded(3 - m_nPlayerNumber))
        {
            Send(GameVocabulary::CONFIRM);
        }

    }
    else // Game move is not valid
    {
        sErrorMessage = "Move " + sMessage + " is invalid";
        std::cerr << sErrorMessage << std::endl;
        std::cout << "Exiting" << std::endl;
        Send(GameVocabulary::UNCONFIRM);
        Send(GameVocabulary::FATAL_EXIT);
        exit(EXIT_FAILURE);
    }

    return true;
}

void NetworkPlayer::RecvConfirmation()
{
    std::string sCommand;

    // Receive networked player's confirmation
    if (Socket::Recv(sCommand) < 0)
        throw SocketException("Did not receive move confirmation");

    // Evaluate CONFIRM command
    std::string sToken = GameVocabulary::ParseCommand(sCommand);
    if (sToken.compare(GameVocabulary::CONFIRM) != 0)
    {
        std::string sErrorMessage  = "Expected command " + GameVocabulary::CONFIRM + ", but received " + sCommand;
        std::cerr << sErrorMessage << std::endl;
        std::cout << "Exiting" << std::endl;
        Send(GameVocabulary::FATAL_EXIT);
        exit(EXIT_FAILURE);
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
        if (Socket::Recv(sMessage) < 0)
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
            Send(GameVocabulary::FATAL_EXIT);
            exit(EXIT_FAILURE);
        }
    }
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
