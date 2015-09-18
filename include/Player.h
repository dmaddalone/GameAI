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

#include "Game.h"
#include "Logger.h"

// Used to identify the type of player
enum class PlayerType
{
    TYPE_NONE,
    TYPE_HUMAN,
    TYPE_AI,
    TYPE_SERVER,
    TYPE_CLIENT
};

class Player
{
    public:
        // Construct a player of ecPlayerType and provide it with an identifying number
        Player(PlayerType ecPlayerType) { m_ecPlayerType = ecPlayerType; m_nPlayerNumber = m_nPlayerCount; }

        //
        // Virtual functions to be defined by dervied classes
        //

        // Destructor
        virtual ~Player() {};

        // Player initialization
        virtual void Initialize(std::string sHost, int nPort, bool &bSwap) = 0;

        // Player provides a move
        virtual bool Move(Game &cGame) = 0;

        // Player actions at end of game
        virtual bool Finish(Game &cGame) = 0;

        // Return the type of player as a string
        virtual std::string TypeName() const = 0;

        // Set and return the name of the player as a string
        void SetPlayerName(std::string sName) { m_sPlayerName.assign(sName); }
        std::string PlayerName() const        { return m_sPlayerName; }

        // Make a player
        static std::unique_ptr<Player> MakePlayer(PlayerType ecPlayerType);

        // Set player number
        void SetPlayerNumber(int nNumber) { m_nPlayerNumber = nNumber; }
        int PlayerNumber() const          { return m_nPlayerNumber; }

        // Return the type of player
        PlayerType Type() const { return m_ecPlayerType; }

        // Set the level of logging
        void SetVerbosity(int n)  { m_cLogger.SetLevel(n); m_cLogger.UseTimeStamp(false); m_cLogger.UseTag(false); m_cLogger.UseLevelIndent(true); }

        // Set the number of plies a machine player will use to make a move
        void SetPlies(int nPlies) { m_nDepth = nPlies; }

        // Return the number of plies
        int  Plies() const        { return m_nDepth; }

        // Set the probability threshold a machine player will use to make a move
        void SetProbability(int nProb) { m_nProbabilityThreshold = nProb; }

        // Return the probability threshold
        int  Probability() const        { return m_nProbabilityThreshold; }

        // Set and get game title
        void SetGameTitle(std::string sTitle) { m_sGameTitle = sTitle; }
        std::string GameTitle() const         { return m_sGameTitle; }

    protected:
        // Set the default player type to none
        PlayerType m_ecPlayerType {PlayerType::TYPE_NONE};

        // Set the player number to zero
        int  m_nPlayerNumber      {0};

        // Player name
        std::string m_sPlayerName {};

        // Set the depth of plies to four
        int  m_nDepth             {4}; // For AI

        // Set the probability threshold to 5 (a.k.a., 0.5, 50%)
        int m_nProbabilityThreshold {5};

        // Game title - used for network play to coordinate game
        std::string m_sGameTitle  {""};

        // Create a Logger object
        Logger m_cLogger;

    private:
        // Used to create unique player identifier numbers
        static int m_nPlayerCount;
};

#endif // PLAYER_H
