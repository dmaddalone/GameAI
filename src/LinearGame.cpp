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

#include "LinearGame.h"

/**
  * Generate a GameMove from a string.
  *
  * From a string, generate a GameMove object.
  *
  * \param sMove A string representing a game move.
  *
  * \return A GameMove object.
  */

GameMove LinearGame::GenerateMove(std::string sMove) const
{
    GameMove cGameMove = BoardGame::GenerateMove(sMove);

    if (cGameMove.Resignation())
        return cGameMove;

    // Set the To X coordinate from the first charatcter of the string.
    cGameMove.SetToX(sMove[0]);

    if (m_bUseY)
    {
        cGameMove.SetUseY(true);

        // Remove the first character ...
        sMove.erase(0,1);

        // ... and if more of the string exists, set the To Y coordinate
        if (sMove.length() > 0)
        {
            try
            {
                cGameMove.SetToY(sMove[0]);
            }
            catch (...)
            {
                cGameMove.SetToY(-1);
            }
        }
        else
        {
            cGameMove.SetToY(-1);
        }
    }
    else
    {
        cGameMove.SetUseY(false);
    }

   return cGameMove;
}

/**
  * Apply a move to the game.
  *
  * After checking to for a move's validity, apply the move to the game.
  *
  * \param nPlayer   The player whose turn it is.
  * \param cGameMove The player's move
  *
  * \return True, if valid.  False otherwise.
  */

bool LinearGame::ApplyMove(int nPlayer, GameMove &cGameMove)
{
    // Check player number
    if ((nPlayer != m_knPlayer1) && (nPlayer != m_knPlayer2))
        return false;

    // Check for resignation
    if (cGameMove.Resignation())
    {
        // Capture move for later playback or analysis
        m_vGameMoves.push_back(cGameMove);
        return true;
    }

    if (!cBoard.ValidLocation(cGameMove.ToX(), cGameMove.ToY()))
        return false;

    // Check to see if a space is clear
    if (cBoard.PositionOccupied(cGameMove.ToX(), cGameMove.ToY()))
        return false;

    // Apply move to the board
    GamePiece cGamePiece(m_acTokens[nPlayer], nPlayer);
    cBoard.SetPiece(cGameMove.ToX(), cGameMove.ToY(), cGamePiece);

    // Increment move counter
    ++m_nNumberOfMoves;

    // Capture move for later playback or analysis
    m_vGameMoves.push_back(cGameMove);

    return true;
}

/**
  * Announce game move.
  *
  * Construct and return a string containing the move.
  *
  * \param nPlayer   The player whose turn it is.
  * \param cGameMove The player's move
  *
  * \return A string containing the move.
  */

std::string LinearGame::AnnounceMove(int nPlayer, const GameMove &cGameMove) const
{
    std::string sMessage = "\rMove number: " + std::to_string(m_nNumberOfMoves + 1) + "  Player: " + std::to_string(nPlayer) + "  Moves To: " + cGameMove.AnnounceToMove();

    return sMessage;
}

/**
  * Evaluate the game state.
  *
  * From a player's perspective, return a value cooresponding to the player's
  * standing in the game.  If the player has won the game, return a large,
  * positive integer.  If winning, a smaller, positive integer. If lost or
  * losing, a negative integer.
  *
  * \param nPlayer   The player whose turn it is.
  *
  * \return An integer representing game state for the player.
  */

int LinearGame::EvaluateGameState(int nPlayer)
{
    // If won, return largest positive integer // TODO: make these constants
    if (m_nWinner == nPlayer)
        return INT_MAX;

    // If lost, return largest negative integer // TODO: make these constants
    if (m_nWinner == (1 - nPlayer + 2))
        return INT_MIN;

    // Clear structs for game state; one for player and one for opponent
    m_stMyCounts = {};
    m_stOpponentCounts = {};

    // Evaluate the entire board and count sequences of tokens in all directions
    // (horizontal, vertical, and both diagonals for both players.
    if (m_nWinner == 0)
    {
        for (int yyy = 0; yyy < m_knY; ++yyy)
        {
            for (int xxx = 0; xxx < m_knX; ++xxx)
            {
                CountSequence(CheckHorizontal(nPlayer, yyy, xxx), m_stMyCounts);
                CountSequence(CheckHorizontal(1 - nPlayer + 2, yyy, xxx), m_stOpponentCounts);
                CountSequence(CheckVertical(nPlayer, yyy, xxx), m_stMyCounts);
                CountSequence(CheckVertical(1 - nPlayer + 2, yyy, xxx), m_stOpponentCounts);
                CountSequence(CheckDiagonalUpperLeftLowerRight(nPlayer, yyy, xxx), m_stMyCounts);
                CountSequence(CheckDiagonalUpperLeftLowerRight(1 - nPlayer + 2, yyy, xxx), m_stOpponentCounts);
                CountSequence(CheckDiagonalUpperRightLowerLeft(nPlayer, yyy, xxx), m_stMyCounts);
                CountSequence(CheckDiagonalUpperRightLowerLeft(1 - nPlayer + 2, yyy, xxx), m_stOpponentCounts);
            }
        }

        // Calculate a number based on the number of sequence counts for the player
        // less the number of sequence counts for the opponent, weighting the opponent's
        // sequence counts higher.
        return ( (m_stMyCounts.nCount2 * 10) - (m_stOpponentCounts.nCount2 * 100) +
                 (m_stMyCounts.nCount3 * 1000) - (m_stOpponentCounts.nCount3 * 10000) );
    }

    return 0;
}

