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
 * \brief The LinearGame class represents a virtual game played on a
 * two-dimensonal space, navigated with X and Y coordinates, and using one type
 * of token per player.
 *
 */

#ifndef LINEARGAME_H
#define LINEARGAME_H

#include <cctype>
#include <climits>
#include <iostream>
#include <string>

#include "Game.h"

class LinearGame : public Game
{
    public:
        // Construct a LinearGame
        LinearGame(GameType ecGameType, int nX, int nY, char cClear, char cToken1, char cToken2, int nWin, bool bDisplayGrid, bool bDisplayXCoordinates, bool bDisplayYCoordinates) :
            Game(ecGameType),
            m_knX(nX),
            m_knY(nY),
            m_kcClear(cClear),
            m_kcToken1(cToken1),
            m_kcToken2(cToken2),
            m_knWin(nWin),
            m_kbDisplayGrid(bDisplayGrid),
            m_kbDisplayXCoordinates(bDisplayXCoordinates),
            m_kbDisplayYCoordinates(bDisplayYCoordinates)
            {  SetTokens(); ClearBoard(); }

        // Destructor
        ~LinearGame() {}

        // Display the game board
        virtual void Display() const override;
        // Return a list of valid moves in string format
        virtual std::string ValidMoves(int nPlayer) const override;
        // Get the move from the designated player
        virtual GameMove GetMove(int nPlayer) const override;
        // Generate a GameMove from text input
        virtual GameMove GenerateMove(std::string sMove) const override;
        // Provide a preferred move
        virtual int  PreferredMove(const GameMove &cGameMove) const override;
        // Return the score of the game
        virtual std::string GameScore() const override;
        // Apply the move to the game
        virtual bool ApplyMove(int nPlayer, GameMove &cGameMove) override;
        // Announce the move made
        virtual std::string AnnounceMove(int nPlayer, const GameMove &cGameMove) const override;
        // Evaluate the game state from the perspective of the nPlayer
        virtual int  EvaluateGameState(int nPlayer) override;
        // Check to see if the game has ended
        virtual bool GameEnded(int nPlayer) override;
        // Clone the current game
        virtual std::unique_ptr<Game> Clone() const = 0;

    protected:
        // Set the tokens to be used for clear and both players
        void SetTokens() { m_acTokens[0] = m_kcClear; m_acTokens[1] = m_kcToken1; m_acTokens[2] = m_kcToken2; }
        // Remove all tokens from the board
        void ClearBoard();
        // Set up the board
        void SetBoard() { return; };
        // Check to see if a players tokens are connected linearly
        bool CheckOrthogonal(int nPlayer, int nConnect);
        int  CheckHorizontal(int nPlayer, int y, int x) const;
        int  CheckVertical(int nPlayer, int y, int x) const;
        bool CheckDiagonal(int nPlayer, int nConnect);
        int  CheckDiagonalUpperLeftLowerRight(int nPlayer, int y, int x) const;
        int  CheckDiagonalUpperRightLowerLeft(int nPlayer, int y, int x) const;
        // Check for whether a X- and Y-coordinate are valid (on the board)
        bool ValidMove(int y, int x) const;

        // Max X-coordinate for this game
        const int  m_knX;
        // Max Y-coordinate this game
        const int  m_knY;
        // Clear token (no token)
        const char m_kcClear;
        // Player 1's token
        const char m_kcToken1;
        // Player 2's token
        const char m_kcToken2;
        // Winning player
        const int m_knWin;
        // Booleans to display game information
        const bool m_kbDisplayGrid;
        const bool m_kbDisplayXCoordinates;
        const bool m_kbDisplayYCoordinates;
        // Used to convery alpha X coordinates to integer values for internal representation
        const char m_kcXCoordinate {'a'};
        // Max X-coordinate for any game
        static const int m_knMaxX {8};
        // Max Y-coordinate for any game
        static const int m_knMaxY {8};
        // The board for any game
        int m_acGrid[m_knMaxY][m_knMaxX] {{}};
        // Number of tokens
        static const int m_knTokens {3};
        // Array used to hold tokens
        char m_acTokens[m_knTokens];
        // Struct used to evaluate the game state
        struct SequenceCounts {
            int nCount1 {0};
            int nCount2 {0};
            int nCount3 {0};
        } m_stMyCounts, m_stOpponentCounts;

        // Count the number and length of connected tokens per player
        void CountSequence(int nSequence, SequenceCounts &stSequenceCounts);
};

#endif // LINEARGAME_H
