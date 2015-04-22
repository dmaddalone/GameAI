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
        // Construct a Minimax object
        Minimax(PlayerType ecPlayerType) : Player(ecPlayerType) {};

        //// TODO: Consider an rotating icon to indicate "thinking"
        //// Minimax(PlayerType ecPlayerType) : Player(ecPlayerType), m_acSpin{"\\|-/-"} {};

        // Destructor
        ~Minimax() {};

        // Initializer
        virtual void Initialize(bool &bSwap) override { (void)bSwap; };

        // Generate the next game move
        virtual bool Move(Game &cGame) override;

        // Announce the type pf player
        std::string TypeName() { return "Minimax"; }

    private:
        // Generate the best move using the minimax algorithm
        GameMove MinimaxMove(int nPlayer, Game &cGame, int nDepth);
        // Generate a move by the opponent player
        int MinMove(int nPlayer, Game &cGame, int nDepth, int nAlpha, int nBeta);
        // Generate a move for the player
        int MaxMove(int nPlayer, Game &cGame, int nDepth, int nAlpha, int nBeta);

        //// TODO: Variables for rotating icon
        ////char m_acSpin[4] = {'|', '/', '--', '\\'};
        ////char m_acSpin[] = "\\|-/-";
        ////char m_acSpin[6];
        ////int  m_nSpinIndex {0};
        ////char Spin() { ++m_nSpinIndex; if (m_nSpinIndex > 5) m_nSpinIndex = 0; return m_acSpin[m_nSpinIndex]; }
};

#endif // MINIMAX_H
