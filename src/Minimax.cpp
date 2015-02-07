#include "Minimax.h"

bool Minimax::Move(Game &cGame)
{
    cGame.Display(true);

    std::cout << "Valid moves: ";
    cGame.DisplayValidMoves();

    GameMove cGameMove = MinimaxMove(m_nPlayerNumber, cGame, m_nDepth);

    cGame.AnnounceMove(m_nPlayerNumber, cGameMove);

     if ( cGame.ApplyMove(m_nPlayerNumber, cGameMove) == -1 )
        return false;

    return true;
}

GameMove Minimax::MinimaxMove(int nPlayer, Game &cGame, int nDepth)
{
    std::vector<GameMove> vGameMoves = cGame.GenerateMoves();
    GameMove cBestMove = vGameMoves[0];
    int nBestScore = {INT_MIN};
    int nAlpha {INT_MIN};
    int nBeta {INT_MAX};

    for (GameMove cGameMove : vGameMoves)
    {
        if ( cGame.ApplyMove(m_nPlayerNumber, cGameMove) == -1 )
        {
            cGame.AnnounceMove(m_nPlayerNumber, cGameMove);
            throw GameAIException("Invalid move");
        }
        int nScore = MinMove(1 - nPlayer + 2, cGame, nDepth - 1, nAlpha, nBeta);

        // TODO: have game provide verbose output?
        if (m_bVerbose) std::cout << "\tMinimaxMove Move=" << cGameMove.ToX() << " Score=" << nScore << std::endl;

        if (nScore == nBestScore)
        {
            if (cGame.PreferredMove(cGameMove) < cGame.PreferredMove(cBestMove))
            {
                cBestMove = cGameMove;
            }
        }

        if (nScore > nBestScore)
        {
            cBestMove = cGameMove;
            nBestScore = nScore;
        }

        cGame.RetractMove(m_nPlayerNumber, cGameMove);
    }

    return cBestMove;
}

int Minimax::MinMove(int nPlayer, Game &cGame, int nDepth, int nAlpha, int nBeta)
{
    if (cGame.GameEnded() || nDepth == 0)
        return cGame.EvaluateGameState(1 - nPlayer + 2) * (nDepth + 1);

    std::vector<GameMove> vGameMoves = cGame.GenerateMoves();

    for (GameMove cGameMove : vGameMoves)
    {

        int nResult = cGame.ApplyMove(nPlayer, cGameMove);
        int nScore = MaxMove(1- nPlayer + 2, cGame, nDepth - 1, nAlpha, nBeta);

        cGame.RetractMove(nResult, cGameMove);

        if (nScore <= nAlpha)
            return nAlpha; // fail hard alpha-cutoff

        if (nScore < nBeta)
            nBeta = nScore; // nBeta acts like min
    }

    return nBeta;
}

int Minimax::MaxMove(int nPlayer, Game &cGame, int nDepth, int nAlpha, int nBeta)
{
    if (cGame.GameEnded() || nDepth == 0)
        return cGame.EvaluateGameState(nPlayer) * (nDepth + 1);

    std::vector<GameMove> vGameMoves = cGame.GenerateMoves();

    for (GameMove cGameMove : vGameMoves)
    {
        int nResult = cGame.ApplyMove(nPlayer, cGameMove);

        int nScore = MinMove(1 - nPlayer + 2, cGame, nDepth -1, nAlpha, nBeta);

        cGame.RetractMove(nResult, cGameMove);

        if (nScore >= nBeta)
            return nBeta; // fail hard beta-cutoff

        if (nScore > nAlpha)
            nAlpha = nScore; // nAlpha acts like a max
    }

    return nAlpha;
}
