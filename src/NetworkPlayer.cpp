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
    std::string sToken;
    std::string sErrorMessage;

    // Get last move made
    GameMove cGameMove = cGame.LastMove();

    //// Create command string
    //std::string sMessage = cGameMove.Command() + GameVocabulary::DELIMETER + cGameMove.Argument();

    // Create JSON of GameMove
    Json::Value jGameMove = cGameMove.JsonSerialization();
    Json::FastWriter jFWriter;

    m_cLogger.LogInfo("Sending move to opponent", 2);

    // Send command string to networked player
    //if (!Socket::Send(sMessage))
    // Send JSON GameMOve to networked player
    if (!Socket::Send(jFWriter.write(jGameMove)))
    {
        sErrorMessage = "Could not send command: " + sCommand;
        Socket::Send(GameVocabulary::FATAL_EXIT);
        throw SocketException(sErrorMessage);
    }

    // Receive command string (networked player's confirmation)
    // Limit received message to CONFIRM
    if (!Socket::Recv(sCommand, GameVocabulary::CONFIRM.length()) < 0)
        throw SocketException("Did not receive move confirmation");

    // Evaluate CONFIRM command
    sToken = GameVocabulary::ParseCommand(sCommand);
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
    std::string sArgument;
    std::string sErrorMessage;
    std::string sMessage;
    std::string sJsonGameMove;

    //m_cLogger.LogInfo("Waiting for opponent's message", 2);
    m_cLogger.LogInfo("Waiting for opponent's move", 2);

    // Receive the last move made from the networked player
    //if (!Socket::Recv(sMessage) < 0)
    if (!Socket::Recv(sJsonGameMove) < 0)
        //throw SocketException("Did not receive move command");
        throw SocketException("Did not receive move");

    //std::cout << "Received move " << sMessage << " from opponent" << std::endl;
    sMessage = "Received move\n" + sJsonGameMove + "\nfrom opponent";
    m_cLogger.LogInfo("Waiting for opponent's move", 3);

    //// Evaluate command
    //sCommand = GameVocabulary::ParseCommand(sMessage);

    if (!cGameMove.JsonDeserialization(sJsonGameMove, sErrorMessage))
    {
        // Check for FATAL_EXIT
        //if (sCommand.compare(GameVocabulary::FATAL_EXIT) == 0)
        if (sJsonGameMove.compare(GameVocabulary::FATAL_EXIT) == 0)
        {
            sErrorMessage = "Opponent experienced a fatal error.";
            std::cerr << sErrorMessage << std::endl;
            std::cout << "Exiting." << std::endl;
            exit(EXIT_FAILURE);
        }
        else
        {
            std::cerr << sErrorMessage << std::endl;
            std::cout << "Exiting." << std::endl;
            exit(EXIT_FAILURE);
        }

    }

/*
    // Evaluate for DELCARE_WIN
    else if (sCommand.compare(GameVocabulary::DECLARE_WIN) == 0)
    {
        return true;
    }
*/

/*
    if (sToken.compare(GameVocabulary::MOVE) != 0)
    {

        if (sToken.compare(GameVocabulary::FATAL_EXIT) == 0)
        {
            sErrorMessage = "Opponent experienced a fatal error.";
            std::cerr << sErrorMessage << std::endl;
            std::cout << "Exiting." << std::endl;
            exit(EXIT_FAILURE);
        }


        else if (sToken.compare(GameVocabulary::DECLARE_WIN) == 0)
        {
            return true;
        }


        else
        {
            sErrorMessage  = "Expected command " + GameVocabulary::MOVE + ", but received " + sCommand;
            std::cerr << sErrorMessage << std::endl;
            std::cout << "Exiting" << std::endl;
            Socket::Send(GameVocabulary::FATAL_EXIT);
            throw GameAIException(sErrorMessage);
        }
    }
*/
    //// Evaluate move
    //sToken = GameVocabulary::ParseArgument(sCommand);
    //std::cout << "Received move " << sToken << " from opponent" << std::endl;

    //// Generate a game move from the received move
    //cGameMove = cGame.GenerateMove(sToken);

    // Test game move for validity.
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
        //sErrorMessage = "Move " + sToken + " is invalid";
        sErrorMessage = "Move " + sMessage + " is invalid";
        std::cerr << sErrorMessage << std::endl;
        std::cout << "Exiting" << std::endl;
        Socket::Send(GameVocabulary::UNCONFIRM);
        Socket::Send(GameVocabulary::FATAL_EXIT);
        throw GameAIException(sErrorMessage);
    }

    return true;
}
