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

/** \file
 *
 * \brief The Socket class provides basic socket methods.
 *
 */

#ifndef SOCKET_H
#define SOCKET_H

#include <cstring>
#include <iostream>

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif // defined

#include <unistd.h>

// Maximum number of backlogged messages in queue
const int BACKLOG = 5;
// Largest message to receive
const int MAXRECV = 500;

class Socket
{
    public:
        // Constructor
        Socket();
        // Destructor
        virtual ~Socket();

        // Server and Client initialization
        bool Create();

        // Server setup
        bool Bind(const int port);
        bool Listen() const;
        bool Accept();

        // Client setup
        bool Connect (const std::string host, const int port);

        // Data transimission
        bool Send(const std::string) const;
        int  Recv(std::string&) const;

        //void SetNonBlocking(const bool);

        // Evaluate validity of socket
        bool IsValid() const { return m_nSocketID != -1; }

    private:
        // Server socket ID
        int         m_nSocketID;
        // Established client socket ID
        int         m_nSendRecvSocketID;
        // Socket structure
        sockaddr_in m_SocketAddress;
};

#endif // SOCKET_H
