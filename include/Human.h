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

#include "Player.h"
#include "Game.h"

class Human: public Player
{
    public:
        // Construct a human player
        Human(PlayerType ecPlayerType) : Player(ecPlayerType) {}

        // Initializer
        virtual void Initialize(std::string sHost, int nPort, bool &bSwap) override { (void)sHost; (void)nPort; (void)bSwap; }

        // Generate the next game move
        virtual bool Move(Game &cGame) override;

        // Player actions at end of game
        virtual bool Finish(Game &cGame) override { (void)cGame; return true; }

        // Announce the type pf player
        std::string TypeName() const override { return "Human"; }
};

#endif // HUMAN_H
