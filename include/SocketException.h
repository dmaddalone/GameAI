#ifndef SOCKETEXCEPTION_H
#define SOCKETEXCEPTION_H

#include <stdexcept>
#include <string>

class SocketException : public std::runtime_error
{
    public:
        explicit SocketException(const std::string &s) : std::runtime_error(s) {}
};

#endif // SOCKETEXCEPTION_H
