#include "TTT.h"

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

/*
bool TTT::GameEnded(int nPlayer)
{
    m_nWinner = 0;
    m_sWinBy.assign("nothing");

    if (CheckOrthogonal(m_knPlayer1, m_kWin))
    {
        m_nWinner = m_knPlayer1;
        return true;
    }

    if (CheckOrthogonal(m_knPlayer2, m_kWin))
    {
        m_nWinner = m_knPlayer2;
        return true;
    }

    if (CheckDiagonal(m_knPlayer1, m_kWin))
    {
        m_nWinner = m_knPlayer1;
        return true;
    }

    if (CheckDiagonal(m_knPlayer2, m_kWin))
    {
        m_nWinner = m_knPlayer2;
        return true;
    }

    std::vector<GameMove> vGameMoves = GenerateMoves(nPlayer);
    if (vGameMoves.empty())
    {
        return true;
    }

    return false;
}
*/


