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

NetworkPlayer::NetworkPlayer(PlayerType ecPlayerType) : Player(ecPlayerType) // TODO: Cleanup logic
{
    if (m_nPlayerNumber == 1)
        SetToReceiving();
    else
        SetToSending();
}

/**
  * Generate the next game move.
  *
  * Use GameMove to collect the next game move from a server player.
  *
  * \param cGame The game.
  *
  * \return True, if a move has been made or no move to make.
  */

bool NetworkPlayer::Move(Game &cGame)
{
// TODO: Cleanup logic
    if (m_nPlayerNumber == 2)  // Network Player #2
    {
        if (!SendLastMove(cGame)) // Send Player #1 last move
            return false;

        // Wait for opponent's (Player #2) move

        return RecvLastMove(cGame); // Receive opponent's (Player #2) last move
    }
    else // Network Player #1
    {
        if (Sending())
        {
            //SetToReceiving();  // For next turn

            if (!SendLastMove(cGame)) // Send Player #2 last move
                return false;

            // Wait for opponent's (Player #2) move

            return RecvLastMove(cGame); // Receive opponent's (Player #2) last move
        }
        else // Receiving
        {
            SetToSending(); // For next turn

            return RecvLastMove(cGame); // Receive opponent's (Player #1) last move
        }
    }
}

bool NetworkPlayer::SendLastMove(Game &cGame)
{
    std::string sCommand;
    std::string sToken;
    std::string sErrorMessage;

    GameMove cGameMove = cGame.LastMove();

    std::string sMessage = GameVocabulary::MOVE + " " + cGameMove.ToInputX() + cGameMove.ToInputY();

    std::cout << "Sending move to opponent." << std::endl;

    if (!Socket::Send(sMessage))
    {
        sErrorMessage = "Could not send command: " + sCommand;
        Socket::Send(GameVocabulary::FATAL_EXIT);
        throw SocketException(sErrorMessage);
    }

    if (!Socket::Recv(sCommand) < 0)
        throw SocketException("Did not receive move confirmation");

    sToken = GameVocabulary::ParseCommand(sCommand);
    if (sToken.compare(GameVocabulary::CONFIRM) != 0)
    {
        if (sToken.compare(GameVocabulary::UNCONFIRM) == 0)
        {
            sErrorMessage  = "Opponent did not confirm " + sMessage;
            std::cerr << sErrorMessage << std::endl;
            std::cout << "Exiting" << std::endl;
            Socket::Send(GameVocabulary::FATAL_EXIT);
            throw GameAIException(sErrorMessage);
        }
        else
        {
            sErrorMessage  = "Expected command " + GameVocabulary::CONFIRM + ", but received " + sCommand;
            std::cerr << sMessage << std::endl;
            std::cout << "Exiting" << std::endl;
            Socket::Send(GameVocabulary::FATAL_EXIT);
            throw GameAIException(sMessage);
        }
    }

    return true;
}


bool NetworkPlayer::RecvLastMove(Game &cGame)
{
    GameMove cGameMove;
    std::string sCommand;
    std::string sToken;
    std::string sErrorMessage;

    std::cout << "Waiting for opponent's move" << std::endl;

    if (!Socket::Recv(sCommand) < 0)
        throw SocketException("Did not receive move command");

    sToken = GameVocabulary::ParseCommand(sCommand);
    if (sToken.compare(GameVocabulary::MOVE) != 0)
    {
        sErrorMessage  = "Expected command " + GameVocabulary::MOVE + ", but received " + sCommand;
        std::cerr << sErrorMessage << std::endl;
        std::cout << "Exiting" << std::endl;
        Socket::Send(GameVocabulary::FATAL_EXIT);
        throw GameAIException(sErrorMessage);
    }

    sToken = GameVocabulary::ParseArgument(sCommand);
    std::cout << "Received move " << sToken << " from Player " << std::to_string(m_nPlayerNumber) << std::endl;

    cGameMove = cGame.GenerateMove(sToken);

    // Test move for validity.
    if (cGame.ApplyMove(m_nPlayerNumber, cGameMove))
    {
        sCommand = GameVocabulary::CONFIRM;
        if (!Socket::Send(sCommand))
        {
            sErrorMessage = "Could not send command: " + sCommand;
            throw SocketException(sErrorMessage);
        }
    }
    else
    {
        sErrorMessage = "Move " + sToken + " is invalid";
        std::cerr << sErrorMessage << std::endl;
        std::cout << "Exiting" << std::endl;
        Socket::Send(GameVocabulary::UNCONFIRM);
        Socket::Send(GameVocabulary::FATAL_EXIT);
        throw GameAIException(sErrorMessage);
    }

    return true;
}
