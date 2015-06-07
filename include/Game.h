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
 * \brief The Game class represents a virtual game, with two players.
 *
 */

#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

#if defined(_WIN32)
//#include <winsock2.h>
//#include <windows.h>
//#include <wincon.h>
#endif

#include "GameMove.h"
#include "Logger.h"

// Used to identify the type of game
enum class GameType
{
    TYPE_NONE,
    TYPE_CONNECT_FOUR,
    TYPE_TTT,
    TYPE_REVERSI,
    TYPE_CHESSGARDNER,
    TYPE_CHESSBABY,
    TYPE_CHESSJACOBSMEIROVITZ,
    TYPE_CHESSPETTY
};

class Game
{
    public:
        // Construct a game of ecGameType and two players
        Game(GameType ecGameType) :
        m_knPlayer1(1),
        m_knPlayer2(2),
        m_nWinner(0),
        m_nNumberOfMoves(0)
        { m_ecGameType = ecGameType; }

        //
        // Virtual functions to be defined by child classes
        //

        // Destructor
        virtual ~Game() {};

        // Display the game
        virtual void Display() const  = 0;

        // Return a list of valid moves in string format
        virtual std::string ValidMoves(int nPlayer) const = 0;

        // Get the move from the designated player
        virtual GameMove GetMove(int nPlayer) const = 0;

        // Generate a GameMove from text input
        virtual GameMove GenerateMove(std::string sMove) const = 0;

        // Apply the move to the game
        virtual bool ApplyMove(int nPlayer, GameMove &cGameMove) = 0;

        // Announce the move made
        virtual std::string AnnounceMove(int nPlayer, const GameMove &cGameMove) const = 0;

        // Provide a preferred move
        virtual int  PreferredMove(const GameMove &cGameMove) const = 0;

        // Return the score of the game
        virtual std::string GameScore() const = 0;

        // Check to see if the game has ended
        virtual bool GameEnded(int nPlayer) = 0;

        // Generate a vector of valid moves
        virtual std::vector<GameMove> GenerateMoves(int nPlayer) const = 0;

        // Evaluate the game state from the perspective of the nPlayer
        virtual int  EvaluateGameState(int nPlayer) = 0;

        // Clone the current game
        virtual std::unique_ptr<Game> Clone() const = 0;

        // Return the title of the game
        virtual std::string Title() = 0;


        // Make a game of ecGameType
        static std::unique_ptr<Game> Make(GameType ecGameType);

        // Set the level of logging
        void SetVerbosity(int n)  { m_cLogger.SetLevel(n); m_cLogger.UseTimeStamp(false); m_cLogger.UseTag(false); m_cLogger.UseLevelIndent(true); }

        // Read moves from a text file and apply them
        int ReadMoves(std::string sFileName);

        // Write moves to a text file
        bool WriteMoves(std::string sFileName);

        // Return game information
        GameType Type() const      { return m_ecGameType; }
        int  Player1() const       { return m_knPlayer1; }
        int  Player2() const       { return m_knPlayer2; }
        GameMove LastMove() const  { if (!m_vGameMoves.empty()) return m_vGameMoves.back(); else { GameMove cGameMove; cGameMove.SetNoMove(true); return cGameMove; } }
        int  Winner() const        { return m_nWinner; };
        int  NumberOfMoves() const { return m_nNumberOfMoves; }
        std::string WinBy() const  { return m_sWinBy; }

    protected:
        // Player numbers, defined in the Constructor
        const int m_knPlayer1;//  {1};
        const int m_knPlayer2;//  {2};

        // Default game type
        GameType m_ecGameType {GameType::TYPE_NONE};

        // List of all game moves
        std::vector<GameMove> m_vGameMoves;

        // Current winning players
        int m_nWinner;//         {0};
        // Number of moves
        int m_nNumberOfMoves;//  {0};
        // How the winner won
        std::string m_sWinBy  {};

        // Create a Logger object
        Logger m_cLogger;

};

#endif // GAME_H
