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
 * \brief The Logger class provides simple logging capabilities.
 *
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <ctime>
#include <iostream>

class Logger
{
    public:
        // Construct a Logger class with default logging level of zero
        Logger()           : m_nLevel(0) {}
        // Construct a Logger class with logging level set as a parameter
        Logger(int nLevel) : m_nLevel(nLevel) {}

        // Set and return the logging level
        void SetLevel(int nLevel)   { m_nLevel = nLevel; }
        int  Level() const          { return m_nLevel; }

        // Booleans to use a time stamp, use a logging tag, and indenting the log message
        void UseTimeStamp(bool b)   { m_bUseTimeStamp = b; }
        void UseTag(bool b)         { m_bUseTag = b; }
        void UseLevelIndent(bool b) { m_bUseLevelIndent = b; }

        // Calls for different types of logging
        void LogInfo(const std::string sMessage, int nLevel) const;
        void LogWarn(const std::string sMessage);
        void LogError(const std::string sMessage);
        void LogFatal(const std::string sMessage);
        void LogDebug(const std::string sMessage);

    private:
        // Generate a time stamp
        std::string TimeStamp() const;
        // Write the logging message
        void Log(const std::string sTag, const std::string sMessage, int nLevel) const;

        // The logging level
        int  m_nLevel;
        // Use indentation in the log message
        bool m_bUseLevelIndent{false};
        // Use a time stamp in the log message
        bool m_bUseTimeStamp {false};
        // Use a tag in the log message
        bool m_bUseTag {true};
};

#endif // LOGGER_H
