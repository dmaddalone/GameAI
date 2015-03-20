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
 * \brief The Player class represents a game player.
 *
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <memory>

#include "Game.h"
#include "Logger.h"

// Used to identify the type of player
enum class PlayerType
{
    TYPE_NONE,
    TYPE_HUMAN,
    TYPE_MINIMAX
};

class Player
{
    public:
        // Construct a player of ecPlayerType and provide it with an identifying number
        ////Player(PlayerType ecPlayerType) { m_ecPlayerType = ecPlayerType; m_nPlayerNumber = m_nPlayerCount; m_nOpponentNumber = 2 - m_nPlayerNumber + 1; }
        Player(PlayerType ecPlayerType) { m_ecPlayerType = ecPlayerType; m_nPlayerNumber = m_nPlayerCount; }

        //
        // Virtual functions to be defined by child classes
        //

        // Destructor
        virtual ~Player() {};

        // Player provides a move
        virtual bool Move(Game &pcGame) = 0;

        // Return the type of player as a string
        virtual std::string TypeName()  = 0;


        // Make a player
        static std::unique_ptr<Player> MakePlayer(PlayerType ecPlayerType);

        // Return the type of player
        PlayerType Type() { return m_ecPlayerType; }

        // Set the level of logging
        void SetVerbosity(int n)  { m_cLogger.SetLevel(n); m_cLogger.UseTimeStamp(false); m_cLogger.UseTag(false); m_cLogger.UseLevelIndent(true); }

        // Set the number of plies a machine player will use to make a move
        void SetPlies(int nPlies) { m_nDepth = nPlies; }

        // Return the number of plies
        int  Plies()              { return m_nDepth; }

    protected:
        //int  m_nMove              {0};
        // Set the default player type to none
        PlayerType m_ecPlayerType {PlayerType::TYPE_NONE};
        // Set the player number to zero
        int  m_nPlayerNumber      {0};
        //int  m_nOpponentNumber    {0};
        // Set the depth of plies to four
        int  m_nDepth             {4}; // For AI

        // Create a Logger object
        Logger m_cLogger;

    private:
        // Used to create unique player identifier numbers
        static int m_nPlayerCount;
};

#endif // PLAYER_H
