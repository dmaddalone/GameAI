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
 * \brief The ChessBaby class represents the minichess variant Baby Chess,
 * invented by Martin Gardner.  Played on a 5x5 board and allows all
 * normal chess moves.
 *
 */

#ifndef CHESSBABY_H
#define CHESSBABY_H

#include "ChessGame.h"

class ChessBaby : public ChessGame
{
    public:
        // Constructor
        ChessBaby(GameType ecGameType) : ChessGame(ecGameType, 5, 5, 12, true, true, true)
        {
            SetBoard();
        }
        // Destructor
        ~ChessBaby() {}

        // Clone the current game
        virtual std::unique_ptr<Game> Clone() const override { return std::unique_ptr<Game>(new ChessBaby(*this)); }
        // Return the title of the game
        virtual std::string Title() override { return "Baby Minichess"; }

        void SetBoard();
};

#endif // CHESSBABY_H
