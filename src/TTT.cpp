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
