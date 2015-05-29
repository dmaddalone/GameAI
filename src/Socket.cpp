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

#include "Socket.h"

/**
  * Construct a Socket class.
  *
  * Initialize socket structure before creating a socket.
  *
  */

Socket::Socket() : m_nSocketID(-1)
{
    memset(&m_SocketAddress, 0, sizeof(m_SocketAddress));
}

/**
  * Deconstruct a Socket class.
  *
  * Close the socket.
  *
  */

Socket::~Socket()
{
    if (IsValid())
#if defined(_WIN32)
        closesocket(m_nSocketID);
#else
        close(m_nSocketID);
#endif // defined

}

/**
  * Create a socket.
  *
  * Create a stream socket.
  *
  * \return True if socket created.  False otherwise.
  */

bool Socket::Create()
{
#if defined(_WIN32)
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
        return false;
#endif

    // Create socket and socket ID
    m_nSocketID = socket(PF_INET, SOCK_STREAM, 0);

    // EValuate socket ID
    if (!IsValid())
        return false;

    // Make socket port reuseable
    int yes = 1;
    if (setsockopt(m_nSocketID, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(int)) == -1 )
        return false;

    return true;
}

/**
  * Bind a local address to the socket.
  *
  * Bind the socket to the port on the server.
  *
  * \param nPort Port specification
  *
  * \return True if bind is successful.  False otherwise.
  */

bool Socket::Bind(const int nPort)
{
    // Evaluate validity of socket id
    if (!IsValid())
        return false;

    // Set socket structure to port on the server
    m_SocketAddress.sin_family = AF_INET;
    m_SocketAddress.sin_port = htons(nPort);
    m_SocketAddress.sin_addr.s_addr = INADDR_ANY;

    // Associate the socket with our local endpoint
    if (bind(m_nSocketID, (struct sockaddr *)&m_SocketAddress, sizeof(m_SocketAddress)) == -1)
        return false;

  return true;
}

/**
  * Place socket into listening state.
  *
  * \return True if listen is successful.  False otherwise.
  */

bool Socket::Listen() const
{
    // Evaluate validity of socket ID
    if (!IsValid())
        return false;

    // Place socket into listening state
    if (listen(m_nSocketID, BACKLOG) == -1)
        return false;

    return true;
}

/**
  * Accept new connection.
  *
  * Create a new socket for newly created connection.
  *
  * \return True if accept is successful.  False otherwise.
  */

bool Socket::Accept()
{
    // Create a ndew socket for communications to the client
    m_nSendRecvSocketID = accept(m_nSocketID, (sockaddr *) NULL, NULL);

    if (m_nSendRecvSocketID == -1)
        return false;

    return true;
}

/**
  * Send data to a connected socket.
  *
  * \param nsMessage Data to be sent.
  *
  * \return True if send is successful.  False otherwise.
  */

bool Socket::Send(const std::string &sMessage) const
{
    if (send(m_nSendRecvSocketID, sMessage.c_str(), sMessage.size(), 0) == -1)
        return false;

    return true;
}

/**
  * Receive data from a connected socket.
  *
  * \param sMessage Data to be received.
  * \param nLength Length of message to receive, defaults to 0, meaning MAXRECV
  *
  * \return Size of message.  Zero on error.
  */

int Socket::Recv(std::string &sMessage, int nLength) const
{
    size_t nMessageLength;

    // Determine length of message to receive
    if (nLength > 0)
        nMessageLength = nLength;
    else
        nMessageLength = MAXRECV;

    // Clear message buffer
    char cBuf[nMessageLength + 1];
    memset(cBuf, 0, nMessageLength + 1);

    sMessage = ""; // TODO: Why not use cBuf and get rid of sMessage?

    // Reeive message
    int nStatus = recv(m_nSendRecvSocketID, cBuf, nMessageLength, 0);

    // Evaluate status
    if (nStatus == -1)
    {
        return 0;
    }
    else if (nStatus == 0)
    {
        return 0;
    }
    else
    {
        sMessage = cBuf;
        return nStatus;
    }
}

/**
  * Establish a connection to a host.
  *
  * \param sHost Host name or IP address
  * \param nport Port specification
  *
  * \return True if connect was successful.  False otherwise.
  */

bool Socket::Connect(const std::string sHost, const int nPort)
{
    // Evaluate socket ID
    if (!IsValid())
        return false;

    // Set up socket structure
    m_SocketAddress.sin_family = AF_INET;
    m_SocketAddress.sin_port = htons(nPort);
    m_SocketAddress.sin_addr.s_addr = inet_addr(sHost.c_str());

    // Connect to host
    if (connect(m_nSocketID, (sockaddr *)&m_SocketAddress, sizeof(m_SocketAddress)) == -1)
    {
#if defined(_WIN32)
        std::cerr << "Error when connecting " << WSAGetLastError() << std::endl;
#else
        perror("connect");
#endif // defined
        return false;
    }

    // Assign new connected socket ID
    m_nSendRecvSocketID = m_nSocketID;

    return true;
}

/*
void Socket::SetNonBlocking(const bool b )
{
    int nFlags = fcntl(m_nSocketID, F_GETFL);

    if (nFlags == -1)
        return;

    if (b)
        nFlags = (nFlags | O_NONBLOCK);
    else
        nFlags = (nFlags & ~O_NONBLOCK);

    fcntl(m_nSocketID, F_SETFL, nFlags);
}
*/
