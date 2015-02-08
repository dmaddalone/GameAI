#include "TTT.h"

void TTT::ClearBoard()
{
    for (int xxx = 0; xxx < m_kX; ++xxx)
    {
        for (int yyy = 0; yyy < m_kY; ++yyy)
        {
            m_acGrid[yyy][xxx] = m_kClear;
        }
    }
}

void TTT::Display(const bool bDisplayCoordinates) const
{
    if (bDisplayCoordinates)
    {
        std::cout << "   ";
        for (int xxx = 0; xxx < m_kX; ++xxx)
        {
            std::cout << xxx + 1 << "   ";
        }
        std::cout << std::endl << std::endl;;
    }

    for (int yyy = 0; yyy < m_kY; ++yyy)
    {
        if (bDisplayCoordinates)
        {
            std::cout << yyy + 1 << "  ";
        }

        for (int xxx = 0; xxx < m_kX; ++xxx)
        {
            if (xxx > 0)
                std::cout << " | ";

            //std::cout << m_anGrid[yyy][xxx] << "   ";
            std::cout << static_cast<char>(m_acGrid[yyy][xxx]);
        }

        if (yyy < m_kY - 1)
            std::cout << std::endl << "  -----------" << std::endl;
        else
            std::cout << std::endl << std::endl;
    }
}

void TTT::DisplayValidMoves() const
{
    std::vector<GameMove> vGameMoves = GenerateMoves();
    for (GameMove cGameMove : vGameMoves)
    {
        std::cout << cGameMove.ToX() + 1 << m_kDelimeter << cGameMove.ToY() + 1 << " ";
    }
    std::cout << std::endl;
}

std::vector<GameMove> TTT::GenerateMoves() const
{
    std::vector<GameMove> vGameMoves {};

    for (int xxx = 0; xxx < m_kX; ++xxx)
    {
        for (int yyy = 0; yyy < m_kY; ++yyy)
        {
            if (m_acGrid[yyy][xxx] == m_kClear)
                vGameMoves.emplace_back(0,0,xxx,yyy);
        }
    }

    return vGameMoves;
}


GameMove TTT::GetMove() const
{
    std::string sMove {};
    GameMove cGameMove;

    std::cin >> sMove;

    std::string sToken = sMove.substr(0,sMove.find(m_kDelimeter));
    int xxx = atoi(sToken.c_str());

    sToken = sMove.substr(sMove.find(m_kDelimeter) + 1,sMove.length() );
    int yyy = atoi(sToken.c_str());

    cGameMove.SetToX(xxx - 1);
    cGameMove.SetToY(yyy - 1);

    return cGameMove;
}

void TTT::AnnounceMove(const int nPlayer, const GameMove &cGameMove)
{
    std::cout << "Move number " << m_nNumberOfMoves + 1 << " -- Player " << nPlayer << " moves: " << cGameMove.ToX() + 1 << std::endl;;
}

int TTT::ApplyMove(const int nPlayer, GameMove &cGameMove)
{
    if ((nPlayer != m_kPlayer1) && (nPlayer != m_kPlayer2))
        return -1;

    if ((cGameMove.ToX() > m_kX - 1) || (cGameMove.ToX() < 0))
        return -1;

    if ((cGameMove.ToY() > m_kY - 1) || (cGameMove.ToY() < 0))
        return -1;

    //if (m_anGrid[cGameMove.ToY()][cGameMove.ToX()] != 0)
    if (m_acGrid[cGameMove.ToY()][cGameMove.ToX()] != m_kClear)
        return -1;

    //int y = FindBottom(cGameMove.ToX()); //TODO: remove

    //if (y == -1)
    //    return -1;

    //cGameMove.SetToY(y);

    //m_anGrid[cGameMove.ToY()][cGameMove.ToX()] = nPlayer;
    m_acGrid[cGameMove.ToY()][cGameMove.ToX()] = m_kTokens[nPlayer];
    ++m_nNumberOfMoves;

    return 1;
}

bool TTT::RetractMove(const int nPlayer, const GameMove &cGameMove)
{
    //m_anGrid[cGameMove.ToY()][cGameMove.ToX()] = 0;
    m_acGrid[cGameMove.ToY()][cGameMove.ToX()] = m_kClear;
    --m_nNumberOfMoves;

    return true;
}

void TTT::CountSequence(int nSequence, SequenceCounts &stSequenceCounts)
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

int TTT::EvaluateGameState(const int nPlayer)
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
/*
        return ( ((m_stMyCounts.nCount2 - m_stOpponentCounts.nCount2) * 10) +
                 ((m_stMyCounts.nCount3 - m_stOpponentCounts.nCount3) * 100) );
 */

        return ( (m_stMyCounts.nCount2 * 10) - (m_stOpponentCounts.nCount2 * 100) +
                 (m_stMyCounts.nCount3 * 1000) - (m_stOpponentCounts.nCount3 * 10000) );
    }

    return 0;
}

