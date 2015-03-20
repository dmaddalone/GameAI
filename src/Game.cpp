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

#include "Game.h"
#include "ConnectFour.h"
#include "TTT.h"
#include "Reversi.h"

/**
  * Make a game.
  *
  * Based on the type of game, create the game and return a unique pointer to it.
  *
  * \param ecGameType The type of game.
  *
  * \return A unique pointer to a game object.  Default is nullptr.
  */

std::unique_ptr<Game> Game::Make(GameType ecGameType)
{
    switch (ecGameType)
    {
        case GameType::TYPE_CONNECT_FOUR:
            return std::unique_ptr<Game>(new ConnectFour(ecGameType));
        case GameType::TYPE_TTT:
            return std::unique_ptr<Game>(new TTT(ecGameType));
        case GameType::TYPE_REVERSI:
            return std::unique_ptr<Game>(new Reversi(ecGameType));
        default:
            return nullptr;
    }
}
