#include "Minimax.h"

bool Minimax::Move(Game &cGame)
{
    cGame.Display(true);

    std::cout << "Valid moves: ";
    cGame.DisplayValidMoves();

    int nMove = MinimaxMove(m_nPlayerNumber, cGame, m_nDepth);

    std::cout << "Player " << m_nPlayerNumber << " moves: " << nMove << std::endl;

    if ( cGame.ApplyMove(m_nPlayerNumber, nMove) == -1 )
        return false;

    return true;
}

int Minimax::MinimaxMove(int nPlayer, Game &cGame, int nDepth)
{
    std::vector<int> vMoves = cGame.GenerateMoves();
    int nBestMove = vMoves[0];
    int nBestScore = {INT_MIN};
    for (int nMove : vMoves)

    {
        //std::cout << "\r" << Spin() << std::flush;

        int nResult = cGame.ApplyMove(m_nPlayerNumber, nMove + 1);
        int nScore = MinMove(1 - nPlayer + 2, cGame, nDepth);

        std::cout << "\tMinimaxMove Move=" << nMove + 1 << " Score=" << nScore << std::endl;

        if (nScore == nBestScore)
        {
            if (cGame.PreferredMove(nMove + 1) < cGame.PreferredMove(nBestMove + 1))
            {
                nBestMove = nMove;
            }
        }

        if (nScore > nBestScore)
        {
            nBestMove = nMove;
            nBestScore = nScore;
        }

        cGame.RetractMove(nResult, nMove);
    }

    return nBestMove + 1;
}

int Minimax::MinMove(int nPlayer, Game &cGame, int nDepth)
{
    //std::cout << "\r" << Spin() << std::flush;

    if (cGame.GameEnded() || nDepth == 0)
        return cGame.EvaluateGameState(1 - nPlayer + 2) * (nDepth + 1);

    std::vector<int> vMoves = cGame.GenerateMoves();
    int nBestScore = {INT_MAX};
    for (int nMove : vMoves)
    {
        int nResult = cGame.ApplyMove(nPlayer, nMove + 1);
        int nScore = MaxMove(1- nPlayer + 2, cGame, nDepth - 1);

        //std::cout << "MinMove     Move=" << nMove << " Score=" << nScore << std::endl;

        if (nScore < nBestScore)
        {
            nBestScore = nScore;
        }

        cGame.RetractMove(nResult, nMove);
    }

    return nBestScore;
}

int Minimax::MaxMove(int nPlayer, Game &cGame, int nDepth)
{
    //std::cout << "\r" << Spin() << std::flush;

    if (cGame.GameEnded() || nDepth == 0)
        return cGame.EvaluateGameState(nPlayer) * (nDepth + 1);

    std::vector<int> vMoves = cGame.GenerateMoves();
    int nBestScore {INT_MIN};
    for (int nMove : vMoves)
    {
        int nResult = cGame.ApplyMove(nPlayer, nMove + 1);
        int nScore = MinMove(1 - nPlayer + 2, cGame, nDepth -1);

        //std::cout << "MaxMove     Move=" << nMove << " Score=" << nScore << std::endl;

        if (nScore > nBestScore)
        {
            nBestScore = nScore;
        }

        cGame.RetractMove(nResult, nMove);
    }

    return nBestScore;
}

