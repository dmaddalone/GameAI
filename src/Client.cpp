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

/**
  * Construct a Client class.
  *
  */

Client::Client(PlayerType ecPlayerType) : NetworkPlayer(ecPlayerType)
{}

/**
  * Initialize the client.
  *
  * Create the client socket.  Upon connection, walk through setup dialog
  * with the server.
  *
  * \param sHost Host name or IP address
  * \param nport Port specification
  * \param bSwap Flag to indicate to caller that the server player number
  *  should be swapped with the other player.
  *
  */

void Client::Initialize(std::string sHost, int nPort, bool &bSwap)
{
    // Do not swap
    bSwap = false;

    std::string sCommand;
    std::string sErrorMessage;
    std::string sToken;
    std::string sMessage;

    // Change logger settings
    m_cLogger.UseLevelIndent(false);
    m_cLogger.UseTag(true);

    // Create and connect on the socket
    if (!Socket::Create())
        throw SocketException ("Could not create client socket.");

    // Log the socket creation
    m_cLogger.LogInfo("Socket created", 3);

    if (!Socket::Connect(sHost, nPort))
        throw SocketException ("Could not connect to port.");

    // Log the connection
    sMessage = "Connected to server " + sHost + " on port " + std::to_string(nPort);
    m_cLogger.LogInfo(sMessage, 3);
    //std::cout << "Client connected to server " << sHost << " on port " << nPort << std::endl;

    // Upon creating a connection, begin the following dialog to setup
    // a game with the server:

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

    sMessage = "Establishing game of " + GameTitle() + " with server.";
    m_cLogger.LogInfo(sMessage, 2);

    // 1. Send Establish Game
    sCommand = GameVocabulary::ESTABLISH_GAME + GameVocabulary::DELIMETER + GameTitle();
    Send(sCommand);

    // 4. Receive Confirm
    RecvConfirmation();

    // 5. Request Player Number
    m_cLogger.LogInfo("Requesting player number from server.", 2);

    sCommand = GameVocabulary::REQUEST_CLIENT_PLAYER_NUMBER;
    Send(sCommand);

    // 8. Receive Client Player Number
    if (!Socket::Recv(sCommand) < 0)
        throw SocketException("Did not receive player number");

    sToken = GameVocabulary::ParseCommand(sCommand);
    if (sToken.compare(GameVocabulary::ESTABLISH_CLIENT_PLAYER_NUMBER) != 0)
    {
        sErrorMessage  = "Expected command " + GameVocabulary::ESTABLISH_CLIENT_PLAYER_NUMBER + ", but received " + sCommand;
        std::cerr << sErrorMessage << std::endl;
        std::cout << "Exiting" << std::endl;
        Socket::Send(GameVocabulary::FATAL_EXIT);
        exit(EXIT_FAILURE);
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
        // Change player numbers on the client
        m_cLogger.LogInfo("Swapping player numbers.", 3);
        bSwap = true;
    }
}
