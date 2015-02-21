#include "Reversi.h"

void Reversi::SetBoard()
{
    m_acGrid[(m_knY / 2) - 1][(m_knX / 2) - 1] = m_acTokens[2];
    m_acGrid[(m_knY / 2) - 1][(m_knX / 2)]     = m_acTokens[1];
    m_acGrid[(m_knY / 2)][(m_knX / 2) - 1]     = m_acTokens[1];
    m_acGrid[(m_knY / 2)][(m_knX / 2)]         = m_acTokens[2];
}

std::vector<GameMove> Reversi::GenerateMoves(int nPlayer) const
{
    std::vector<GameMove> vGameMoves {};

    for (int xxx = 0; xxx < m_knX; ++xxx)
    {
        for (int yyy = 0; yyy < m_knY; ++yyy)
        {
             if (m_acGrid[yyy][xxx] == m_kcClear)
            {
                if (Contiguous(xxx, yyy, nPlayer)) // Check valid move
                {
                    vGameMoves.emplace_back(0,0,xxx,yyy);
                }
            }
        }
    }

    return vGameMoves;
}

bool Reversi::ApplyMove(int nPlayer, GameMove &cGameMove)
{
    bool bValidMove = false;
    std::vector<GameMove> vGameMoves = GenerateMoves(nPlayer);

    if (!LinearGame::ApplyMove(nPlayer, cGameMove))
        return false;

    for (GameMove &cValidGameMove : vGameMoves)
    {
        if (cValidGameMove.SameTo(cGameMove))
        {
            Flip(nPlayer, cGameMove);
            bValidMove = true;
            break;
        }
    }

    return bValidMove;
}

void Reversi::Flip(int nPlayer, const GameMove &cGameMove)
{
    FlipUp(cGameMove.ToX(), cGameMove.ToY(), nPlayer);
    FlipUpRight(cGameMove.ToX(), cGameMove.ToY(), nPlayer);
    FlipRight(cGameMove.ToX(), cGameMove.ToY(), nPlayer);
    FlipDownRight(cGameMove.ToX(), cGameMove.ToY(), nPlayer);
    FlipDown(cGameMove.ToX(), cGameMove.ToY(), nPlayer);
    FlipDownLeft(cGameMove.ToX(), cGameMove.ToY(), nPlayer);
    FlipLeft(cGameMove.ToX(), cGameMove.ToY(), nPlayer);
    FlipUpLeft(cGameMove.ToX(), cGameMove.ToY(), nPlayer);

    return;
}

void Reversi::FlipUp(int nX, int nY, int nPlayer)
{
    if (ContiguousUp(nX, nY, nPlayer))
    {
        while (true)
        {
            --nY;

            if (m_acGrid[nY][nX] != m_acTokens[nPlayer])
                m_acGrid[nY][nX] = m_acTokens[nPlayer];
            else
                break;
        }
    }
}

void Reversi::FlipUpRight(int nX, int nY, int nPlayer)
{
    if (ContiguousUpRight(nX, nY, nPlayer))
    {
        while (true)
        {
            ++nX;
            --nY;

            if (m_acGrid[nY][nX] != m_acTokens[nPlayer])
                m_acGrid[nY][nX] = m_acTokens[nPlayer];
            else
                break;
        }
    }
}

void Reversi::FlipRight(int nX, int nY, int nPlayer)
{
    if (ContiguousRight(nX, nY, nPlayer))
    {
        while (true)
        {
            ++nX;

            if (m_acGrid[nY][nX] != m_acTokens[nPlayer])
                m_acGrid[nY][nX] = m_acTokens[nPlayer];
            else
                break;
        }
    }
}

void Reversi::FlipDownRight(int nX, int nY, int nPlayer)
{
    if (ContiguousDownRight(nX, nY, nPlayer))
    {
        while (true)
        {
            ++nX;
            ++nY;

            if (m_acGrid[nY][nX] != m_acTokens[nPlayer])
                m_acGrid[nY][nX] = m_acTokens[nPlayer];
            else
                break;
        }
    }
}

void Reversi::FlipDown(int nX, int nY, int nPlayer)
{
    if (ContiguousDown(nX, nY, nPlayer))
    {
        while (true)
        {
            ++nY;

            if (m_acGrid[nY][nX] != m_acTokens[nPlayer])
                m_acGrid[nY][nX] = m_acTokens[nPlayer];
            else
                break;
        }
    }
}

void Reversi::FlipDownLeft(int nX, int nY, int nPlayer)
{
    if (ContiguousDownLeft(nX, nY, nPlayer))
    {
        while (true)
        {
            --nX;
            ++nY;

            if (m_acGrid[nY][nX] != m_acTokens[nPlayer])
                m_acGrid[nY][nX] = m_acTokens[nPlayer];
            else
                break;
        }
    }
}

void Reversi::FlipLeft(int nX, int nY, int nPlayer)
{
    if (ContiguousLeft(nX, nY, nPlayer))
    {
        while (true)
        {
            --nX;

            if (m_acGrid[nY][nX] != m_acTokens[nPlayer])
                m_acGrid[nY][nX] = m_acTokens[nPlayer];
            else
                break;
        }
    }
}

void Reversi::FlipUpLeft(int nX, int nY, int nPlayer)
{
    if (ContiguousUpLeft(nX, nY, nPlayer))
    {
        while (true)
        {
            --nX;
            --nY;

            if (m_acGrid[nY][nX] != m_acTokens[nPlayer])
                m_acGrid[nY][nX] = m_acTokens[nPlayer];
            else
                break;
        }
    }
}

