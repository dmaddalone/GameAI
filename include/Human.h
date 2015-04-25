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
 * \brief The Human class represents a human game player.
 *
 */

#ifndef HUMAN_H
#define HUMAN_H

#include <iostream>
#include "Player.h"
#include "Game.h"
#include "GameMove.h"
#include "Logger.h"

class Human: public Player
{
    public:
        // Construct a human player
        Human(PlayerType ecPlayerType) : Player(ecPlayerType) {}

        // Destructor
        ~Human() {}

        // Initializer
        virtual void Initialize(bool &bSwap) override { (void)bSwap; }

        // Generate the next game move
        virtual bool Move(Game &cGame) override;

        // Player sends last move to networked player - not used in Human
        virtual bool SendLastMove(Game &cGame) override { (void)cGame; return false; }

        // Player receives last move from networked player - not used in Human
        virtual bool RecvLastMove(Game &cGame) override { (void)cGame; return false; }

        // Announce the type pf player
        std::string TypeName() { return "Human"; }
};

#endif // HUMAN_H
