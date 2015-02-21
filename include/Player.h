/*
    Copyright 2014 Dom Maddalone

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
 * \brief The Player class represents a game player.
 *
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <memory>

#include "Game.h"
#include "Logger.h"

enum class PlayerType
{
    TYPE_NONE,
    TYPE_HUMAN,
    TYPE_MINIMAX
};

class Player
{
    public:
        Player(PlayerType ecPlayerType) { m_ecPlayerType = ecPlayerType; m_nPlayerNumber = m_nPlayerCount; m_nOpponentNumber = 2 - m_nPlayerNumber + 1; }
        virtual ~Player() {};

        static std::unique_ptr<Player> MakePlayer(PlayerType ecPlayerType);

        virtual bool Move(Game &pcGame) = 0;

        virtual std::string TypeName()  = 0;
        PlayerType Type() { return m_ecPlayerType; }

        void SetVerbosity(int n)  { m_cLogger.SetLevel(n); m_cLogger.UseTimeStamp(false); m_cLogger.UseTag(false); m_cLogger.UseLevelIndent(true); }
        void SetPlies(int nPlies) { m_nDepth = nPlies; }
        int  Plies()              { return m_nDepth; }

    protected:
        int  m_nMove              {0};
        PlayerType m_ecPlayerType {PlayerType::TYPE_NONE};
        int  m_nPlayerNumber      {0};
        int  m_nOpponentNumber    {0};
        int  m_nDepth             {4}; // For AI
        //int  m_nVerbosity         {1};

        Logger m_cLogger;

    private:
        static int m_nPlayerCount;
};

#endif // PLAYER_H
