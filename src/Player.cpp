/*
    Copyright 2014 Dom Maddalone

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

int Player::m_nPlayerCount {0};

std::unique_ptr<Player> Player::MakePlayer(PlayerType ecPlayerType)
{
    ++m_nPlayerCount;

    switch (ecPlayerType)
    {
        case PlayerType::TYPE_HUMAN:
            return std::unique_ptr<Player>(new Human(ecPlayerType));
        case PlayerType::TYPE_MINIMAX:
            return std::unique_ptr<Player>(new Minimax(ecPlayerType));
        default:
            return nullptr;
    }
}