bool TTT::GameEnded()
{
    m_nWinner = 0;
    m_sWinBy.assign("nothing");

    if (CheckOrthogonal(m_kPlayer1, 3))
    {
        m_nWinner = m_kPlayer1;
        return true;
    }

    if (CheckOrthogonal(m_kPlayer2, 3))
    {
        m_nWinner = m_kPlayer2;
        return true;
    }

    if (CheckDiagonal(m_kPlayer1, 3))
    {
        m_nWinner = m_kPlayer1;
        return true;
    }

    if (CheckDiagonal(m_kPlayer2, 3))
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

bool TTT::CheckOrthogonal(const int nPlayer, int nConnect) //const
{
    bool bConnect = false;

    for (int yyy = 0; yyy < m_kY; ++yyy)
    {
        for (int xxx = 0; xxx < m_kX; ++xxx)
        {
            if (CheckHorizontal(nPlayer, yyy, xxx) == nConnect)
            {
                bConnect = true;
                if (nConnect == 3)
                    m_sWinBy.assign("Horizontal");
                break;
            }
        }

        if (bConnect)
            break;
    }

    if (!bConnect)
    {
        for (int yyy = 0; yyy < m_kY; ++yyy)
        {
            for (int xxx = 0; xxx < m_kX; ++xxx)
            {
                if (CheckVertical(nPlayer, yyy, xxx) == nConnect)
                {
                    bConnect = true;
                    if (nConnect == 3)
                        m_sWinBy.assign("Horizontal");
                    break;
                }
            }

            if (bConnect)
                break;
        }
    }

    return bConnect;
}

int TTT::CheckHorizontal(const int nPlayer, const int y, const int x) //const
{
    if (!ValidMove(y, x)) return 0;

    //if (m_anGrid[y][x] == nPlayer)
    if (m_acGrid[y][x] == m_kTokens[nPlayer])
        return (1 + CheckHorizontal(nPlayer, y, x+1));
    else
        return 0;
}

int TTT::CheckVertical(const int nPlayer, const int y, const int x) //const
{
    if (!ValidMove(y, x)) return 0;

    //if (m_anGrid[y][x] == nPlayer)
    if (m_acGrid[y][x] == m_kTokens[nPlayer])
        return (1 + CheckVertical(nPlayer, y+1, x));
    else
        return 0;
}

bool TTT::CheckDiagonal(const int nPlayer, int nConnect) //const
{
    bool bConnect = false;

    for (int yyy = 0; yyy < m_kY; ++yyy)
    {
        for (int xxx = 0; xxx < m_kX; ++xxx)
        {
            if (CheckDiagonalUpperLeftLowerRight(nPlayer, yyy, xxx) == nConnect)
            {
                bConnect = true;
                if (nConnect == 3)
                    m_sWinBy.assign("DiagonalULLR");
                break;
            }
        }

        if (bConnect)
            break;
    }

    if (!bConnect)
    {
        for (int yyy = 0; yyy < m_kY; ++yyy)
        {
            for (int xxx = m_kX - 1; xxx >= 0; --xxx)
            {
                if (CheckDiagonalUpperRightLowerLeft(nPlayer, yyy, xxx) == nConnect)
                {
                    bConnect = true;
                    if (nConnect == 3)
                        m_sWinBy.assign("DiagonalURLL");
                    break;
                }
            }

            if (bConnect)
                break;
        }
    }

    return bConnect;
}

int TTT::CheckDiagonalUpperLeftLowerRight(const int nPlayer, const int y, const int x) //const
{
    if (!ValidMove(y, x)) return 0;

    //if (m_anGrid[y][x] == nPlayer)
    if (m_acGrid[y][x] == m_kTokens[nPlayer])
        return (1 + CheckDiagonalUpperLeftLowerRight(nPlayer, y+1, x+1));
    else
        return 0;
}

int TTT::CheckDiagonalUpperRightLowerLeft(const int nPlayer, const int y, const int x) //const
{
    if (!ValidMove(y, x)) return 0;

    //if (m_anGrid[y][x] == nPlayer)
    if (m_acGrid[y][x] == m_kTokens[nPlayer])
        return (1 + CheckDiagonalUpperRightLowerLeft(nPlayer, y+1, x-1));
    else
        return 0;
}

bool TTT::ValidMove(const int y, const int x) const
{
    if ((y >= m_kY) || (y < 0))
        return false;

    if ((x >= m_kX) || (x < 0))
        return false;

    return true;
}

int TTT::PreferredMove(const GameMove &cGameMove) const
{
    //return std::abs(cGameMove.ToX() - 3);
    return 0;
}
