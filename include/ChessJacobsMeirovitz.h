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
 * \brief The ChessJacosMeirovitz class represents the minichess variant
 * invented by Paul Jacobs and Marco Meirovitz.  Played on a 5x5 board and
 * allows all normal chess moves.
 *
 */

#ifndef CHESSJACOBSMEIROVITZ_H
#define CHESSJACOBSMEIROVITZ_H

#include "ChessGame.h"

class ChessJacobsMeirovitz : public ChessGame
{
    public:
        // Constructor
        ChessJacobsMeirovitz(GameType ecGameType) : ChessGame(ecGameType, 5, 5, 12, true, true, true)
        {
            SetBoard();
        }
        // Destructor
        ~ChessJacobsMeirovitz() {}

        // Clone the current game
        virtual std::unique_ptr<Game> Clone() const override { return std::unique_ptr<Game>(new ChessJacobsMeirovitz(*this)); }
        // Return the title of the game
        virtual std::string Title() const override { return "Jacobs-Meirovitz Minichess"; }

        void SetBoard();
};

#endif // CHESSJACOBSMEIROVITZ_H
