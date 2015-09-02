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
 * \brief The ChessQuick class represents the minichess variant Quick Chess,
 * invented by Joseph Miccio.  Played on a 5x6 board and does not allow
 * double pawn, en passant moves, or castling.
 *
 */

#ifndef CHESSQUICK_H
#define CHESSQUICK_H

#include "ChessGame.h"

class ChessQuick : public ChessGame
{
    public:
        // Constructor
        ChessQuick(GameType ecGameType) : ChessGame(ecGameType, 5, 6, 12, true, true, true)
        {
            // Turn off castling, pawn double move, and en passant
            m_abCastlingAllowed[0] = false;
            m_abCastlingAllowed[1] = false;
            m_bDoublePawnMoveAllowed = false;
            m_bEnPassantAllowed = false;

            SetBoard();
        }
        // Destructor
        ~ChessQuick() {}

        // Clone the current game
        virtual std::unique_ptr<Game> Clone() const override { return std::unique_ptr<Game>(new ChessQuick(*this)); }
        // Return the title of the game
        virtual std::string Title() const override { return "Quick Minichess"; }

        void SetBoard();
};

#endif // CHESSQUICK_H
