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
    std::string sMessage;

    if (m_cLogger.Level() >= 1)
        cGame.Display();

    sMessage = "Valid moves: " + cGame.ValidMoves(m_nPlayerNumber);
    m_cLogger.LogInfo(sMessage, 3);

    GameMove cGameMove = MinimaxMove(m_nPlayerNumber, cGame, m_nDepth);

    m_cLogger.LogInfo(cGame.AnnounceMove(m_nPlayerNumber, cGameMove),1);

    if (cGameMove.NoMove())
        return true;

     if (!cGame.ApplyMove(m_nPlayerNumber, cGameMove))
        return false;

    return true;
}

GameMove Minimax::MinimaxMove(int nPlayer, Game &cGame, int nDepth)
{
    int nBestScore = {INT_MIN};
    int nAlpha {INT_MIN};
    int nBeta {INT_MAX};

    std::string sMessage;

    std::vector<GameMove> vGameMoves = cGame.GenerateMoves(nPlayer);

    if (vGameMoves.empty())
    {
        GameMove cNoMove = GameMove();
        cNoMove.SetNoMove(true);
        return cNoMove;
    }

    GameMove cBestMove = vGameMoves[0];

    for (GameMove cGameMove : vGameMoves)
    {
        std::unique_ptr<Game> pcGameClone = cGame.Clone();

        sMessage = "MinimaxMove Player=" + std::to_string(nPlayer) + " Evaluate Move=" + cGameMove.AnnounceToMove();
        m_cLogger.LogInfo(sMessage,3);

        pcGameClone->ApplyMove(nPlayer, cGameMove);

        int nScore = MinMove(1 - nPlayer + 2, *pcGameClone, nDepth - 1, nAlpha, nBeta);

        sMessage = "MinimaxMove Move=" + cGameMove.AnnounceToMove() + " Score=" + std::to_string(nScore);
        m_cLogger.LogInfo(sMessage, 2);

        if (nScore == nBestScore)
        {
            if (pcGameClone->PreferredMove(cGameMove) < pcGameClone->PreferredMove(cBestMove))
            {
                cBestMove = cGameMove;
            }
        }

        if (nScore > nBestScore)
        {
            cBestMove = cGameMove;
            nBestScore = nScore;
        }

        //cGame.RetractMove(m_nPlayerNumber, cGameMove);
    }

    return cBestMove;
}

int Minimax::MinMove(int nPlayer, Game &cGame, int nDepth, int nAlpha, int nBeta)
{
    std::string sMessage;

    if (cGame.GameEnded(nPlayer) || nDepth == 0)
        //return cGame.EvaluateGameState(1 - nPlayer + 2) * (nDepth + 1);
        return cGame.EvaluateGameState(1 - nPlayer + 2);

    std::vector<GameMove> vGameMoves = cGame.GenerateMoves(nPlayer);

    sMessage = "MinMove Depth=" + std::to_string(nDepth) + " Player=" + std::to_string(nPlayer) + " Valid moves: " + cGame.ValidMoves(nPlayer);
    m_cLogger.LogInfo(sMessage,3);

    for (GameMove cGameMove : vGameMoves)
    {
        std::unique_ptr<Game> pcGameClone = cGame.Clone();

        sMessage = "MinMove Player=" + std::to_string(nPlayer) + " Evaluate Move= " + cGameMove.AnnounceToMove();
        m_cLogger.LogInfo(sMessage,3);

        pcGameClone->ApplyMove(nPlayer, cGameMove);

        int nScore = MaxMove(1 - nPlayer + 2, *pcGameClone, nDepth - 1, nAlpha, nBeta);

        sMessage = "MinMove Depth=" + std::to_string(nDepth) + " Player=" + std::to_string(nPlayer) + " Move=" + cGameMove.AnnounceToMove() + " Score=" + std::to_string(nScore);
        m_cLogger.LogInfo(sMessage,3);

        //cGame.RetractMove(nPlayer, cGameMove);

        if (nScore <= nAlpha)
            return nAlpha; // fail hard alpha-cutoff

        if (nScore < nBeta)
            nBeta = nScore; // nBeta acts like min
    }

    return nBeta;
}

int Minimax::MaxMove(int nPlayer, Game &cGame, int nDepth, int nAlpha, int nBeta)
{
    std::string sMessage;

    if (cGame.GameEnded(nPlayer) || nDepth == 0)
        //return cGame.EvaluateGameState(nPlayer) * (nDepth + 1);
        return cGame.EvaluateGameState(nPlayer);

    std::vector<GameMove> vGameMoves = cGame.GenerateMoves(nPlayer);

    sMessage = "MaxMove Depth=" + std::to_string(nDepth) + " Player=" + std::to_string(nPlayer) + " Valid moves: " + cGame.ValidMoves(nPlayer);
    m_cLogger.LogInfo(sMessage,3);

    for (GameMove cGameMove : vGameMoves)
    {
        std::unique_ptr<Game> pcGameClone = cGame.Clone();

        sMessage = "MaxMove Player=" + std::to_string(nPlayer) + " Evaluate Move= " + cGameMove.AnnounceToMove();
        m_cLogger.LogInfo(sMessage,3);

        pcGameClone->ApplyMove(nPlayer, cGameMove);
        int nScore = MinMove(1 - nPlayer + 2, *pcGameClone, nDepth -1, nAlpha, nBeta);

        sMessage = "MaxMove Depth=" + std::to_string(nDepth) + " Player=" + std::to_string(nPlayer) + " Move=" + cGameMove.AnnounceToMove() + " Score=" + std::to_string(nScore);
        m_cLogger.LogInfo(sMessage,3);

        //cGame.RetractMove(nPlayer, cGameMove);

        if (nScore >= nBeta)
            return nBeta; // fail hard beta-cutoff

        if (nScore > nAlpha)
            nAlpha = nScore; // nAlpha acts like a max
    }

    return nAlpha;
}
