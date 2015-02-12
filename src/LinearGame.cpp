#include "LinearGame.h"

/*
LinearGame::LinearGame()
{
    //ctor
}

LinearGame::~LinearGame()
{
    //dtor
}
*/

void LinearGame::ClearBoard()
{
    for (int xxx = 0; xxx < m_kX; ++xxx)
    {
        for (int yyy = 0; yyy < m_kY; ++yyy)
        {
            m_acGrid[yyy][xxx] = m_kClear;
        }
    }
}

int LinearGame::EvaluateGameState(const int nPlayer) //TODO: may be game specific
{
    if (m_nWinner == nPlayer)
        return 1000000;

    if (m_nWinner == (1 - nPlayer + 2))
        return -1000000;

    m_stMyCounts = {};
    m_stOpponentCounts = {};

    if (m_nWinner == 0)
    {
        for (int yyy = 0; yyy < m_kY; ++yyy)
        {
            for (int xxx = 0; xxx < m_kX; ++xxx)
            {
                CountSequence(CheckHorizontal(nPlayer, yyy, xxx), m_stMyCounts);
                CountSequence(CheckHorizontal(1 - nPlayer + 2, yyy, xxx), m_stOpponentCounts);
                CountSequence(CheckVertical(nPlayer, yyy, xxx), m_stMyCounts);
                CountSequence(CheckVertical(1 - nPlayer + 2, yyy, xxx), m_stOpponentCounts);
                CountSequence(CheckDiagonalUpperLeftLowerRight(nPlayer, yyy, xxx), m_stMyCounts);
                CountSequence(CheckDiagonalUpperLeftLowerRight(1 - nPlayer + 2, yyy, xxx), m_stOpponentCounts);
                CountSequence(CheckDiagonalUpperRightLowerLeft(nPlayer, yyy, xxx), m_stMyCounts);
                CountSequence(CheckDiagonalUpperRightLowerLeft(1 - nPlayer + 2, yyy, xxx), m_stOpponentCounts);
            }
        }

        return ( (m_stMyCounts.nCount2 * 10) - (m_stOpponentCounts.nCount2 * 100) +
                 (m_stMyCounts.nCount3 * 1000) - (m_stOpponentCounts.nCount3 * 10000) );
    }

    return 0;
}

void LinearGame::CountSequence(int nSequence, SequenceCounts &stSequenceCounts)
{
    switch (nSequence)
    {
        case 0:
            break;
        case 1:
            ++stSequenceCounts.nCount1;
            break;
        case 2:
            ++stSequenceCounts.nCount2;
            break;
        case 3:
            ++stSequenceCounts.nCount3;
            break;
        default:
            break; // TODO - should not get here
    }
    return;
}

bool LinearGame::CheckOrthogonal(const int nPlayer, int nConnect) //const
{
    bool bWin = false;

    for (int yyy = 0; yyy < m_kY; ++yyy)
    {
        for (int xxx = 0; xxx < m_kX; ++xxx)
        {
            if (CheckHorizontal(nPlayer, yyy, xxx) == nConnect)
            {
                bWin = true;
                if (nConnect == 3)
                    m_sWinBy.assign("Horizontal");
                break;
            }
        }

        if (bWin)
            break;
    }

    if (!bWin)
    {
        for (int yyy = 0; yyy < m_kY; ++yyy)
        {
            for (int xxx = 0; xxx < m_kX; ++xxx)
            {
                if (CheckVertical(nPlayer, yyy, xxx) == nConnect)
                {
                    bWin = true;
                    if (nConnect == 3)
                        m_sWinBy.assign("Horizontal");
                    break;
                }
            }

            if (bWin)
                break;
        }
    }

    return bWin;
}

int LinearGame::CheckHorizontal(const int nPlayer, const int y, const int x) //const
{
    if (!ValidMove(y, x)) return 0;

    if (m_acGrid[y][x] == m_kTokens[nPlayer])
        return (1 + CheckHorizontal(nPlayer, y, x+1));
    else
        return 0;
}

int LinearGame::CheckVertical(const int nPlayer, const int y, const int x) //const
{
    if (!ValidMove(y, x)) return 0;

    if (m_acGrid[y][x] == m_kTokens[nPlayer])
        return (1 + CheckVertical(nPlayer, y+1, x));
    else
        return 0;
}

bool LinearGame::CheckDiagonal(const int nPlayer, int nConnect) //const
{
    bool bWin = false;

    for (int yyy = 0; yyy < m_kY; ++yyy)
    {
        for (int xxx = 0; xxx < m_kX; ++xxx)
        {
            if (CheckDiagonalUpperLeftLowerRight(nPlayer, yyy, xxx) == nConnect)
            {
                bWin = true;
                if (nConnect == 3)
                    m_sWinBy.assign("DiagonalULLR");
                break;
            }
        }

        if (bWin)
            break;
    }

    if (!bWin)
    {
        for (int yyy = 0; yyy < m_kY; ++yyy)
        {
            for (int xxx = m_kX - 1; xxx >= 0; --xxx)
            {
                if (CheckDiagonalUpperRightLowerLeft(nPlayer, yyy, xxx) == nConnect)
                {
                    bWin = true;
                    if (bWin == 3)
                        m_sWinBy.assign("DiagonalURLL");
                    break;
                }
            }

            if (bWin)
                break;
        }
    }

    return bWin;
}

int LinearGame::CheckDiagonalUpperLeftLowerRight(const int nPlayer, const int y, const int x) //const
{
    if (!ValidMove(y, x)) return 0;

    if (m_acGrid[y][x] == m_kTokens[nPlayer])
        return (1 + CheckDiagonalUpperLeftLowerRight(nPlayer, y+1, x+1));
    else
        return 0;
}

int LinearGame::CheckDiagonalUpperRightLowerLeft(const int nPlayer, const int y, const int x) //const
{
    if (!ValidMove(y, x)) return 0;

    if (m_acGrid[y][x] == m_kTokens[nPlayer])
        return (1 + CheckDiagonalUpperRightLowerLeft(nPlayer, y+1, x-1));
    else
        return 0;
}

bool LinearGame::ValidMove(const int y, const int x) const
{
    if ((y >= m_kY) || (y < 0))
        return false;

    if ((x >= m_kX) || (x < 0))
        return false;

    return true;
}