/**
  * Count sequences of contiguous tokens.
  *
  * Increment a sequence count.
  *
  * \param nSequence        The length of a contiguous line of tokens to count
  * \param stSequenceCounts The struct containing the sequence counts
  */

void LinearGame::CountSequence(int nSequence, SequenceCounts &stSequenceCounts)
{
    switch (nSequence)
    {
        case 0:
            break;
        case 1:
            ++stSequenceCounts.nCount1;
            break;
        case 2:
            ++stSequenceCounts.nCount2;
            break;
        case 3:
            ++stSequenceCounts.nCount3;
            break;
        default:
            break; // TODO - should not get here
    }
    return;
}

/**
  * Check for a number of contiguous tokens along orthogonal lines
  *
  * Evaluate the entire board horizontally and vertically for a number of
  * contiguous tokens for a player.
  *
  * \param nPlayer  The player whose turn it is
  * \param nConnect The number of contiguous (connected) tokens to evaluate for
  *
  * \return True, if the length of contiguous line of tokens matches the nConnect parameter.  False otherwise.
  */

bool LinearGame::CheckOrthogonal(int nPlayer, int nConnect)
{
    bool bWin = false;

    // Evaluate the entire board, checking horizontally for the player's tokens
    for (int yyy = 0; yyy < m_knY; ++yyy)
    {
        for (int xxx = 0; xxx < m_knX; ++xxx)
        {
            // If a contiguous line is found that matches the desired length,
            // set the bWin flag to true and assign a value to sWinBy.
            if (CheckHorizontal(nPlayer, yyy, xxx) == nConnect)
            {
                bWin = true;
                m_sWinBy.assign("Horizontal");
                break;
            }
        }

        // If game won, stop evaluating
        if (bWin)
            break;
    }

    // Evaluate the entire board, checking vertically for the player's tokens
    if (!bWin)
    {
        for (int yyy = 0; yyy < m_knY; ++yyy)
        {
            for (int xxx = 0; xxx < m_knX; ++xxx)
            {
                // If a contiguous line is found that matches the desired length,
                // set the bWin flag to true and assign a value to sWinBy.
                if (CheckVertical(nPlayer, yyy, xxx) == nConnect)
                {
                    bWin = true;
                    m_sWinBy.assign("Vertical");
                    break;
                }
            }

            // If game won, stop evaluating
            if (bWin)
                break;
        }
    }

    return bWin;
}

/**
  * Check for a number of contiguous tokens along horizontal lines
  *
  * A recursive function that is given a pair of X- and Y-coordinates and moves
  * horizontally upwards, counting the number of contiguus tokens for a player.
  *
  * \param nPlayer  The player whose turn it is
  * \param y        The Y-coordinate
  * \param x        The X-coordinate
  *
  * \return The number of contiguous tokens counted
  */

int LinearGame::CheckHorizontal(int nPlayer, int y, int x) const
{
    if (!cBoard.ValidLocation(x, y)) return 0;

    if (cBoard.PositionOccupiedByPlayer(x, y, nPlayer))
        return (1 + CheckHorizontal(nPlayer, y, x+1));
    else
        return 0;
}

/**
  * Check for a number of contiguous tokens along vertical lines
  *
  * A recursive function that is given a pair of X- and Y-coordinates and moves
  * vertically to the right, counting the number of contiguus tokens for a player.
  *
  * \param nPlayer  The player whose turn it is
  * \param y        The Y-coordinate
  * \param x        The X-coordinate
  *
  * \return The number of contiguous tokens counted
  */

int LinearGame::CheckVertical(int nPlayer, int y, int x) const
{
    if (!cBoard.ValidLocation(x, y)) return 0;

    if (cBoard.PositionOccupiedByPlayer(x, y, nPlayer))
        return (1 + CheckVertical(nPlayer, y+1, x));
    else
        return 0;
}

/**
  * Check for a number of contiguous tokens along diagonal lines
  *
  * Evaluate the entire board diagnollay for a number of
  * contiguous tokens for a player.
  *
  * \param nPlayer  The player whose turn it is
  * \param nConnect The number of contiguous (connected) tokens to evaluate for
  *
  * \return True, if the length of contiguous line of tokens matches the nConnect parameter.  False otherwise.
  */

