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
 * \brief TODO: The Simulation class gathers Strategies at one table and runs
 * simulations.
 *
 */

#ifndef TTT_H
#define TTT_H

#include <iostream>
#include "Game.h"
#include "LinearGame.h"

class TTT : public LinearGame
{
    public:
        TTT(GameType ecGameType) : LinearGame(ecGameType, 3, 3, ' ', 'X', 'O', 3, true, true, true) {}
        ~TTT() {}

        virtual std::vector<GameMove> GenerateMoves(int nPlayer) const override;

        virtual std::unique_ptr<Game> Clone() const override { return std::unique_ptr<Game>(new TTT(*this)); }

        virtual std::string Title() override { return "Tic-Tac-Toe"; }

    private:
        //const int m_kWin {3};
};

#endif // TTT_H
