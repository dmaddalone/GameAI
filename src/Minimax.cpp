/*
    Copyright 2015 Dom Maddalone

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

/**
  * Make a move.
  *
  * Call MinimaxMove to generate the best game move and apply it to the game.
  *
  * \param cGame The game board.
  *
  * \return True, if applied move is valid.  False otherwise.
  */

bool Minimax::Move(Game &cGame)
{
    // Used to display valid moves
    std::string sMessage;

    // Display game board
    if (m_cLogger.Level() >= 1)
        cGame.Display();

    // Display valid moves
    sMessage = "Valid moves: " + cGame.ValidMoves(m_nPlayerNumber);
    m_cLogger.LogInfo(sMessage, 3);

    // Get best game move
    GameMove cGameMove = MinimaxMove(m_nPlayerNumber, cGame, m_nDepth);

    // Announce game move
    m_cLogger.LogInfo(cGame.AnnounceMove(m_nPlayerNumber, cGameMove),1);

    // If player cannot move, return true
    if (cGameMove.NoMove())
        return true;

    // If game move is not valid, return false
    if (!cGame.ApplyMove(m_nPlayerNumber, cGameMove))
        return false;

    return true;
}

/**
  * Evaluate moves and return the best move for this player.
  *
  * Use the minimax algorithm, with alpha-beta pruning, to determine the
  * best move.
  *
  * \param nPlayer The player whose turn it is.
  * \param cGame   The game board
  * \param nDepth  The number of plies (depth) to evaluate possible game moves
  *
  * \return The best game move.
  */

GameMove Minimax::MinimaxMove(int nPlayer, Game &cGame, int nDepth)
{
    // Initialize the best score for a move to the minimum integer
    int nBestScore = {INT_MIN};
    // Initialiaze the alpha score, the maximum score for the maximizing player, to the minimum integer
    int nAlpha {INT_MIN};
    // Initialize the beta score, the minimum score for the minimizing playerm, to the maximum integer
    int nBeta {INT_MAX};

    // Used for logging
    std::string sMessage;

    // Generate all possible valid moves for this player
    std::vector<GameMove> vGameMoves = cGame.GenerateMoves(nPlayer);

    // If no valid moves are possible, return a "no move"
    if (vGameMoves.empty())
    {
        GameMove cNoMove = GameMove();
        cNoMove.SetNoMove(true);
        return cNoMove;
    }

    // As a default, set the first possible move as the best move
    GameMove cBestMove = vGameMoves[0];

    // Evaluate all possible moves
    for (GameMove cGameMove : vGameMoves)
    {
        // Clone the game board
        std::unique_ptr<Game> pcGameClone = cGame.Clone();

        // Log the current move evaluation
        sMessage = "MinimaxMove Player=" + std::to_string(nPlayer) + " Evaluate Move=" + cGameMove.AnnounceFromMove() + cGameMove.AnnounceToMove();
        m_cLogger.LogInfo(sMessage,3);

        // Apply the move to the game clone
        pcGameClone->ApplyMove(nPlayer, cGameMove);

        // Return the score of this applied move by calling the minimizing player's move evaluation
        int nScore = MinMove(1 - nPlayer + 2, *pcGameClone, nDepth - 1, nAlpha, nBeta);

        // Log the evaluated moves score
        sMessage = "MinimaxMove Move=" + cGameMove.AnnounceFromMove()+ cGameMove.AnnounceToMove() + " Score=" + std::to_string(nScore);
        m_cLogger.LogInfo(sMessage, 2);

        // If the current move's score is equal to the best move's score, ask the game to
        // determine a perferred move
        if (nScore == nBestScore)
        {
            if (pcGameClone->PreferredMove(cGameMove) < pcGameClone->PreferredMove(cBestMove))
            {
                cBestMove = cGameMove;
            }
        }

        // If the current move's score is better than the best move's score, make the current move
        // the best move.
        if (nScore > nBestScore)
        {
            cBestMove = cGameMove;
            nBestScore = nScore;
        }
    }

    return cBestMove;
}

/**
  * Evaluate moves and return the best move for the minimizing player.
  *
  * Use the minimax algorithm, with alpha-beta pruning, to determine the
  * best move.
  *
  * \param nPlayer The minimizing player whose turn it is.
  * \param cGame   The game board
  * \param nDepth  The number of plies (depth) to evaluate possible game moves
  * \param nAlpha  The alpha (maximizing) score
  * \param nBeta   The beta (minimizing) score
  *
  * \return The beta score.
  */

