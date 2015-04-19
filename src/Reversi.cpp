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

#include "Reversi.h"

/**
  * Initilize the board for Reversi play.
  *
  * Set the tokens for both players in the central squares of the board.
  */

void Reversi::SetBoard()
{
    m_acGrid[(m_knY / 2) - 1][(m_knX / 2) - 1] = m_acTokens[2];
    m_acGrid[(m_knY / 2) - 1][(m_knX / 2)]     = m_acTokens[1];
    m_acGrid[(m_knY / 2)][(m_knX / 2) - 1]     = m_acTokens[1];
    m_acGrid[(m_knY / 2)][(m_knX / 2)]         = m_acTokens[2];
}

/**
  * Return a vector of valid game moves.
  *
  * Review and collect all valid moves for a player into a vector.
  *
  * \param nPlayer The player whose turn it is.
  *
  * \return A vector of valid moves.
  */

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


/**
  * Apply the move to the game.
  *
  * Find the bottom of the grid for the selected move, update the GameMove
  * object with that location, and pass it to LinearGame for application.
  *
  * \param nPlayer The player whose turn it is.
  * \param cGameMove The game move
  *
  * \return True if the move is valid, false otherwise
  */

bool Reversi::ApplyMove(int nPlayer, GameMove &cGameMove)
{
    bool bValidMove = false;

    // generate a vector lf all possible valid moves for this player
    std::vector<GameMove> vGameMoves = GenerateMoves(nPlayer);

    // If the GameMove (passed to this method) is not valid for a LinearGame
    // return false
    if (!LinearGame::ApplyMove(nPlayer, cGameMove))
        return false;

    // Compare passed GameMove to generated game moves.  If one is found to be
    // the same, flip tokens accordingly.
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

/**
  * Flip (change) tokens.
  *
  * Examine all directions from the GameMove location, and flip tokens
  * accordingly.
  *
  * \param nPlayer The player whose turn it is.
  * \param cGameMove The game move
  */

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

/**
  * Flip (change) tokens along a vertical-up line.
  *
  * Examine a line "up" from token location and flip all contiguous tokens
  * accordingly.
  *
  * \param nX The token's X-coordinate
  * \param nY The tokens's Y-coordinate
  * \param nPlayer The player whose turn it is.
  */

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

/**
  * Flip (change) tokens along a diagonal-up-and-right line.
  *
  * Examine a line "up and to the right" from token location and flip all
  * contiguous tokens accordingly.
  *
  * \param nX The token's X-coordinate
  * \param nY The tokens's Y-coordinate
  * \param nPlayer The player whose turn it is.
  */

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

/**
  * Flip (change) tokens along a horizontal-right line.
  *
  * Examine a line "to the right" from token location and flip all
  * contiguous tokens accordingly.
  *
  * \param nX The token's X-coordinate
  * \param nY The tokens's Y-coordinate
  * \param nPlayer The player whose turn it is.
  */

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

/**
  * Flip (change) tokens along a diagonal-down-and-right line.
  *
  * Examine a line "down and to the right" from token location and flip all
  * contiguous tokens accordingly.
  *
  * \param nX The token's X-coordinate
  * \param nY The tokens's Y-coordinate
  * \param nPlayer The player whose turn it is.
  */

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

/**
  * Flip (change) tokens along a dvertical-down line.
  *
  * Examine a line "down" from token location and flip all
  * contiguous tokens accordingly.
  *
  * \param nX The token's X-coordinate
  * \param nY The tokens's Y-coordinate
  * \param nPlayer The player whose turn it is.
  */

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

/**
  * Flip (change) tokens along a diagonal-down-and-left line.
  *
  * Examine a line "down and to the left" from token location and flip all
  * contiguous tokens accordingly.
  *
  * \param nX The token's X-coordinate
  * \param nY The tokens's Y-coordinate
  * \param nPlayer The player whose turn it is.
  */

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

/**
  * Flip (change) tokens along a horizontal-left line.
  *
  * Examine a line "to the left" from token location and flip all
  * contiguous tokens accordingly.
  *
  * \param nX The token's X-coordinate
  * \param nY The tokens's Y-coordinate
  * \param nPlayer The player whose turn it is.
  */

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

/**
  * Flip (change) tokens along a diagonal-up-and-left line.
  *
  * Examine a line "up and to the left" from token location and flip all
  * contiguous tokens accordingly.
  *
  * \param nX The token's X-coordinate
  * \param nY The tokens's Y-coordinate
  * \param nPlayer The player whose turn it is.
  */

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

/**
  * Evaluate the contiguousness of a player's tokens from a location
  * on the board.  To make a valid move in Reversi, the location must have
  * at least one straight (horizontal, vertical, or diagonal) occupied line
  * between the player's new token and another token, with one or more contiguous
  * opponent's tokens between them.
  *
  * Examine all directions from a given location, returning true if any
  * direction has contiguous tokens.
  *
  * \param nX The token's X-coordinate
  * \param nY The token's Y-coordinate
  * \param nPlayer The player whose turn it is.
  */

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

/**
  * Examine tokens along a vertical-up line.
  *
  * Examine a line "up" from token location for contiguousness.
  *
  * \param nX The token's X-coordinate
  * \param nY The token's Y-coordinate
  * \param nPlayer The player whose turn it is.
  *
  * \return True if a contiguous line exists.  False otherwise.
  */

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

/**
  * Examine tokens along a diagonal-up-and-right line.
  *
  * Examine a line "up and to the right" from token location for contiguousness.
  *
  * \param nX The token's X-coordinate
  * \param nY The token's Y-coordinate
  * \param nPlayer The player whose turn it is.
  *
  * \return True if a contiguous line exists.  False otherwise.
  */

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

/**
  * Examine tokens along a horizontal-right line.
  *
  * Examine a line "to the right" from token location for contiguousness.
  *
  * \param nX The token's X-coordinate
  * \param nY The token's Y-coordinate
  * \param nPlayer The player whose turn it is.
  *
  * \return True if a contiguous line exists.  False otherwise.
  */

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

/**
  * Examine tokens along a diagonal-down-and-right line.
  *
  * Examine a line "down and to the right" from token location for contiguousness.
  *
  * \param nX The token's X-coordinate
  * \param nY The token's Y-coordinate
  * \param nPlayer The player whose turn it is.
  *
  * \return True if a contiguous line exists.  False otherwise.
  */

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

/**
  * Examine tokens along a vertical-down line.
  *
  * Examine a line "down" from token location for contiguousness.
  *
  * \param nX The token's X-coordinate
  * \param nY The token's Y-coordinate
  * \param nPlayer The player whose turn it is.
  *
  * \return True if a contiguous line exists.  False otherwise.
  */

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

/**
  * Examine tokens along a diagonal-down-and-left line.
  *
  * Examine a line "down and to the left" from token location for contiguousness.
  *
  * \param nX The token's X-coordinate
  * \param nY The token's Y-coordinate
  * \param nPlayer The player whose turn it is.
  *
  * \return True if a contiguous line exists.  False otherwise.
  */

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

/**
  * Examine tokens along a horizontal-left line.
  *
  * Examine a line "left" from token location for contiguousness.
  *
  * \param nX The token's X-coordinate
  * \param nY The token's Y-coordinate
  * \param nPlayer The player whose turn it is.
  *
  * \return True if a contiguous line exists.  False otherwise.
  */

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

/**
  * Examine tokens along a diagonal-up-and-left line.
  *
  * Examine a line "up and to the left" from token location for contiguousness.
  *
  * \param nX The token's X-coordinate
  * \param nY The token's Y-coordinate
  * \param nPlayer The player whose turn it is.
  *
  * \return True if a contiguous line exists.  False otherwise.
  */

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

/**
  * Examine the space on the board for contiguousness.
  *
  * Examine
  *
  * \param nX The X-coordinate of the game location under evaluation.
  * \param nY The Y-coordinate of the game location under evaluation.
  * \param nPlayer The player whose turn it is.
  * \param bOpponentPieceAdjacent A boolean flag indicating whether
  *        the opponent player has a token adjacent to this space.
  *
  * \return True if a contiguous line identified.  False otherwise.
  */

bool Reversi::CheckContiguous(int nX, int nY, int nPlayer, bool &bOpponentPieceAdjacent) const
{
    // If location is not valid, set opponent piece adjacent to false and return false
    if (!ValidMove(nX, nY))
    {
        bOpponentPieceAdjacent = false;
        return false;
    }

    // If location is clear, set opponent piece adjacent to false and return false
    if (m_acGrid[nY][nX] == m_kcClear)
    {
        bOpponentPieceAdjacent = false;
        return false;
    }

    // If location contains an opponent's token, set opponent piece adjacent to true and return false
    if (m_acGrid[nY][nX] == m_acTokens[2 - nPlayer + 1])
    {
        bOpponentPieceAdjacent = true;
        return false;
    }

    // If location contains a player's token, and we have already identified an opponent's token as adjacent,
    // then return true.  Otherwise false.
    if (m_acGrid[nY][nX] == m_acTokens[nPlayer])
    {
        if (bOpponentPieceAdjacent)
            return true;
        else
            return false;
    }

    // If no antecedants triggered, set opponent piece adjacent to false and return false
    bOpponentPieceAdjacent = false;
    return false;
}

/**
  * Evaluate the game state.
  *
  * From a player's perspective, return a value cooresponding to the player's
  * standing in the game.  If the player has won the game, return a large,
  * positive integer.  If winning, a smaller, positive integer. If lost or
  * losing, a negative integer.
  *
  * \param nPlayer The player whose turn it is.
  *
  * \return An integer representing game state for the player.
  */

int Reversi::EvaluateGameState(int nPlayer)
{
    // If won, return largest positive integer
    if (m_nWinner == nPlayer)
        return INT_MAX;

    // If lost, return largest negative integer
    if (m_nWinner == (1 - nPlayer + 2))
        return INT_MIN;

    // Evaluate the number of tokens for each player. "Greedy evaluation."
    int nCountEval  = CountEvaluation(nPlayer)  - CountEvaluation(2 - nPlayer + 1);
    // Evaluate  the value of the occupied positions.
    int nSquareEval = SquareEvaluation(nPlayer) - SquareEvaluation(2 - nPlayer + 1);
    // Evaluate the number of moves available.
    int nMobilityEval = MobilityEvaluation(nPlayer) - MobilityEvaluation(2 - nPlayer + 1);

    return (nCountEval * 10) + (nSquareEval * 15) + (nMobilityEval * 5);

}

/**
  * Count the number of tokens for a player.
  *
  * \param nPlayer The player whose turn it is.
  *
  * \return An integer representing the number of tokens for this player.
  */

int Reversi::CountEvaluation(int nPlayer) const
{
    int nEval = 0;

    for (int xxx = 0; xxx < m_knX; ++xxx)
    {
        for (int yyy = 0; yyy < m_knY; ++yyy)
        {
            if (m_acGrid[yyy][xxx] == m_acTokens[nPlayer])
                ++nEval;
        }
    }

    return nEval;
}

/**
  * Evaluate the square positions occurpied by the player.  Use a static
  * table of square values.
  *
  * \param nPlayer The player whose turn it is.
  *
  * \return An integer representing the position values occupied by the player.
  */

int Reversi::SquareEvaluation(int nPlayer) const
{
    int nEval = 0;

    for (int xxx = 0; xxx < m_knX; ++xxx)
    {
        for (int yyy = 0; yyy < m_knY; ++yyy)
        {
            if (m_acGrid[yyy][xxx] == m_acTokens[nPlayer])
                nEval += m_kaiEvalTable[yyy][xxx];
        }
    }

    return nEval;
}

/**
  * Evaluate the number of valid moves for the player.
  *
  * \param nPlayer The player whose turn it is.
  *
  * \return An integer representing the number of valid moves for the player.
  */

int Reversi::MobilityEvaluation(int nPlayer) const
{

    // Generate all possible valid moves for this player
    std::vector<GameMove> vGameMoves = GenerateMoves(nPlayer);

    return vGameMoves.size();
}

/**
  * Return a string providing a current score of the game.
  *
  * \return A string reporting the game score.
  */

std::string Reversi::GameScore() const
{
    std::string sScore = "Score: Player 1=" + std::to_string(CountEvaluation(m_knPlayer1)) +
                         " Player 2=" + std::to_string(CountEvaluation(m_knPlayer2));
    return sScore;
}

/**
  * Check to see if a player has won the game.
  *
  * For a each player, count the number of tokens.
  *
  * \param nPlayer The player
  *
  * \return True, if any player has won the game.  False otherwise.
  */

bool Reversi::GameEnded(int nPlayer)
{
    // nPlayer not used in this override
    (void)nPlayer;

    // Count the number of tokens for each player
    int nCountPlayer1 = CountEvaluation(m_knPlayer1);
    int nCountPlayer2 = CountEvaluation(m_knPlayer2);

    // Create a string for the winning player
    m_sWinBy = "a difference of " + std::to_string(abs(nCountPlayer1 - nCountPlayer2));

    if (nCountPlayer1 > nCountPlayer2)
    {
        m_nWinner = m_knPlayer1;
    }
    else
    {
        m_nWinner = m_knPlayer2;
    }

    // If nor more moves are available for either player, the game is over; return true
    std::vector<GameMove> vGameMovesPlayer1 = GenerateMoves(m_knPlayer1);
    if (vGameMovesPlayer1.empty())
    {
        std::vector<GameMove> vGameMovesPlayer2 = GenerateMoves(m_knPlayer2);
        if (vGameMovesPlayer2.empty())
            return true;
    }

    // Return false - the game has not ended
    m_nWinner = 0;
    return false;
}
