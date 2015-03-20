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
 * \brief The Minimax class represents a computer game player, based
 * on the Minimax algorithm, using alpha-beta pruning.
 *
 */

#ifndef MINIMAX_H
#define MINIMAX_H

#include <climits>
#include <iostream>
#include "Player.h"
#include "Game.h"
#include "GameAIException.h"

class Minimax: public Player
{
    public:
        Minimax(PlayerType ecPlayerType) : Player(ecPlayerType) {};
        // Minimax(PlayerType ecPlayerType) : Player(ecPlayerType), m_acSpin{"\\|-/-"} {};
        ~Minimax() {};

        bool Move(Game &cGame);

        std::string TypeName() { return "Minimax"; }

    private:
        GameMove MinimaxMove(int nPlayer, Game &cGame, int nDepth);
        int MinMove(int nPlayer, Game &cGame, int nDepth, int nAlpha, int nBeta);
        int MaxMove(int nPlayer, Game &cGame, int nDepth, int nAlpha, int nBeta);

        //char m_acSpin[4] = {'|', '/', '--', '\\'};
        //char m_acSpin[] = "\\|-/-";
        //char m_acSpin[6];
        //int  m_nSpinIndex {0};
        //char Spin() { ++m_nSpinIndex; if (m_nSpinIndex > 5) m_nSpinIndex = 0; return m_acSpin[m_nSpinIndex]; }
};

#endif // MINIMAX_H
