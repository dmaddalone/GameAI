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
    int nAlpha {INT_MIN};
    int nBeta {INT_MAX};

    for (int nMove : vMoves)
    {
        //std::cout << "\r" << Spin() << std::flush;

        // TODO: how to capture result of move in m_stMove structure?
        // TODO: change ApplyMove to use m_stMove
        int nResult = cGame.ApplyMove(m_nPlayerNumber, nMove + 1);
        //int nScore = MinMove(1 - nPlayer + 2, cGame, nDepth - 1);
        int nScore = MinMove(1 - nPlayer + 2, cGame, nDepth - 1, nAlpha, nBeta);

        // TODO: have game provide verbose output?
        if (m_bVerbose) std::cout << "\tMinimaxMove Move=" << nMove + 1 << " Score=" << nScore << std::endl;

        if (nScore == nBestScore)
        {
            // TODO: update to use m_stMove
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

//int Minimax::MinMove(int nPlayer, Game &cGame, int nDepth)
int Minimax::MinMove(int nPlayer, Game &cGame, int nDepth, int nAlpha, int nBeta)
{
    //std::cout << "\r" << Spin() << std::flush;

    if (cGame.GameEnded() || nDepth == 0)
        return cGame.EvaluateGameState(1 - nPlayer + 2) * (nDepth + 1);
        //return cGame.EvaluateGameState(1 - nPlayer + 2);

    std::vector<int> vMoves = cGame.GenerateMoves();
    //int nBestScore = {INT_MAX};
    for (int nMove : vMoves)
    {
        int nResult = cGame.ApplyMove(nPlayer, nMove + 1);
        int nScore = MaxMove(1- nPlayer + 2, cGame, nDepth - 1, nAlpha, nBeta);

        //std::cout << "MinMove     Move=" << nMove << " Score=" << nScore << std::endl;
/*
        if (nScore < nBestScore)
        {
            nBestScore = nScore;
        }
*/
        cGame.RetractMove(nResult, nMove);

        if (nScore <= nAlpha)
            return nAlpha; // fail hard alpha-cutoff

        if (nScore < nBeta)
            nBeta = nScore; // nBeta acts like min
    }

    //return nBestScore;
    return nBeta;
}

//int Minimax::MaxMove(int nPlayer, Game &cGame, int nDepth)
int Minimax::MaxMove(int nPlayer, Game &cGame, int nDepth, int nAlpha, int nBeta)
{
    //std::cout << "\r" << Spin() << std::flush;

    if (cGame.GameEnded() || nDepth == 0)
        return cGame.EvaluateGameState(nPlayer) * (nDepth + 1);
        //return cGame.EvaluateGameState(nPlayer);

    std::vector<int> vMoves = cGame.GenerateMoves();
    //int nBestScore {INT_MIN};
    for (int nMove : vMoves)
    {
        int nResult = cGame.ApplyMove(nPlayer, nMove + 1);
        //int nScore = MinMove(1 - nPlayer + 2, cGame, nDepth -1);
        int nScore = MinMove(1 - nPlayer + 2, cGame, nDepth -1, nAlpha, nBeta);

        //std::cout << "MaxMove     Move=" << nMove << " Score=" << nScore << std::endl;
/*
        if (nScore > nBestScore)
        {
            nBestScore = nScore;
        }
*/
        cGame.RetractMove(nResult, nMove);

        if (nScore >= nBeta)
            return nBeta; // fail hard beta-cutoff

        if (nScore > nAlpha)
            nAlpha = nScore; // nAlpha acts like a max
    }

    //return nBestScore;
    return nAlpha;
}
