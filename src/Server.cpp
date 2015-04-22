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

#include "Server.h"

Server::Server(PlayerType ecPlayerType, int nPort) : Player(ecPlayerType)
{
    if (!Socket::Create())
        throw SocketException("Could not create server socket");

    if (!Socket::Bind(nPort))
        throw SocketException("Could not bind to port");

    if (!Socket::Listen())
        throw SocketException("Could not listen to socket");

    std::cout << "Server lisenting on port " << nPort << std::endl;

    if (!Socket::Accept())
        throw SocketException("Could not accept connection");
}

void Server::Initialize(bool &bSwap)
{
    bSwap = false;
    std::string sCommand;
    std::string sMessage;
    std::string sToken;

    // Step  Client                         Server
    // ----  ---------------------          ------------------------
    // 1.    Send Establish Game
    // 2.                                   Receive Establish Game
    // 3.                                   Send Confirm
    // 4.    Receive Confirm
    // 5.    Send Request Player Number
    // 6.                                   Receieve Request Player Number
    // 7.                                   Send Client Player Number
    // 8.    Receieve Client Player Number
    // 9.    Send Start Game
    // 10.                                   Receive Start Game


    // 2. Receive Establish Game
    if (!Socket::Recv(sCommand) < 0)
        throw SocketException("Did not receive establish game command");

    sToken = GameVocabulary::ParseCommand(sCommand);
    if (sToken.compare(GameVocabulary::ESTABLISH_GAME) != 0)
    {
        sMessage  = "Expected command " + GameVocabulary::ESTABLISH_GAME + ", but received " + sToken;
        std::cerr << sMessage;
        std::cout << "Exiting";
        Socket::Send(GameVocabulary::FATAL_EXIT);
        throw GameAIException(sMessage);
    }

    sToken = GameVocabulary::ParseArgument(sCommand);
    if (sToken.compare(GameTitle()) != 0)
    {
        sMessage = "Server playing " + GameTitle() + " and client is playing " + sToken;
        std::cerr << sMessage;
        std::cout << "Exiting";
        Socket::Send(GameVocabulary::UNCONFIRM);
        Socket::Send(GameVocabulary::FATAL_EXIT);
        throw GameAIException(sMessage);
    }

    // 3. Send Confirm Game
    std::cout << "Confirming game of " << GameTitle() << " with client." << std::endl;
    sCommand = GameVocabulary::CONFIRM;
    if (!Socket::Send(sCommand))
    {
        sMessage = "Could not send command: " + sCommand;
        throw SocketException(sMessage);
    }

    // 6. Receieve Request Player Number
    if (!Socket::Recv(sCommand) < 0)
        throw SocketException("Did not receive request for client player number");

    sToken = GameVocabulary::ParseCommand(sCommand);
    if (sToken.compare(GameVocabulary::REQUEST_CLIENT_PLAYER_NUMBER) != 0)
    {
        sMessage  = "Expected command " + GameVocabulary::REQUEST_CLIENT_PLAYER_NUMBER + ", but received " + sToken;
        std::cerr << sMessage;
        std::cout << "Exiting";
        Socket::Send(GameVocabulary::FATAL_EXIT);
        throw GameAIException(sMessage);
    }

    // 7. Send Client Player Number
    std::cout << "Establishing client player number of " << m_nPlayerNumber << " with client." << std::endl;
    sCommand = GameVocabulary::ESTABLISH_CLIENT_PLAYER_NUMBER + GameVocabulary::DELIMETER + std::to_string(m_nPlayerNumber);
    if (!Socket::Send(sCommand))
    {
        sMessage = "Could not send command: " + sCommand;
        throw SocketException(sMessage);
    }
}

/**
  * Generate the next game move.
  *
  * Use GameMove to collect the next game move from a client player.
  *
  * \param cGame The game.
  *
  * \return True, if a move has been made or no move to make.
  */

bool Server::Move(Game &cGame)
{
    GameMove cGameMove;

    // Show valid moves at proper logging level.
    if (m_cLogger.Level() >= 1)
    {
        cGame.Display();
        std::cout << "Valid moves: " << cGame.ValidMoves(m_nPlayerNumber) << std::endl;
    }

    // Generate all possible valid moves for this player
    std::vector<GameMove> vGameMoves = cGame.GenerateMoves(m_nPlayerNumber);

    // If no valid moves are possible, return true
    if (vGameMoves.empty())
    {
        return true;
    }

    do
    {
        std::cout << "Player " << m_nPlayerNumber << ", enter move: ";

        // Get move from human player.
        cGameMove = cGame.GetMove(m_nPlayerNumber);

        // Test move for validity.
        if (!cGame.ApplyMove(m_nPlayerNumber, cGameMove))
        {
            std::cout << "Invalid move" << std::endl;
        }
        else
        {
            break;
        }

    } while(true);

    return true;
}
