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

#include "Player.h"
#include "Human.h"
#include "Minimax.h"
#include "Server.h"
#include "Client.h"

// Initial static int to zero; used to generate unique identifier numbers for players
int Player::m_nPlayerCount {0};

/**
  * Make a player.
  *
  * Based on the type of player, create the player and return a unique pointer to it.
  *
  * \param ecPlayerType The type of player.
  *
  * \return A unique pointer to a player object.  Default is nullptr.
  */

std::unique_ptr<Player> Player::MakePlayer(PlayerType ecPlayerType)
{
    ++m_nPlayerCount;

    switch (ecPlayerType)
    {
        case PlayerType::TYPE_HUMAN:
            return std::unique_ptr<Player>(new Human(ecPlayerType));
        case PlayerType::TYPE_MINIMAX:
            return std::unique_ptr<Player>(new Minimax(ecPlayerType));
        case PlayerType::TYPE_SERVER:
            return std::unique_ptr<Player>(new Server(ecPlayerType, 60000));
        case PlayerType::TYPE_CLIENT:
            return std::unique_ptr<Player>(new Client(ecPlayerType, "127.0.0.1", 60000));
        default:
            return nullptr;
    }
}
