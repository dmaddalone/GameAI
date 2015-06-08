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
 * \brief The ChessAttack class represents the minichess variant Chess Attack,
 * invented by Joseph Miccio.  Played on a 5x6 board and allows all
 * normal chess moves.
 *
 */

#ifndef CHESSATTACK_H
#define CHESSATTACK_H

#include "ChessGame.h"

class ChessAttack : public ChessGame
{
    public:
        // Constructor
        ChessAttack(GameType ecGameType) : ChessGame(ecGameType, 5, 6, true, true, true)
        {
            SetBoard();
        }
        // Destructor
        ~ChessAttack() {}

        // Clone the current game
        virtual std::unique_ptr<Game> Clone() const override { return std::unique_ptr<Game>(new ChessAttack(*this)); }
        // Return the title of the game
        virtual std::string Title() override { return "Chess Attack Minichess" + ChessGame::Title(); }

        void SetBoard();
};

#endif // CHESSATTACK_H
