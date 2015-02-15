/*
    Copyright 2014 Dom Maddalone

    This file is part of GameAI.

    GameAI is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GameAI is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GameAI.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Minimax.h"

bool Minimax::Move(Game &cGame)
{
    if (m_nVerbosity >= 1)
    {
        cGame.Display();

        std::cout << "Valid moves: ";
        cGame.DisplayValidMoves(m_nPlayerNumber);
    }

    GameMove cGameMove = MinimaxMove(m_nPlayerNumber, cGame, m_nDepth);

    if (m_nVerbosity >= 1)
        cGame.AnnounceMove(m_nPlayerNumber, cGameMove);

     if (!cGame.ApplyMove(m_nPlayerNumber, cGameMove))
        return false;

    return true;
}

GameMove Minimax::MinimaxMove(int nPlayer, Game &cGame, int nDepth)
{
    std::vector<GameMove> vGameMoves = cGame.GenerateMoves(nPlayer);
    GameMove cBestMove = vGameMoves[0];
    int nBestScore = {INT_MIN};
    int nAlpha {INT_MIN};
    int nBeta {INT_MAX};

    for (GameMove cGameMove : vGameMoves)
    {
        cGame.ApplyMove(m_nPlayerNumber, cGameMove);

        int nScore = MinMove(1 - nPlayer + 2, cGame, nDepth - 1, nAlpha, nBeta);

        if (m_nVerbosity >= 2) std::cout << "\tMinimaxMove Move=" << cGameMove.ToX() + 1 << " Score=" << nScore << std::endl;

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
    if (cGame.GameEnded(nPlayer) || nDepth == 0)
        //return cGame.EvaluateGameState(1 - nPlayer + 2) * (nDepth + 1);
        return cGame.EvaluateGameState(1 - nPlayer + 2);

    std::vector<GameMove> vGameMoves = cGame.GenerateMoves(nPlayer);

    for (GameMove cGameMove : vGameMoves)
    {

        cGame.ApplyMove(nPlayer, cGameMove);
        int nScore = MaxMove(1- nPlayer + 2, cGame, nDepth - 1, nAlpha, nBeta);

        cGame.RetractMove(nPlayer, cGameMove);

        if (nScore <= nAlpha)
            return nAlpha; // fail hard alpha-cutoff

        if (nScore < nBeta)
            nBeta = nScore; // nBeta acts like min
    }

    return nBeta;
}

int Minimax::MaxMove(int nPlayer, Game &cGame, int nDepth, int nAlpha, int nBeta)
{
    if (cGame.GameEnded(nPlayer) || nDepth == 0)
        //return cGame.EvaluateGameState(nPlayer) * (nDepth + 1);
        return cGame.EvaluateGameState(nPlayer);

    std::vector<GameMove> vGameMoves = cGame.GenerateMoves(nPlayer);

    for (GameMove cGameMove : vGameMoves)
    {
        cGame.ApplyMove(nPlayer, cGameMove);

        int nScore = MinMove(1 - nPlayer + 2, cGame, nDepth -1, nAlpha, nBeta);

        cGame.RetractMove(nPlayer, cGameMove);

        if (nScore >= nBeta)
            return nBeta; // fail hard beta-cutoff

        if (nScore > nAlpha)
            nAlpha = nScore; // nAlpha acts like a max
    }

    return nAlpha;
}
