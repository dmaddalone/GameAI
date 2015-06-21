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

#include <climits>
#include <iostream>

#include "BoardGame.h"

class LinearGame : public BoardGame
{
    public:
        // Construct a LinearGame
        LinearGame(GameType ecGameType, int nX, int nY, char cToken1, char cToken2, int nInARow, bool bUseY, bool bDisplayGrid, bool bDisplayXCoordinates, bool bDisplayYCoordinates) :
            BoardGame(ecGameType, nX, nY, 2, bDisplayGrid, bDisplayXCoordinates, bDisplayYCoordinates),
            m_kcToken1(cToken1),
            m_kcToken2(cToken2),
            m_knTokensInARowWin(nInARow),
            m_bUseY(bUseY)
            {  SetTokens(); }

        // Destructor
        virtual ~LinearGame() {}

        // Generate a GameMove from text input
        virtual GameMove GenerateMove(std::string sMove) const override;
        // Provide a preferred move
        virtual int  PreferredMove(const GameMove &cGameMove) const override;
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
        void SetTokens() { m_acTokens[1] = m_kcToken1; m_acTokens[2] = m_kcToken2; }

        // Check to see if a players tokens are connected linearly
        bool CheckOrthogonal(int nPlayer, int nConnect);
        int  CheckHorizontal(int nPlayer, int y, int x) const;
        int  CheckVertical(int nPlayer, int y, int x) const;
        bool CheckDiagonal(int nPlayer, int nConnect);
        int  CheckDiagonalUpperLeftLowerRight(int nPlayer, int y, int x) const;
        int  CheckDiagonalUpperRightLowerLeft(int nPlayer, int y, int x) const;

        // Player 1's token
        const char m_kcToken1;
        // Player 2's token
        const char m_kcToken2;

        // For games that require tokens in a row
        const int m_knTokensInARowWin;

        // Use of the Y-Coordinate
        const int m_bUseY;

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