bool Reversi::Contiguous(int nX, int nY, int nPlayer) const
{
    if (ContiguousUp(nX, nY, nPlayer))
        return true;

    if (ContiguousUpRight(nX, nY, nPlayer))
        return true;

    if (ContiguousRight(nX, nY, nPlayer))
        return true;

    if (ContiguousDownRight(nX, nY, nPlayer))
        return true;

    if (ContiguousDown(nX, nY, nPlayer))
        return true;

    if (ContiguousDownLeft(nX, nY, nPlayer))
        return true;

    if (ContiguousLeft(nX, nY, nPlayer))
        return true;

    if (ContiguousUpLeft(nX, nY, nPlayer))
        return true;

    return false;
}

bool Reversi::ContiguousUp(int nX, int nY, int nPlayer) const
{
    bool bOpponentPieceAdjacent = false;

    while (true)
    {
        --nY;

        if (CheckContiguous(nX, nY, nPlayer, bOpponentPieceAdjacent))
            return true;
        else
        {
            if (!bOpponentPieceAdjacent)
                return false;
        }
    }

    return false;
}

bool Reversi::ContiguousUpRight(int nX, int nY, int nPlayer) const
{
    bool bOpponentPieceAdjacent = false;

    while (true)
    {
        ++nX;
        --nY;

        if (CheckContiguous(nX, nY, nPlayer, bOpponentPieceAdjacent))
            return true;
        else
        {
            if (!bOpponentPieceAdjacent)
                return false;
        }
    }

    return false;
}

bool Reversi::ContiguousRight(int nX, int nY, int nPlayer) const
{
    bool bOpponentPieceAdjacent = false;

    while (true)
    {
        ++nX;

        if (CheckContiguous(nX, nY, nPlayer, bOpponentPieceAdjacent))
            return true;
        else
        {
            if (!bOpponentPieceAdjacent)
                return false;
        }
    }

    return false;
}

bool Reversi::ContiguousDownRight(int nX, int nY, int nPlayer) const
{
    bool bOpponentPieceAdjacent = false;

    while (true)
    {
        ++nX;
        ++nY;

        if (CheckContiguous(nX, nY, nPlayer, bOpponentPieceAdjacent))
            return true;
        else
        {
            if (!bOpponentPieceAdjacent)
                return false;
        }
    }

    return false;
}

bool Reversi::ContiguousDown(int nX, int nY, int nPlayer) const
{
    bool bOpponentPieceAdjacent = false;

    while (true)
    {
        ++nY;

        if (CheckContiguous(nX, nY, nPlayer, bOpponentPieceAdjacent))
            return true;
        else
        {
            if (!bOpponentPieceAdjacent)
                return false;
        }
    }

    return false;
}

bool Reversi::ContiguousDownLeft(int nX, int nY, int nPlayer) const
{
    bool bOpponentPieceAdjacent = false;

    while (true)
    {
        --nX;
        ++nY;

        if (CheckContiguous(nX, nY, nPlayer, bOpponentPieceAdjacent))
            return true;
        else
        {
            if (!bOpponentPieceAdjacent)
                return false;
        }
    }

    return false;
}

bool Reversi::ContiguousLeft(int nX, int nY, int nPlayer) const
{
    bool bOpponentPieceAdjacent = false;

    while (true)
    {
        --nX;

        if (CheckContiguous(nX, nY, nPlayer, bOpponentPieceAdjacent))
            return true;
        else
        {
            if (!bOpponentPieceAdjacent)
                return false;
        }
    }

    return false;
}

bool Reversi::ContiguousUpLeft(int nX, int nY, int nPlayer) const
{
    bool bOpponentPieceAdjacent = false;

    while (true)
    {
        --nX;
        --nY;

        if (CheckContiguous(nX, nY, nPlayer, bOpponentPieceAdjacent))
        {
            return true;
        }
        else
        {
            if (!bOpponentPieceAdjacent)
                return false;
        }
    }

    return false;
}

bool Reversi::CheckContiguous(int nX, int nY, int nPlayer, bool &bOpponentPieceAdjacent) const
{
    if (!ValidMove(nX, nY))
    {
        bOpponentPieceAdjacent = false;
        return false;
    }

    if (m_acGrid[nY][nX] == m_kcClear)
    {
        bOpponentPieceAdjacent = false;
        return false;
    }

    if (m_acGrid[nY][nX] == m_acTokens[2 - nPlayer + 1])
    {
        bOpponentPieceAdjacent = true;
        return false;
    }

    if (m_acGrid[nY][nX] == m_acTokens[nPlayer])
    {
        if (bOpponentPieceAdjacent)
            return true;
        else
            return false;
    }

    bOpponentPieceAdjacent = false;
    return false;
}

int Reversi::EvaluateGameState(int nPlayer)
{
    if (m_nWinner == nPlayer)
        return 1000000;

    if (m_nWinner == (1 - nPlayer + 2))
        return -1000000;


    return Count(nPlayer) - Count(2 - nPlayer + 1);
}

int Reversi::Count(int nPlayer) const
{
    int nCount = 0;

    for (int xxx = 0; xxx < m_knX; ++xxx)
    {
        for (int yyy = 0; yyy < m_knY; ++yyy)
        {
            if (m_acGrid[yyy][xxx] == m_acTokens[nPlayer])
                ++nCount;
        }
    }

    return nCount;
}

bool Reversi::GameEnded(int nPlayer)
{
    (void)nPlayer;

    m_nWinner = 0;
    m_sWinBy.assign("nothing");

    std::vector<GameMove> vGameMovesPlayer1 = GenerateMoves(m_knPlayer1);
    if (vGameMovesPlayer1.empty())
    {
        std::vector<GameMove> vGameMovesPlayer2 = GenerateMoves(m_knPlayer2);
        if (vGameMovesPlayer2.empty())
            return true;
    }

    return false;
}
