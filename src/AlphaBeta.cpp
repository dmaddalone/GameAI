#include "AlphaBeta.h"

bool AlphaBeta::Move(Game &cGame)
{
    cGame.Display(true);

    std::cout << "Valid moves: ";
    cGame.DisplayValidMoves();

    int nMove = AlphaBetaMove(m_nPlayerNumber, cGame, m_nDepth);

    std::cout << "Player " << m_nPlayerNumber << " moves: " << nMove << std::endl;

    if ( cGame.ApplyMove(m_nPlayerNumber, nMove) == -1 )
        return false;

    return true;
}

int AlphaBeta::AlphaBetaMove(int nPlayer, Game &cGame, int nDepth)
{
    std::vector<int> vMoves = cGame.GenerateMoves();
    int nBestMove = vMoves[0];
    int nBestScore = {INT_MIN};
    int nAlpha {INT_MIN};
    int nBeta {INT_MAX};

    for (int nMove : vMoves)
    {
        int nResult = cGame.ApplyMove(m_nPlayerNumber, nMove + 1);
        int nScore = AlphaBetaMinMove(1 - nPlayer + 2, cGame, nDepth, nAlpha, nBeta);

        if (nScore > nBestScore)
        {
            nBestMove = nMove;
            nBestScore = nScore;
        }

        cGame.RetractMove(nResult, nMove);
    }

    return nBestMove + 1;
}

int AlphaBeta::AlphaBetaMinMove(int nPlayer, Game &cGame, int nDepth, int nAlpha, int nBeta)
{
    if (cGame.GameEnded() || nDepth == 0)
        return cGame.EvaluateGameState(1 - nPlayer + 2);

    std::vector<int> vMoves = cGame.GenerateMoves();

    for (int nMove : vMoves)
    {
        int nResult = cGame.ApplyMove(nPlayer, nMove + 1);
        int nScore = AlphaBetaMaxMove(1- nPlayer + 2, cGame, nDepth - 1, nAlpha, nBeta);

        cGame.RetractMove(nResult, nMove);

        if (nScore <= nAlpha)
            return nAlpha;

        if (nScore < nBeta)
            nBeta = nScore;
    }

    return nBeta;
}

int AlphaBeta::AlphaBetaMaxMove(int nPlayer, Game &cGame, int nDepth, int nAlpha, int nBeta)
{
    if (cGame.GameEnded() || nDepth == 0)
        return cGame.EvaluateGameState(nPlayer);

    std::vector<int> vMoves = cGame.GenerateMoves();

    for (int nMove : vMoves)
    {
        int nResult = cGame.ApplyMove(nPlayer, nMove + 1);
        int nScore = AlphaBetaMinMove(1 - nPlayer + 2, cGame, nDepth -1, nAlpha, nBeta);

        cGame.RetractMove(nResult, nMove);

        if (nScore >= nBeta)
            return nBeta;

        if (nScore > nAlpha)
            nAlpha = nScore;
    }

    return nAlpha;
}
