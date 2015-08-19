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
  *
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
    std::string sCommand;
    std::string sErrorMessage;

    if (cGame.SyncCards())
    {
        //

        cGame.SetSyncCards(false);
    }

    m_cLogger.LogInfo("Sending move to opponent", 2);

    // Get last move made
    GameMove cGameMove = cGame.LastMove();

    // Create JSON of GameMove
    Json::Value jGameMove = cGameMove.JsonSerialization();
    Json::FastWriter jFWriter;

    // Send JSON GameMove to networked player
    if (!Socket::Send(jFWriter.write(jGameMove)))
    {
        sErrorMessage = "Could not send command: " + sCommand;
        Socket::Send(GameVocabulary::FATAL_EXIT);
        throw SocketException(sErrorMessage);
    }

    // Receive command string (networked player's confirmation)
    // Limit received message to UNCONFIRM
    if (!Socket::Recv(sCommand, GameVocabulary::UNCONFIRM.length()) < 0)
        throw SocketException("Did not receive move confirmation");

    // Evaluate CONFIRM command
    std::string sToken = GameVocabulary::ParseCommand(sCommand);
    if (sToken.compare(GameVocabulary::CONFIRM) != 0)
    {
        sErrorMessage  = "Expected command " + GameVocabulary::CONFIRM + ", but received " + sCommand;
        std::cerr << sErrorMessage << std::endl;
        std::cout << "Exiting" << std::endl;
        Socket::Send(GameVocabulary::FATAL_EXIT);
        throw GameAIException(sErrorMessage);
    }

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
    std::string sCommand;
    std::string sErrorMessage;
    std::string sJsonGameMove;

    if (m_cLogger.Level() >= 1)
    {
        std::cout << "Waiting for opponent's move" << std::endl;
    }

    // Receive the last move made from the networked player
    if (!Socket::Recv(sJsonGameMove) < 0)
        throw SocketException("Did not receive move");

    std::string sMessage = "Received move\n" + sJsonGameMove + "\nfrom opponent";
    m_cLogger.LogInfo(sMessage, 3);

    // Parse Json Game Move into GameMove
    if (!cGameMove.JsonDeserialization(sJsonGameMove, sErrorMessage))
    {
        // If parse was not successful, check for DECLARE_WIN
        if (sJsonGameMove.compare(GameVocabulary::DECLARE_WIN) == 0)
        {
            return true;
        }
        // else check for FATAL_EXIT
        else if (sJsonGameMove.compare(GameVocabulary::FATAL_EXIT) == 0)
        {
            sErrorMessage = "Opponent experienced a fatal error.";
            std::cerr << sErrorMessage << std::endl;
            std::cout << "Exiting." << std::endl;
            exit(EXIT_FAILURE);
        }
        // else report Json parse error
        else
        {
            std::cerr << sErrorMessage << std::endl;
            std::cout << "Exiting." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // Announce move
    std::cout << cGame.AnnounceMove(m_nPlayerNumber, cGameMove) << std::endl;

    // Test GameMove for validity.
    if (cGame.ApplyMove(m_nPlayerNumber, cGameMove))
    {
        // Send confirmation of received move
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