int Minimax::MinMove(int nPlayer, Game &cGame, int nDepth, int nAlpha, int nBeta)
{
    // Used for logging messages
    std::string sMessage;

    // If the game has ended or we have reached the depth of the search,
    // return the score of the game state evaluation from the Min Player perspective
    if (cGame.GameEnded(nPlayer) || nDepth == 0)
        //// TODO: consider modifying the score from the game state evaluation by the depth of the search
        ////return cGame.EvaluateGameState(1 - nPlayer + 2) * (nDepth + 1);
        return cGame.EvaluateGameState(1 - nPlayer + 2);

    // Generate all possible valid moves for the minimizig player
    std::vector<GameMove> vGameMoves = cGame.GenerateMoves(nPlayer);

    // Log the current depth and valid moves
    sMessage = "MinMove Depth=" + std::to_string(nDepth) + " Player=" + std::to_string(nPlayer) + " Valid moves: " + cGame.ValidMoves(nPlayer);
    m_cLogger.LogInfo(sMessage,3);

    // Evaluate all possible moves
    std::unique_ptr<Game> pcGameClone {};
    for (GameMove cGameMove : vGameMoves)
    {
        // Clone the game
        pcGameClone = cGame.Clone();

        // Log the current move evaluation
        sMessage = "MinMove Player=" + std::to_string(nPlayer) + " Evaluate Move= " + cGameMove.AnnounceFromMove() + cGameMove.AnnounceToMove();
        m_cLogger.LogInfo(sMessage,3);

        // Apply the move to the game clone
        pcGameClone->ApplyMove(nPlayer, cGameMove);

        // Return the score of this applied move by calling the maximizing player's move evaluation
        int nScore = MaxMove(1 - nPlayer + 2, *pcGameClone, nDepth - 1, nAlpha, nBeta);

        // Log the evaluated moves score
        sMessage = "MinMove Depth=" + std::to_string(nDepth) + " Player=" + std::to_string(nPlayer) + " Move=" + cGameMove.AnnounceFromMove() + cGameMove.AnnounceToMove() + " Score=" + std::to_string(nScore);
        m_cLogger.LogInfo(sMessage,3);

        // If the score of the current move is less than or equal to the alpha
        // maximizing score, return alpha
        if (nScore <= nAlpha)
            return nAlpha; // fail hard alpha-cutoff

        // If the score of the current move is less than the beta
        // minimzing score, beta is assigned the score of the current move
        if (nScore < nBeta)
            nBeta = nScore; // nBeta acts like min
    }

    return nBeta;
}

int Minimax::MaxMove(int nPlayer, Game &cGame, int nDepth, int nAlpha, int nBeta)
{
    // Used for logging messages
    std::string sMessage;

    // If the game has ended or we have reached the depth of the search,
    // return the score of the game state evaluation from the Max Player perspective
    if (cGame.GameEnded(nPlayer) || nDepth == 0)
        //// TODO: consider modifying the score from the game state evaluation by the depth of the search
        //return cGame.EvaluateGameState(nPlayer) * (nDepth + 1);
        return cGame.EvaluateGameState(nPlayer);

    // Generate all possible valid moves for the maximizing player
    std::vector<GameMove> vGameMoves = cGame.GenerateMoves(nPlayer);

    // Log the current depth and valid moves
    sMessage = "MaxMove Depth=" + std::to_string(nDepth) + " Player=" + std::to_string(nPlayer) + " Valid moves: " + cGame.ValidMoves(nPlayer);
    m_cLogger.LogInfo(sMessage,3);

    // Evaluate all possible moves

    std::unique_ptr<Game> pcGameClone {};
    for (GameMove cGameMove : vGameMoves)
    {
        // Clone the game
        pcGameClone = cGame.Clone();

        // Log the current move evaluation
        sMessage = "MaxMove Player=" + std::to_string(nPlayer) + " Evaluate Move= " + cGameMove.AnnounceFromMove() + cGameMove.AnnounceToMove();
        m_cLogger.LogInfo(sMessage,3);

        // Apply the move to the game clone
        pcGameClone->ApplyMove(nPlayer, cGameMove);

        // Return the score of this applied move by calling the maximizing player's move evaluation
        int nScore = MinMove(1 - nPlayer + 2, *pcGameClone, nDepth -1, nAlpha, nBeta);

        // Log the evaluated moves score
        sMessage = "MaxMove Depth=" + std::to_string(nDepth) + " Player=" + std::to_string(nPlayer) + cGameMove.AnnounceFromMove() + " Move=" + cGameMove.AnnounceToMove() + " Score=" + std::to_string(nScore);
        m_cLogger.LogInfo(sMessage,3);

        // If the score of the current move is greater than or equal to the beta
        // minimizing score, return beta
        if (nScore >= nBeta)
            return nBeta; // fail hard beta-cutoff

        // If the score of the current move is less than or equal to the alpha
        // maximizing score, alpha is assigned the score of the current move
        if (nScore > nAlpha)
            nAlpha = nScore; // nAlpha acts like a max
    }

    return nAlpha;
}
