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

/**
  * Construct a Server class.
  *
  */

Server::Server(PlayerType ecPlayerType) : NetworkPlayer(ecPlayerType)
{}

/**
  * Initialize the server.
  *
  * Create the server socket.  Upon connection, walk through setup dialog
  * with the client.
  *
  * \param sHost Host name or IP address
  * \param nport Port specification
  * \param bSwap Flag to indicate to caller that the server player number
  *  should be swapped with the other player.
  *
  */

void Server::Initialize(std::string sHost, int nPort, bool &bSwap)
{
    // sHose parameter not used
    (void)sHost;

    // Do not swap
    bSwap = false;

    std::string sCommand;
    std::string sErrorMessage;
    std::string sToken;
    std::string sMessage;

    // Change logger settings
    m_cLogger.UseLevelIndent(false);
    m_cLogger.UseTag(true);

    // Create, bind, listen, and accept on the socket
    if (!Socket::Create())
        throw SocketException("Could not create server socket");

    // Log the socket creation
    m_cLogger.LogInfo("Socket created", 3);

    if (!Socket::Bind(nPort))
        throw SocketException("Could not bind to port");

    // Log the address bind
    m_cLogger.LogInfo("Bind created", 3);

    if (!Socket::Listen())
        throw SocketException("Could not listen to socket");

    // Log the port listening
    sMessage = "Server listening on port " + std::to_string(nPort);
    m_cLogger.LogInfo(sMessage, 2);

    std::cout << "Waiting for client to connect" << std::endl;

    if (!Socket::Accept())
        throw SocketException("Could not accept connection");

    // Log the connection accepted
    m_cLogger.LogInfo("Accepted network connection", 3);

    // Upon accepting a connection, begin the following dialog to setup
    // a game with the client:

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
        throw GameAIException(sErrorMessage);
    }

    sToken = GameVocabulary::ParseArgument(sCommand);
    if (sToken.compare(GameTitle()) != 0)
    {
        sErrorMessage = "Server playing " + GameTitle() + " and client is playing " + sToken;
        std::cerr << sErrorMessage << std::endl;
        std::cout << "Exiting" << std::endl;
        Socket::Send(GameVocabulary::UNCONFIRM);
        throw GameAIException(sErrorMessage);
    }

    // 3. Send Confirm Game
    sMessage = "Confirming game of " + GameTitle() + " with client.";
    m_cLogger.LogInfo(sMessage, 2);

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
        throw GameAIException(sErrorMessage);
    }

    // 7. Send Client Player Number
    sMessage = "Establishing client player number of " + std::to_string(m_nPlayerNumber);
    m_cLogger.LogInfo(sMessage, 2);

    sCommand = GameVocabulary::ESTABLISH_CLIENT_PLAYER_NUMBER + GameVocabulary::DELIMETER + std::to_string(m_nPlayerNumber);
    if (!Socket::Send(sCommand))
    {
        sErrorMessage = "Could not send command: " + sCommand;
        throw SocketException(sErrorMessage);
    }
}
