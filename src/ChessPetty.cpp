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

#include "ChessPetty.h"

/**
  * Set up the game board.
  *
  * Set initial positions of game pieces and initialize the Zobrist hash.
  */

void ChessPetty::SetBoard()
{
    GamePiece cPiece;

    ChessGame::SetBoard();

    cPiece.Set(m_kcQueenToken, 1, m_knQueenValue, m_knWhiteQueenIndex);
    cBoard.SetPiece(0, 0, cPiece);
    cPiece.Set(m_kcKingToken, 1, m_knKingValue, m_knWhiteKingIndex);
    cBoard.SetPiece(1, 0, cPiece);
    cPiece.Set(m_kcBishopToken, 1, m_knBishopValue, m_knWhiteBishopIndex);
    cBoard.SetPiece(2, 0, cPiece);
    cPiece.Set(m_kcKnightToken, 1, m_knKnightValue, m_knWhiteKnightIndex);
    cBoard.SetPiece(3, 0, cPiece);
    cPiece.Set(m_kcRookToken, 1, m_knRookValue, m_knWhiteRookIndex);
    cBoard.SetPiece(4, 0, cPiece);
    cPiece.Set(m_kcPawnToken, 1, m_knPawnValue, m_knWhitePawnIndex);
    cBoard.SetPiece(0, 1, cPiece);
    cBoard.SetPiece(1, 1, cPiece);
    cBoard.SetPiece(2, 1, cPiece);
    cBoard.SetPiece(3, 1, cPiece);
    cBoard.SetPiece(4, 1, cPiece);

    cPiece.Set(m_kcQueenToken, 2, m_knQueenValue, m_knBlackQueenIndex);
    cBoard.SetPiece(0, 5, cPiece);
    cPiece.Set(m_kcKingToken, 2, m_knKingValue, m_knBlackKingIndex);
    cBoard.SetPiece(1, 5, cPiece);
    cPiece.Set(m_kcBishopToken, 2, m_knBishopValue, m_knBlackBishopIndex);
    cBoard.SetPiece(2, 5, cPiece);
    cPiece.Set(m_kcKnightToken, 2, m_knKnightValue, m_knBlackKnightIndex);
    cBoard.SetPiece(3, 5, cPiece);
    cPiece.Set(m_kcRookToken, 2, m_knRookValue, m_knBlackRookIndex);
    cBoard.SetPiece(4, 5, cPiece);
    cPiece.Set(m_kcPawnToken, 2, m_knPawnValue, m_knBlackPawnIndex);
    cBoard.SetPiece(0, 4, cPiece);
    cBoard.SetPiece(1, 4, cPiece);
    cBoard.SetPiece(2, 4, cPiece);
    cBoard.SetPiece(3, 4, cPiece);
    cBoard.SetPiece(4, 4, cPiece);

    cBoard.InitializeZobrist();

    return;
}
