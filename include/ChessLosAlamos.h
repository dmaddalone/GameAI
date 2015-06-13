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
 * \brief The ChessLosAlamos class represents the minichess variant Los Alamos
 * Chess, invented by Paul Stein and Mark Wells.  Played on a 6x6 board and
 * does not allow double pawn, en passant moves, or castling.
 *
 */

#ifndef CHESSLOSALAMOS_H
#define CHESSLOSALAMOS_H

#include "ChessGame.h"

class ChessLosAlamos : public ChessGame
{
    public:
        // Constructor
        ChessLosAlamos(GameType ecGameType) : ChessGame(ecGameType, 6, 6, true, true, true)
        {
            m_sTitle.assign("Los Alamos Minichess");
            SetBoard();
        }
        // Destructor
        ~ChessLosAlamos() {}

        // Clone the current game
        virtual std::unique_ptr<Game> Clone() const override { return std::unique_ptr<Game>(new ChessLosAlamos(*this)); }
        // Return the title of the game
        virtual std::string Title() override { return m_sTitle + ChessGame::Title(); }

        void SetBoard();
};

#endif // CHESSLOSALAMOS_H
