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

#include "ConnectFour.h"

std::vector<GameMove> ConnectFour::GenerateMoves(int nPlayer) const
{
    (void)nPlayer;

    std::vector<GameMove> vGameMoves {};

    for (int xxx = 0; xxx < m_knX; ++xxx)
    {
        if (FindBottom(xxx) >= 0)
        {
            vGameMoves.emplace_back(-1,-1,xxx,-1,false);
        }
    }

    return vGameMoves;
}

GameMove ConnectFour::GetMove(int nPlayer) const
{
    (void)nPlayer;

    char     cMove {};
    GameMove cGameMove;

    std::cin >> cMove;

    cGameMove.SetToX(cMove - m_kcXCoordinate);

    return cGameMove;
}

int ConnectFour::FindBottom(int x) const
{
    for (int yyy = m_knY - 1; yyy >= 0; --yyy)
    {
        if (m_acGrid[yyy][x] == m_kcClear)
            return yyy;
    }

    return -1;
}

int ConnectFour::PreferredMove(const GameMove &cGameMove) const
{
    return std::abs(cGameMove.ToX() - 3);
}
