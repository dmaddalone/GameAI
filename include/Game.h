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

#include <algorithm>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

#include "GameMove.h"
#include "GameVocabulary.h"
#include "Logger.h"

// Used to identify the type of game
enum class GameType
{
    TYPE_NONE,
    TYPE_CONNECT_FOUR,
    TYPE_TTT,
    TYPE_REVERSI,
    TYPE_CHESS_GARDNER,
    TYPE_CHESS_BABY,
    TYPE_CHESS_JACOBS_MEIROVITZ,
    TYPE_CHESS_PETTY,
    TYPE_CHESS_SPEED,
    TYPE_CHESS_QUICK,
    TYPE_CHESS_ELENA,
    TYPE_CHESS_ATTACK,
    TYPE_CHESS_LOS_ALAMOS,
    TYPE_CHESS,
    TYPE_CARDGAME_WAR,
    TYPE_CARDGAME_GO_FISH
};

class Game
{
    public:
        // Construct a game of ecGameType and two players
        Game(GameType ecGameType)
        { m_ecGameType = ecGameType; }

        //
        // Virtual functions to be defined by child classes
        //

        // Destructor
        virtual ~Game() {};

        // Display the game
        virtual void Display() const  = 0;

        // Synchronize the game environment across networked players
        virtual bool GetSyncInfo(std::string &sGameInformation) = 0;
        virtual bool ApplySyncInfo(const std::string &sGameInformation, std::string &sErrorMessage) = 0;

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
        virtual std::string Title() const = 0;

        // Return the description of the game
        virtual std::string Description() const = 0;

        // Make a game of ecGameType
        static std::unique_ptr<Game> Make(GameType ecGameType);

        // Set the level of logging
        void SetVerbosity(int n)  { m_cLogger.SetLevel(n); m_cLogger.UseTimeStamp(false); m_cLogger.UseTag(false); m_cLogger.UseLevelIndent(true); }

        // Read moves from a text file and apply them
        virtual int  ReadMovesFromFile(const std::string &sFileName);

        // Write Moves to a text file
        virtual bool WriteMovesToFile(const std::string &sFileName);

        // Set Player information
        void SetPlayer1Name(std::string sName) { m_sPlayer1Name.assign(sName); }
        void SetPlayer2Name(std::string sName) { m_sPlayer2Name.assign(sName); }
        void SetPlayer1Type(std::string sType) { m_sPlayer1Type.assign(sType); }
        void SetPlayer2Type(std::string sType) { m_sPlayer2Type.assign(sType); }

        // Set sync game information
        bool Sync() const     { return m_bSync; }
        void SetSync(bool b)  { m_bSync = b; }

        // Return game information
        GameType Type() const      { return m_ecGameType; }
        int  Player1() const       { return m_knPlayer1; }
        int  Player2() const       { return m_knPlayer2; }
        GameMove LastMove() const  { if (!m_vGameMoves.empty()) return m_vGameMoves.back(); else { GameMove cGameMove; cGameMove.SetNoMove(true); return cGameMove; } }
        int  Winner() const        { return m_nWinner; };
        int  NumberOfMoves() const { return m_nNumberOfMoves; }
        std::string WinBy() const  { return m_sWinBy; }
        bool GameOver() const      { return m_bGameOver; }

    protected:
        // Write and read moves from a file
        bool OpenFileForWrite(const std::string &sFileName, std::fstream &fsFile);
        bool WriteMoves(const std::string &sFileName, std::fstream &fsFile);
        bool OpenFileForRead(const std::string &sFileName, std::fstream &fsFile);
        int  ReadAndApplyMoves(const std::string &sFileName, std::fstream &fsFile);
        void CloseFile(std::fstream &fsFile);

        // Name of the program
        const std::string m_sProgramName {"GameAI"};

        // Player numbers
        const int m_knPlayer1 {1};
        const int m_knPlayer2 {2};

        // Player attributes
        std::string m_sPlayer1Name {"??"};
        std::string m_sPlayer1Type {"??"};
        std::string m_sPlayer2Name {"??"};
        std::string m_sPlayer2Type {"??"};

        // Default game type
        GameType m_ecGameType {GameType::TYPE_NONE};

        // List of all game moves
        std::vector<GameMove> m_vGameMoves;

        // Current winning players
        int m_nWinner {0};
        // Number of moves
        int m_nNumberOfMoves {0};
        // How the winner won
        std::string m_sWinBy  {};
        // Game Over flag
        bool m_bGameOver {false};

        // Flag to sync game information between networked players
        bool m_bSync      { false };

        // Create a Logger object
        Logger m_cLogger;

        // Number words
        const std::string m_asNumbers[5] {"zero", "one", "two", "three", "four"};
};

#endif // GAME_H
