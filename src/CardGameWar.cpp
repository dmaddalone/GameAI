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

#include "CardGameWar.h"

/**
  * Return a vector of valid game moves.
  *
  * For War, the only move is to draw, if the player has cards in their hand.
  *
  * \param nPlayer The player whose turn it is.
  *
  * \return A vector of valid moves.
  */

std::vector<GameMove> CardGameWar::GenerateMoves(int nPlayer) const
{
    std::vector<GameMove> vGameMoves {};

    if (m_vHands[nPlayer].HasCards() > 0)
    {
        GameMove cGameMove;
        cGameMove.SetDraw(true);
        vGameMoves.push_back(cGameMove);
    }

    return vGameMoves;
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

bool CardGameWar::ApplyMove(int nPlayer, GameMove &cGameMove)
{/*
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

    return true;*/
}




