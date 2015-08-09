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
 * \brief The Chess class represents an orthodox chess game.
 *
 */

#ifndef CHESS_H
#define CHESS_H

#include "ChessGame.h"

class Chess : public ChessGame
{
    public:
        // Constructor
        Chess(GameType ecGameType) : ChessGame(ecGameType, 8, 8, 12, true, true, true)
        {
            SetBoard();
        }
        // Destructor
        ~Chess() {}

        // Clone the current game
        virtual std::unique_ptr<Game> Clone() const override { return std::unique_ptr<Game>(new Chess(*this)); }
        // Return the title of the game
        virtual std::string Title() override { return "Chess"; }
        // Set up the board
        void SetBoard();
};
#endif // CHESS_H
