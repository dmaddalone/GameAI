#ifndef GAMEAIEXCEPTION_H
#define GAMEAIEXCEPTION_H

#include <stdexcept>
#include <string>

class GameAIException : public std::logic_error
{
    public:
        explicit GameAIException(const std::string &s) : std::logic_error(s) {}

        GameAIException(const std::string &sLoc, const std::string &sArg) :
            std::logic_error(sLoc),
            m_sArg(sArg)
        { }

        const std::string m_sArg;
};

#endif // GAMEAIEXCEPTION_H
