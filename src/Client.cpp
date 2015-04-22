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

#include "Client.h"

Client::Client(PlayerType ecPlayerType, std::string sHost, int nPort ) : Player(ecPlayerType)
{
    if (!Socket::Create())
        throw SocketException ("Could not create client socket.");

    std::cout << "Connecting to server ..." << std::endl;
    if (!Socket::Connect(sHost, nPort))
        throw SocketException ("Could not connect to port.");

    std::cout << "Client connected to server " << sHost << " on port " << nPort << std::endl;
}

void Client::Initialize(bool &bSwap)
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
    // 6.                                   Receive Request Player Number
    // 7.                                   Send Client Player Number
    // 8.    Receive Client Player Number
    // 9.    Send Start Game
    // 10.                                  Receive Start Game

    // 1. Send Establish Game
    std::cout << "Establishing game of " << GameTitle() << " with server." << std::endl;
    sCommand = GameVocabulary::ESTABLISH_GAME + GameVocabulary::DELIMETER + GameTitle();
    if (!Socket::Send(sCommand))
    {
        sMessage = "Could not send command: " + sCommand;
        throw SocketException(sMessage);
    }

    // 4. Receive Confirm
    if (!Socket::Recv(sCommand) < 0)
        throw SocketException("Did not receive game confirmation");

    sToken = GameVocabulary::ParseCommand(sCommand);
    if (sToken.compare(GameVocabulary::CONFIRM) != 0)
    {
        if (sToken.compare(GameVocabulary::UNCONFIRM) == 0)
        {
            sMessage  = "Server did not confirm game of " + GameTitle();
            std::cerr << sMessage;
            std::cout << "Exiting";
            Socket::Send(GameVocabulary::FATAL_EXIT);
            throw GameAIException(sMessage);
        }
        else
        {
            sMessage  = "Expected command " + GameVocabulary::CONFIRM + ", but received " + sToken;
            std::cerr << sMessage;
            std::cout << "Exiting";
            Socket::Send(GameVocabulary::FATAL_EXIT);
            throw GameAIException(sMessage);
        }
    }

    // 5. Request Player Number
    std::cout << "Requesting player number from server." << std::endl;
    sCommand = GameVocabulary::REQUEST_CLIENT_PLAYER_NUMBER;
    if (!Socket::Send(sCommand))
    {
        sMessage = "Could not send command: " + sCommand;
        throw SocketException(sMessage);
    }

    // 8. Receive Client Player Number TODO
    if (!Socket::Recv(sCommand) < 0)
        throw SocketException("Did not receive player number");

    sToken = GameVocabulary::ParseCommand(sCommand);
    if (sToken.compare(GameVocabulary::ESTABLISH_CLIENT_PLAYER_NUMBER) != 0)
    {
        sMessage  = "Expected command " + GameVocabulary::ESTABLISH_CLIENT_PLAYER_NUMBER + ", but received " + sToken;
        std::cerr << sMessage;
        std::cout << "Exiting";
        Socket::Send(GameVocabulary::FATAL_EXIT);
        throw GameAIException(sMessage);
    }

    // Server will establish client's player number as the opposite of what the client
    // has established.  E.g.,
    //   Server: GameAI -1 human  -2 client -g ttt // Meaning on the server, human plays first and client plays second
    //                                             // and server sends 2 as the client player number.
    //
    //   Client: GameAI -1 server -2 human  -g ttt // Meaning on the client, server plays first and human plays second
    //                                             // and client has 1 as the server player number.  This is aligned.
    sToken = GameVocabulary::ParseArgument(sCommand);
    if (sToken.compare(std::to_string(2 - m_nPlayerNumber + 1)) != 0) // Player numbers are aligned
    {
        // change player numbers on the client
        bSwap = true;
    }
    sMessage = "Server playing " + std::to_string((2 - stoi(sToken) + 1))  + " and client is playing " + sToken;
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

bool Client::Move(Game &cGame)
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
