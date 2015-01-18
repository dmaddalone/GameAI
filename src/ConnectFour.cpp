#include "ConnectFour.h"

void ConnectFour::Display(const bool bDisplayCoordinates) const
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
            std::cout << m_anGrid[yyy][xxx] << "   ";
        }

        std::cout << std::endl << std::endl;
    }
}

void ConnectFour::DisplayValidMoves() const
{
    std::vector<int> vMoves = GenerateMoves();
    for (int iii : vMoves)
    {
        std::cout << iii + 1 << " ";
    }
    std::cout << std::endl;
}

int ConnectFour::ApplyMove(const int nPlayer, const int x)
{
    int nAdjustedX = x - 1;

    if ((nPlayer != m_kPlayer1) && (nPlayer != m_kPlayer2))
        return -1;

    if ((nAdjustedX > m_kX - 1) || (nAdjustedX < 0))
        return -1;

    if (m_anGrid[0][nAdjustedX] != 0)
        return -1;

    int y = FindBottom(nAdjustedX);

    if (y == -1)
        return -1;

    m_anGrid[y][nAdjustedX] = nPlayer;
    ++m_nNumberOfMoves;

    return y;
}

bool ConnectFour::RetractMove(const int y, const int x)
{
    m_anGrid[y][x] = 0;
    --m_nNumberOfMoves;
    return true;
}

std::vector<int> ConnectFour::GenerateMoves() const
{
    std::vector<int> vMoves {};

    for (int xxx = 0; xxx < m_kX; ++xxx)
    {
        if (FindBottom(xxx) >= 0)
            vMoves.push_back(xxx);
    }

    return vMoves;
}

int ConnectFour::EvaluateGameState(const int nPlayer) const
{
    if (nPlayer == m_nWinner)
        return 1;

    if (m_nWinner == 0)
        return 0;

    return -1;
}

int ConnectFour::FindBottom(const int x) const
{
    for (int yyy = m_kY - 1; yyy >= 0; --yyy)
    {
        if (m_anGrid[yyy][x] == 0)
            return yyy;
    }

    return -1;
}

bool ConnectFour::GameEnded()
{
    m_nWinner = 0;
    m_sWinBy.assign("Horizontal");

    if (CheckOrthogonal(m_kPlayer1))
    {
        m_nWinner = m_kPlayer1;
        return true;
    }

    if (CheckOrthogonal(m_kPlayer2))
    {
        m_nWinner = m_kPlayer2;
        return true;
    }

    if (CheckDiagonal(m_kPlayer1))
    {
        m_nWinner = m_kPlayer1;
        return true;
    }

    if (CheckDiagonal(m_kPlayer2))
    {
        m_nWinner = m_kPlayer2;
        return true;
    }

    std::vector<int> vMoves = GenerateMoves();
    if (vMoves.empty())
    {
    //std::cout << "Game ended - no more moves" << std::endl; // DEBUG
        return true;
    }

    return false;
}

bool ConnectFour::CheckOrthogonal(const int nPlayer) //const
{
    bool bConnectFour = false;

    for (int yyy = 0; yyy < m_kY; ++yyy)
    {
        for (int xxx = 0; xxx < m_kX; ++xxx)
        {
            if (CheckHorizontal(nPlayer, yyy, xxx) == 4)
            {
              //      std::cout << "Game ended - Horizontal Player " << nPlayer << std::endl; // DEBUG
                bConnectFour = true;
                m_sWinBy.assign("Horizontal");
                //std::string sHorizontal("Horizontal");
                //m_sWinBy = sHorizontal;
                //m_nWinBy = 1;
                break;
            }
        }

        if (bConnectFour)
            break;
    }

    if (!bConnectFour)
    {
        for (int yyy = 0; yyy < m_kY; ++yyy)
        {
            for (int xxx = 0; xxx < m_kX; ++xxx)
            {
                if (CheckVertical(nPlayer, yyy, xxx) == 4)
                {
            //    std::cout << "Game ended - Vertical Player " << nPlayer << std::endl; // DEBUG
                    bConnectFour = true;
                    m_sWinBy.assign("Horizontal");
                    //m_nWinBy = 2;
                    break;
                }
            }

            if (bConnectFour)
                break;
        }
    }

    return bConnectFour;
}

int ConnectFour::CheckHorizontal(const int nPlayer, const int y, const int x) //const
{
    if (!ValidMove(y, x)) return 0;

    if (m_anGrid[y][x] == nPlayer)
        return (1 + CheckHorizontal(nPlayer, y, x+1));
    else
        return 0;
}

int ConnectFour::CheckVertical(const int nPlayer, const int y, const int x) //const
{
    if (!ValidMove(y, x)) return 0;

    if (m_anGrid[y][x] == nPlayer)
        return (1 + CheckVertical(nPlayer, y+1, x));
    else
        return 0;
}

bool ConnectFour::CheckDiagonal(const int nPlayer) //const
{
    bool bConnectFour = false;

    for (int yyy = 0; yyy < m_kY; ++yyy)
    {
        for (int xxx = 0; xxx < m_kX; ++xxx)
        {
            if (CheckDiagonalUpperLeftLowerRight(nPlayer, yyy, xxx) == 4)
            {
                bConnectFour = true;
                m_sWinBy.assign("DiagonalULLR");
                break;
            }
        }

        if (bConnectFour)
            break;
    }

    if (!bConnectFour)
    {
        for (int yyy = 0; yyy < m_kY; ++yyy)
        {
            for (int xxx = m_kX - 1; xxx >= 0; --xxx)
            {
                if (CheckDiagonalUpperRightLowerLeft(nPlayer, yyy, xxx) == 4)
                {
                    bConnectFour = true;
                    m_sWinBy.assign("DiagonalURLL");
                    break;
                }
            }

            if (bConnectFour)
                break;
        }
    }

    return bConnectFour;
}

int ConnectFour::CheckDiagonalUpperLeftLowerRight(const int nPlayer, const int y, const int x) //const
{
    if (!ValidMove(y, x)) return 0;

    if (m_anGrid[y][x] == nPlayer)
        return (1 + CheckDiagonalUpperLeftLowerRight(nPlayer, y+1, x+1));
    else
        return 0;
}

int ConnectFour::CheckDiagonalUpperRightLowerLeft(const int nPlayer, const int y, const int x) //const
{
    if (!ValidMove(y, x)) return 0;

    if (m_anGrid[y][x] == nPlayer)
        return (1 + CheckDiagonalUpperRightLowerLeft(nPlayer, y+1, x-1));
    else
        return 0;
}

bool ConnectFour::ValidMove(const int y, const int x) const
{
    if ((y >= m_kY) || (y < 0))
        return false;

    if ((x >= m_kX) || (x < 0))
        return false;

    return true;
}

int ConnectFour::PreferredMove(const int nMove) const
{

    return std::abs(nMove - 4);
}
