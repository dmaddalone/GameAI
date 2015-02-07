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
 * \brief The Human class represents a human game player.
 *
 */

#ifndef HUMAN_H
#define HUMAN_H

#include <iostream>
#include "Player.h"
#include "Game.h"
#include "GameMove.h"

class Human: public Player
{
    public:
        Human(PlayerType ecPlayerType) : Player(ecPlayerType) {};
        ~Human() {};

        bool Move(Game &cGame);

        std::string TypeName() { return "Human"; }

    private:
};

#endif // HUMAN_H
