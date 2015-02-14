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
#include "LinearGame.h"

class ConnectFour : public LinearGame
{
    public:
        ConnectFour(GameType ecGameType) : LinearGame(ecGameType, 7, 6, '0', '1', '2', false) {}
        ~ConnectFour() {}

        virtual GameMove GetMove(int nPlayer, int nOpponent) const override;

        virtual int  ApplyMove(int nPlayer, GameMove &cGameMove) override;

        virtual int  PreferredMove(const GameMove &cGameMove) const override;
        virtual bool GameEnded(int nPlayer, int nOpponent) override;

        virtual std::vector<GameMove> GenerateMoves(int nPlayer, int nOpponent) const override;

        virtual std::string Title() override { return "ConnectFour"; }

    private:
        int  FindBottom(int x) const;

        const int m_kWin {4};
};

#endif // CONNECTFOUR_H
