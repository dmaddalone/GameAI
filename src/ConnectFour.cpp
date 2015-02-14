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

std::vector<GameMove> ConnectFour::GenerateMoves() const
{
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


GameMove ConnectFour::GetMove() const
{
    int nMove {};
    GameMove cGameMove;

    std::cin >> nMove;
    cGameMove.SetToX(nMove - 1);

    return cGameMove;
}

int ConnectFour::ApplyMove(const int nPlayer, GameMove &cGameMove)
{
    if ((nPlayer != m_kPlayer1) && (nPlayer != m_kPlayer2))
        return -1;

    if ((cGameMove.ToX() > m_knX - 1) || (cGameMove.ToX() < 0))
        return -1;

    if (m_acGrid[0][cGameMove.ToX()] != m_kcClear)
        return -1;

    int y = FindBottom(cGameMove.ToX());

    if (y == -1)
        return -1;

    cGameMove.SetToY(y);

    m_acGrid[cGameMove.ToY()][cGameMove.ToX()] = m_acTokens[nPlayer];
    ++m_nNumberOfMoves;
    //UpdatePlayerTurn();

    return y;
}

int ConnectFour::FindBottom(const int x) const
{
    for (int yyy = m_knY - 1; yyy >= 0; --yyy)
    {
        if (m_acGrid[yyy][x] == m_kcClear)
            return yyy;
    }

    return -1;
}

bool ConnectFour::GameEnded()
{
    m_nWinner = 0;
    m_sWinBy.assign("nothing");

    if (CheckOrthogonal(m_kPlayer1, m_kWin))
    {
        m_nWinner = m_kPlayer1;
        return true;
    }

    if (CheckOrthogonal(m_kPlayer2, m_kWin))
    {
        m_nWinner = m_kPlayer2;
        return true;
    }

    if (CheckDiagonal(m_kPlayer1, m_kWin))
    {
        m_nWinner = m_kPlayer1;
        return true;
    }

    if (CheckDiagonal(m_kPlayer2, m_kWin))
    {
        m_nWinner = m_kPlayer2;
        return true;
    }

    std::vector<GameMove> vGameMoves = GenerateMoves();
    if (vGameMoves.empty())
    {
        return true;
    }

    return false;
}

int ConnectFour::PreferredMove(const GameMove &cGameMove) const
{
    return std::abs(cGameMove.ToX() - 3);
}
