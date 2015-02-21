#ifndef LOGGER_H
#define LOGGER_H

#include <ctime>
#include <iostream>
#include <iomanip>

class Logger
{
    public:
        Logger()           : m_nLevel(0) {}
        Logger(int nLevel) : m_nLevel(nLevel) {}

        void SetLevel(int nLevel)   { m_nLevel = nLevel; }
        int  Level() const          { return m_nLevel; }
        void UseTimeStamp(bool b)   { m_bUseTimeStamp = b; }
        void UseTag(bool b)         { m_bUseTag = b; }
        void UseLevelIndent(bool b) { m_bUseLevelIndent = b; }

        void LogInfo(const std::string sMessage, int nLevel);
        void LogWarn(const std::string sMessage);
        void LogError(const std::string sMessage);
        void LogFatal(const std::string sMessage);
        void LogDebug(const std::string sMessage);

    private:
        std::string TimeStamp();
        void Log(const std::string sTag, const std::string sMessage, int nLevel);

        int  m_nLevel;
        bool m_bUseLevelIndent{false};
        bool m_bUseTimeStamp {false};
        bool m_bUseTag {true};
};

#endif // LOGGER_H
