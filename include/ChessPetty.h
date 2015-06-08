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
 * \brief The ChessPetty class represents the minichess variant Petty Chess,
 * invented by B. Walker Watson.  Played on a 5x6 board and does not allow
 * double pawn, en passant moves, or castling.
 *
 */

#ifndef CHESSPETTY_H
#define CHESSPETTY_H

#include "ChessGame.h"

class ChessPetty : public ChessGame
{
    public:
        // Constructor
        ChessPetty(GameType ecGameType) : ChessGame(ecGameType, 5, 6, true, true, true)
        {
            m_abCastlingAllowed[0] = false;
            m_abCastlingAllowed[1] = false;
            m_bDoublePawnMoveAllowed = false;
            m_bEnPassantAllowed = false;

            SetBoard();
        }
        // Destructor
        ~ChessPetty() {}

        // Clone the current game
        virtual std::unique_ptr<Game> Clone() const override { return std::unique_ptr<Game>(new ChessPetty(*this)); }
        // Return the title of the game
        virtual std::string Title() override { return "Petty Minichess" + ChessGame::Title(); }

        void SetBoard();
};

#endif // CHESSPETTY_H
