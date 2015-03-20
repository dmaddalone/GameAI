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
 * \brief The Game class represents a virtual game.
 *
 */

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
