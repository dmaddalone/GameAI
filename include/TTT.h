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
 * \brief The TTT class represents a Tic-Tac-Toe game.
 *
 */

#ifndef TTT_H
#define TTT_H

#include "LinearGame.h"

class TTT : public LinearGame
{
    public:
        // Constructor
        TTT(GameType ecGameType) : LinearGame(ecGameType, 3, 3, 'X', 'O', 3, true, true, true, true) {}
        // Destructor
        ~TTT() {}

        // Generate a vector of valis moves
        virtual std::vector<GameMove> GenerateMoves(int nPlayer) const override;

        // Clone the current game
        virtual std::unique_ptr<Game> Clone() const override { return std::unique_ptr<Game>(new TTT(*this)); }

        // Return the title of the game
        virtual std::string Title() override { return "Tic-Tac-Toe"; }

        // Return the description of the game
        virtual std::string Description() override { return ""; }
};

#endif // TTT_H
