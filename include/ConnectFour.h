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
 * \brief The ConnectFour class represents a Connect Four game.
 *
 */

#ifndef CONNECTFOUR_H
#define CONNECTFOUR_H

#include <iostream>
#include "Game.h"

class ConnectFour : public Game
{
    public:
        ConnectFour(GameType ecGameType) : Game(ecGameType) {}
        ~ConnectFour() {}

        void Display(const bool bDisplayCoordinates = false) const;
        void DisplayValidMoves() const;

        GameMove GetMove() const;
        virtual void AnnounceMove(const int nPlayer, const GameMove &cGameMove);

        int  ApplyMove(const int nPlayer, GameMove &cGameMove);

        bool RetractMove(const int nPlayer, const GameMove &cGameMove);

        int  PreferredMove(const GameMove &cGameMove) const;
        bool GameEnded();

        std::vector<GameMove> GenerateMoves() const;
        int  EvaluateGameState(const int nPlayer);

        std::string Title() { return "ConnectFour"; }

    private:
        int  FindBottom(const int x) const;
        bool CheckOrthogonal(const int nPlayer, int nConnect) ;//const;
        int  CheckHorizontal(const int nPlayer, const int y, const int x) ;//const;
        int  CheckVertical(const int nPlayer, const int y, const int x) ;//const;
        bool CheckDiagonal(const int nPlayer, int nConnect) ;//const;
        int  CheckDiagonalUpperLeftLowerRight(const int nPlayer, const int y, const int x) ;//const;
        int  CheckDiagonalUpperRightLowerLeft(const int nPlayer, const int y, const int x) ;//const;
        bool ValidMove(const int y, const int x) const;


        static const int m_kY = 6;
        static const int m_kX = 7;
        static const int m_kClear = 0;
        static const int m_kPlayer1 = 1;
        static const int m_kPlayer2 = 2;

        int m_anGrid[m_kY][m_kX] {{m_kClear}};

        struct SequenceCounts {
            int nCount1 {0};
            int nCount2 {0};
            int nCount3 {0};
        } m_stMyCounts, m_stOpponentCounts;

        void CountSequence(int nSequence, SequenceCounts &stSequenceCounts);
};

#endif // CONNECTFOUR_H
