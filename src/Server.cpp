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

Server::Server(PlayerType ecPlayerType) : NetworkPlayer(ecPlayerType)
{}

void Server::Initialize(std::string sHost, int nPort, bool &bSwap)
{
    (void)sHost;

    bSwap = false;
    std::string sCommand;
    std::string sErrorMessage;
    std::string sToken;

    if (!Socket::Create())
        throw SocketException("Could not create server socket");

    if (!Socket::Bind(nPort))
        throw SocketException("Could not bind to port");

    if (!Socket::Listen())
        throw SocketException("Could not listen to socket");

    std::cout << "Server listening on port " << nPort << std::endl;

    if (!Socket::Accept())
        throw SocketException("Could not accept connection");

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

    // 2. Receive Establish Game
    if (!Socket::Recv(sCommand) < 0)
        throw SocketException("Did not receive establish game command");

    sToken = GameVocabulary::ParseCommand(sCommand);
    if (sToken.compare(GameVocabulary::ESTABLISH_GAME) != 0)
    {
        sErrorMessage  = "Expected command " + GameVocabulary::ESTABLISH_GAME + ", but received " + sCommand;
        std::cerr << sErrorMessage << std::endl;
        std::cout << "Exiting" << std::endl;
        Socket::Send(GameVocabulary::FATAL_EXIT);
        throw GameAIException(sErrorMessage);
    }

    sToken = GameVocabulary::ParseArgument(sCommand);
    if (sToken.compare(GameTitle()) != 0)
    {
        sErrorMessage = "Server playing " + GameTitle() + " and client is playing " + sToken;
        std::cerr << sErrorMessage << std::endl;
        std::cout << "Exiting" << std::endl;
        Socket::Send(GameVocabulary::UNCONFIRM);
        Socket::Send(GameVocabulary::FATAL_EXIT);
        throw GameAIException(sErrorMessage);
    }

    // 3. Send Confirm Game
    std::cout << "Confirming game of " << GameTitle() << " with client." << std::endl;
    sCommand = GameVocabulary::CONFIRM;
    if (!Socket::Send(sCommand))
    {
        sErrorMessage = "Could not send command: " + sCommand;
        throw SocketException(sErrorMessage);
    }

    // 6. Receive Request Player Number
    if (!Socket::Recv(sCommand) < 0)
        throw SocketException("Did not receive request for client player number");

    sToken = GameVocabulary::ParseCommand(sCommand);
    if (sToken.compare(GameVocabulary::REQUEST_CLIENT_PLAYER_NUMBER) != 0)
    {
        sErrorMessage  = "Expected command " + GameVocabulary::REQUEST_CLIENT_PLAYER_NUMBER + ", but received " + sCommand;
        std::cerr << sErrorMessage << std::endl;
        std::cout << "Exiting" << std::endl;
        Socket::Send(GameVocabulary::FATAL_EXIT);
        throw GameAIException(sErrorMessage);
    }

    // 7. Send Client Player Number
    std::cout << "Establishing client player number of " << m_nPlayerNumber << std::endl;
    sCommand = GameVocabulary::ESTABLISH_CLIENT_PLAYER_NUMBER + GameVocabulary::DELIMETER + std::to_string(m_nPlayerNumber);
    if (!Socket::Send(sCommand))
    {
        sErrorMessage = "Could not send command: " + sCommand;
        throw SocketException(sErrorMessage);
    }
}