bool LinearGame::CheckDiagonal(int nPlayer, int nConnect)
{
    bool bWin = false;

    // Evaluate the entire board, checking diagonally for the player's tokens
    for (int yyy = 0; yyy < m_knY; ++yyy)
    {
        for (int xxx = 0; xxx < m_knX; ++xxx)
        {
            // If a contiguous line is found that matches the desired length,
            // set the bWin flag to true and assign a value to sWinBy.
            if (CheckDiagonalUpperLeftLowerRight(nPlayer, yyy, xxx) == nConnect)
            {
                bWin = true;
                m_sWinBy.assign("DiagonalULLR");
                break;
            }
        }
        // If game won, stop evaluating
        if (bWin)
            break;
    }

    // Evaluate the entire board, checking diagonally for the player's tokens
    if (!bWin)
    {
        for (int yyy = 0; yyy < m_knY; ++yyy)
        {
            for (int xxx = m_knX - 1; xxx >= 0; --xxx)
            {
                // If a contiguous line is found that matches the desired length,
                // set the bWin flag to true and assign a value to sWinBy.
                if (CheckDiagonalUpperRightLowerLeft(nPlayer, yyy, xxx) == nConnect)
                {
                    bWin = true;
                    m_sWinBy.assign("DiagonalURLL");
                    break;
                }
            }
            // If game won, stop evaluating
            if (bWin)
                break;
        }
    }

    return bWin;
}

/**
  * Check for a number of contiguous tokens along upper-left to lower-right
  * diagnoal lines
  *
  * A recursive function that is given a pair of X- and Y-coordinates and moves
  * diagnollay to the lower-right, counting the number of contigous tokens for
  * a player.
  *
  * \param nPlayer  The player whose turn it is
  * \param y        The Y-coordinate
  * \param x        The X-coordinate
  *
  * \return The number of contiguous tokens counted
  */

int LinearGame::CheckDiagonalUpperLeftLowerRight(int nPlayer, int y, int x) const
{
    if (!cBoard.ValidLocation(x, y)) return 0;

    if (cBoard.PositionOccupiedByPlayer(x, y, nPlayer))
        return (1 + CheckDiagonalUpperLeftLowerRight(nPlayer, y+1, x+1));
    else
        return 0;
}

/**
  * Check for a number of contiguous tokens along upper-right to lower-left
  * diagnoal lines
  *
  * A recursive function that is given a pair of X- and Y-coordinates and moves
  * diagnollay to the lower-left, counting the number of contigous tokens for
  * a player.
  *
  * \param nPlayer  The player whose turn it is
  * \param y        The Y-coordinate
  * \param x        The X-coordinate
  *
  * \return The number of contiguous tokens counted
  */

int LinearGame::CheckDiagonalUpperRightLowerLeft(int nPlayer, int y, int x) const
{
    if (!cBoard.ValidLocation(x, y)) return 0;

    if (cBoard.PositionOccupiedByPlayer(x, y, nPlayer))
        return (1 + CheckDiagonalUpperRightLowerLeft(nPlayer, y+1, x-1));
    else
        return 0;
}

/**
  * Return a measure of the preference of a move.
  *
  * This method is called if the score of two moves are the same.  To provide
  * some variability in linear games, this method will return a random number.
  *
  * \param cGameMove  The GameMove to be evaluated.
  *
  * \return A random integer.
  */

int LinearGame::PreferredMove(const GameMove &cGameMove) const
{
    (void)cGameMove;

    // Random number generator
#if defined(_WIN32)
    static unsigned seed  = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    ++seed;
    static std::mt19937 RandomNumberGenerator{seed};
#else
    static std::random_device RandomDevice{};
    static std::mt19937 RandomNumberGenerator{RandomDevice()};
#endif // defined

    static std::uniform_int_distribution<int> Distribution(0,9999);

    return Distribution(RandomNumberGenerator);
}

/**
  * Check to see if a player has won the game.
  *
  * For a each player, evaluate the board for a contiguous line of
  * the player's tokens.  Uses m_knWin as the length of a contiguous
  * line to achieve a win.
  *
  * \param nPlayer The player
  *
  * \return True, if any player has won the game.  False otherwise.
  */

bool LinearGame::GameEnded(int nPlayer)
{
    // Clear win variables
    m_nWinner = 0;
    m_sWinBy.assign("nothing");

    if (BoardGame::GameEnded(nPlayer))
        return true;

    // Evaluate orthognal lines for player 1
    if (CheckOrthogonal(m_knPlayer1, m_knTokensInARowWin))
    {
        m_nWinner = m_knPlayer1;
        return true;
    }

    // Evaluate orthognal lines for player 2
    if (CheckOrthogonal(m_knPlayer2, m_knTokensInARowWin))
    {
        m_nWinner = m_knPlayer2;
        return true;
    }

    // Evaluate diagnoal lines for player 1
    if (CheckDiagonal(m_knPlayer1, m_knTokensInARowWin))
    {
        m_nWinner = m_knPlayer1;
        return true;
    }

    // Evaluate diagnoal lines for player 2
    if (CheckDiagonal(m_knPlayer2, m_knTokensInARowWin))
    {
        m_nWinner = m_knPlayer2;
        return true;
    }

    // Evaluate whether the player has any valid moves to make
    std::vector<GameMove> vGameMoves = GenerateMoves(nPlayer);
    if (vGameMoves.empty())
    {
        return true;
    }

    return false;
}
