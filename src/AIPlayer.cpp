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

#include "AIPlayer.h"

/**
  * Make a move.
  *
  * Call MinimaxMove to generate the best game move and apply it to the game.
  *
  * \param cGame The game board.
  *
  * \return True, if applied move is valid.  False otherwise.
  */

bool AIPlayer::Move(Game &cGame)
{
    // If game requires synchronization, pass turn to next player to synchronize information
    if (cGame.Sync())
    {
        return true;
    }

    // If Another Turn is set for opponent, skip this player's turn
    GameMove cGameMove = cGame.LastMove();
    if (cGameMove.AnotherTurn() && (cGameMove.PlayerNumber() != m_nPlayerNumber))
    {
        return true;
    }

    // Used to display valid moves
    std::string sMessage;

    // Display game board
    if (m_cLogger.Level() >= 1)
        cGame.Display();

    // Display valid moves
    if (m_cLogger.Level() >= 3)
    {
        std::string sMoves = cGame.ValidMoves(m_nPlayerNumber);
        if (sMoves.size() > 0)
        {
            sMessage = "Valid moves: " + cGame.ValidMoves(m_nPlayerNumber);
            m_cLogger.LogInfo(sMessage, 3);
        }
    }

    // Get best game move
    cGameMove = MinimaxMove(m_nPlayerNumber, cGame, m_nDepth);

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
  * \param cGame   The game
  * \param nDepth  The number of plies (depth) to evaluate possible game moves
  *
  * \return The best game move.
  */

GameMove AIPlayer::MinimaxMove(int nPlayer, Game &cGame, int nDepth)
{
    // Initialize the best score for a move to the minimum integer
    int nBestScore {INT_MIN};
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

    // If only one move valid, return it
    if (vGameMoves.size() == 1)
    {
        return vGameMoves[0];
    }

    // As a default, set the first possible move as the best move
    GameMove cBestMove = vGameMoves[0];

    float fGameMoves = vGameMoves.size();
    float fPercentComplete = 0.0;
    float fGameMoveEvaluated = 0.0;

    // Evaluate all possible moves
    std::unique_ptr<Game> pcGameClone {};
    for (GameMove cGameMove : vGameMoves)
    {
        // Let them know that we're thinking
        if (m_cLogger.Level() >= 1)
        {
            ++fGameMoveEvaluated;
            fPercentComplete = fGameMoveEvaluated / fGameMoves * 100;
            std::cout << "\rThinking " << std::fixed << std::setprecision(0) << fPercentComplete << "%" << std::flush;
        }

        // Log the current move evaluation
        sMessage = "MinimaxMove Player=" + std::to_string(nPlayer) + " Evaluate Move=" + cGameMove.AnnounceFromMove() + cGameMove.AnnounceToMove();
        m_cLogger.LogInfo(sMessage,3);

        // Clone the game
        pcGameClone = cGame.Clone();

        // Apply the move to the game clone
        pcGameClone->ApplyMove(nPlayer, cGameMove);

        // Return the score of this applied move by calling the minimizing player's move evaluation
        int nScore = MinMove(3 - nPlayer, *pcGameClone, nDepth - 1, nAlpha, nBeta);

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
  * \param cGame   The game
  * \param nDepth  The number of plies (depth) to evaluate possible game moves
  * \param nAlpha  The alpha (maximizing) score
  * \param nBeta   The beta (minimizing) score
  *
  * \return The beta score.
  */

int AIPlayer::MinMove(int nPlayer, Game &cGame, int nDepth, int nAlpha, int nBeta)
{
    // Used for logging messages
    std::string sMessage;

    // If the game has ended or we have reached the depth of the search,
    // return the score of the game state evaluation from the Min Player perspective
    if (cGame.GameEnded(nPlayer) || nDepth == 0)
        //// TODO: consider modifying the score from the game state evaluation by the depth of the search
        ////return cGame.EvaluateGameState(3 - nPlayer) * (nDepth + 1);
        return cGame.EvaluateGameState(3 - nPlayer);

    // Generate all possible valid moves for the minimizig player
    std::vector<GameMove> vGameMoves = cGame.GenerateMoves(nPlayer);

    // Log the current depth and valid moves
    sMessage = "MinMove Depth=" + std::to_string(nDepth) + " Player=" + std::to_string(nPlayer) +
        " Valid moves: " + cGame.ValidMoves(nPlayer);
    m_cLogger.LogInfo(sMessage,3);

    // Evaluate all possible moves
    std::unique_ptr<Game> pcGameClone {};
    int nScore {};
    for (GameMove cGameMove : vGameMoves)
    {
        // Clone the game
        pcGameClone = cGame.Clone();

        // Apply the move to the game clone
        pcGameClone->ApplyMove(nPlayer, cGameMove);

        // Return the score of this applied move by calling the maximizing player's move evaluation
        nScore = MaxMove(3 - nPlayer, *pcGameClone, nDepth - 1, nAlpha, nBeta);

        // Log the evaluated moves score
        sMessage = "MinMove Depth=" + std::to_string(nDepth) + " Player=" + std::to_string(nPlayer) +
            " Move=" + cGameMove.AnnounceFromMove() + cGameMove.AnnounceToMove() +
            " Score=" + std::to_string(nScore);

        // If the score of the current move is less than or equal to the alpha
        // maximizing score, return alpha
        if (nScore <= nAlpha)
        {
            // Complete log message
            sMessage += " [Alpha=" + std::to_string(nAlpha) + " Beta=" + std::to_string(nBeta) + "] alpha-cutoff";
            m_cLogger.LogInfo(sMessage,3);

            return nAlpha; // fail hard alpha-cutoff
        }

        // If the score of the current move is less than the beta
        // minimizing score, beta is assigned the score of the current move
        if (nScore < nBeta)
            nBeta = nScore; // nBeta acts like min

        // Complete log message
        sMessage += " [Alpha=" + std::to_string(nAlpha) + " Beta=" + std::to_string(nBeta) + "]";
        m_cLogger.LogInfo(sMessage,3);
    }

    return nBeta;
}

int AIPlayer::MaxMove(int nPlayer, Game &cGame, int nDepth, int nAlpha, int nBeta)
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
    sMessage = "MaxMove Depth=" + std::to_string(nDepth) + " Player=" + std::to_string(nPlayer) +
        " Valid moves: " + cGame.ValidMoves(nPlayer);
    m_cLogger.LogInfo(sMessage,3);

    // Evaluate all possible moves

    std::unique_ptr<Game> pcGameClone {};
    int nScore {};
    for (GameMove cGameMove : vGameMoves)
    {
        // Clone the game
        pcGameClone = cGame.Clone();

        // Apply the move to the game clone
        pcGameClone->ApplyMove(nPlayer, cGameMove);

        // Return the score of this applied move by calling the maximizing player's move evaluation
        nScore = MinMove(3 - nPlayer, *pcGameClone, nDepth -1, nAlpha, nBeta);

        // Log the evaluated moves score
        sMessage = "MaxMove Depth=" + std::to_string(nDepth) + " Player=" + std::to_string(nPlayer) +
            cGameMove.AnnounceFromMove() + " Move=" + cGameMove.AnnounceToMove() +
            " Score=" + std::to_string(nScore);

        // If the score of the current move is greater than or equal to the beta
        // minimizing score, return beta
        if (nScore >= nBeta)
        {
            // Complete log message
            sMessage += " [Alpha=" + std::to_string(nAlpha) + " Beta=" + std::to_string(nBeta) + "] beta-cutoff";
            m_cLogger.LogInfo(sMessage,3);

            return nBeta; // fail hard beta-cutoff
        }

        // If the score of the current move is less than or equal to the alpha
        // maximizing score, alpha is assigned the score of the current move
        if (nScore > nAlpha)
            nAlpha = nScore; // nAlpha acts like a max

        // Complete log message
        sMessage += " [Alpha=" + std::to_string(nAlpha) + " Beta=" + std::to_string(nBeta) + "]";
        m_cLogger.LogInfo(sMessage,3);
    }

    return nAlpha;
}

GameMove AIPlayer::ProbabilityMove(int nPlayer, Game &cGame)
{
    if (!m_cBlackBoard.Initialized())
    {
        // Remove cards from ProbableDeck that match all my hand's cards
        // Set initialize flag
    }

    // Check last move; if Ask by opponent, add this card to ProbableOpponentHand, with high probability
    // Based on game, update rank probabilities -> void cGame.BlackboardUpdate(cBlackBoard);
    // Based on game, generate move with best probability -> GameMove cBestMove = cGame.BlackboardMove(cBlackBoard);

    GameMove cBestMove;

    return cBestMove;
}
