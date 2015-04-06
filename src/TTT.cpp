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

#include "TTT.h"

/**
  * Return a vector of valid game moves.
  *
  * Review and collect all valid moves for a player into a vector.  For
  * Tic-Tac-Toe, the nPlayer is ignored because valid moves are the same
  * between both players.
  *
  * \param nPlayer The player whose turn it is.
  *
  * \return A vector of valid moves.
  */

std::vector<GameMove> TTT::GenerateMoves(int nPlayer) const
{
    (void)nPlayer;

    std::vector<GameMove> vGameMoves {};

    for (int xxx = 0; xxx < m_knX; ++xxx)
    {
        for (int yyy = 0; yyy < m_knY; ++yyy)
        {
            if (m_acGrid[yyy][xxx] == m_kcClear)
                vGameMoves.emplace_back(0,0,xxx,yyy);
        }
    }

    return vGameMoves;
}
