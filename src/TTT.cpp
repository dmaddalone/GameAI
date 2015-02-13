#include "TTT.h"

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
    std::cout << "Move number " << m_nNumberOfMoves + 1 << " -- Player " << nPlayer << " moves: " << cGameMove.ToX() + 1 << "," << cGameMove.ToY() + 1 << std::endl;;
}

int TTT::ApplyMove(const int nPlayer, GameMove &cGameMove)
{
    if ((nPlayer != m_kPlayer1) && (nPlayer != m_kPlayer2))
        return -1;

    if ((cGameMove.ToX() > m_kX - 1) || (cGameMove.ToX() < 0))
        return -1;

    if ((cGameMove.ToY() > m_kY - 1) || (cGameMove.ToY() < 0))
        return -1;

    if (m_acGrid[cGameMove.ToY()][cGameMove.ToX()] != m_kClear)
        return -1;

    m_acGrid[cGameMove.ToY()][cGameMove.ToX()] = m_acTokens[nPlayer];
    ++m_nNumberOfMoves;

    return 1;
}

bool TTT::RetractMove(const int nPlayer, const GameMove &cGameMove)
{
    m_acGrid[cGameMove.ToY()][cGameMove.ToX()] = m_kClear;
    --m_nNumberOfMoves;

    return true;
}

bool TTT::GameEnded()
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

int TTT::PreferredMove(const GameMove &cGameMove) const
{
    //return std::abs(cGameMove.ToX() - 3);
    return 0;
}
