#include "Logger.h"

std::string Logger::TimeStamp()
{
    std::string sTimeStamp {};
    int nBufLen = 25;

    if (m_bUseTimeStamp)
    {
        std::time_t t = std::time(nullptr);
        char* pcBuffer = new char[nBufLen];

        size_t nLen = strftime(pcBuffer, nBufLen, "%F %T", localtime(&t));
        sTimeStamp = std::string(pcBuffer, nLen);
    }

    return sTimeStamp;
}

void Logger::Log(const std::string sTag, const std::string sMessage, int nLevel)
{
    std::string sLog(TimeStamp());

    if (m_bUseTag)
    {
        sLog.append(" ");
        sLog.append(sTag);
        sLog.append(" ");
    }

    if (m_bUseLevelIndent)
    {
        sLog.insert(0,4 * (nLevel -1), ' ');
    }

    std::cout << sLog << sMessage << std::endl;
}

void Logger::LogInfo(const std::string sMessage, int nLevel=0)
{
    if (m_nLevel >= nLevel)
        Log("INFO:", sMessage, nLevel);

}

void Logger::LogWarn(const std::string sMessage)
{
    Log("WARN:", sMessage,0);
}

void Logger::LogError(const std::string sMessage)
{
    Log("ERROR:", sMessage,0);
}

void Logger::LogFatal(const std::string sMessage)
{
    Log("FATAL:", sMessage,0);
}

void Logger::LogDebug(const std::string sMessage)
{
    Log("DEBUG:", sMessage,0);
}
