#ifndef SOCKET_H
#define SOCKET_H

#include <sys/types.h>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#endif // defined

#include <unistd.h>

//const int MAXHOSTNAME = 200;
const int BACKLOG = 5;
const int MAXRECV = 500;

class Socket
{
    public:
        Socket();
        virtual ~Socket();

        // Server and Client initialization
        bool Create();

        // Server setup
        bool Bind(const int port);
        bool Listen() const;
        //bool Accept(Socket&) const;
        bool Accept();

        // Client setup
        bool Connect (const std::string host, const int port);

        // Data transimission
        bool Send(const std::string) const;
        int  Recv(std::string&) const;

        void SetNonBlocking(const bool);

        bool IsValid() const { return m_nSocketID != -1; }

    private:
        int         m_nSocketID;
        //int         m_nClientSocketID;
        int         m_nSendRecvSocketID;
        sockaddr_in m_SocketAddress;
};

#endif // SOCKET_H
