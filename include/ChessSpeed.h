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
 * \brief The ChessSpeed class represents the minichess variant Speed Chess,
 * invented by  Mr. den Oude.  Played on a 5x6 board and does not allow
 * double pawn or en passant moves.
 *
 */

#ifndef CHESSSPEED_H
#define CHESSSPEED_H

#include "ChessGame.h"

class ChessSpeed : public ChessGame
{
    public:
        // Constructor
        explicit ChessSpeed(GameType ecGameType) : ChessGame(ecGameType, 5, 6, 12, true, true, true)
        {
            // Turn off pawn double move and en passant
            m_bDoublePawnMoveAllowed = false;
            m_bEnPassantAllowed = false;

            SetBoard();
        }
        // Destructor
        ~ChessSpeed() {}

        // Clone the current game
        virtual std::unique_ptr<Game> Clone() const override { return std::unique_ptr<Game>(new ChessSpeed(*this)); }

        // Return the title of the game
        virtual std::string Title() const override { return "Speed Minichess"; }

        void SetBoard();
};

#endif // CHESSSPEED_H
