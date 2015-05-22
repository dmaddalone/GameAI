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

/** \file
 *
 * \brief The ChessPiece class represents a chess piece.
 */

#ifndef CHESSIECE_H
#define CHESSPIECE_H

#include "GamePiece.h"

class ChessPiece : public GamePiece
{
    public:
        // Constructor with no arguments - blank or clear piece
        ChessPiece() :
            GamePiece(),
            m_nValue(m_knNoValue)
        {}

        // Constructor with token, player, and value number
        ChessPiece(char cToken, int nPlayer, int nValue) :
            GamePiece(cToken, nPlayer),
            m_nValue(nValue)
        {}

        // Deconstructor
        ~ChessPiece() {}

        // Return value
        int  Value() const        { return m_nValue; }
        // Return piece
        ChessPiece Piece() const   { return ChessPiece(m_cToken, m_nPlayer, m_nValue); }
        // Set game piece
        void Set(char cToken, int nPlayer, int nValue) { m_cToken = cToken; m_nPlayer = nPlayer; m_nValue = nValue; }
        // Clear a game piece
        void Clear() { GamePiece::Clear(); m_nValue = m_knNoValue; }

        bool HasMoved() { return m_bHasMoved; }
        void SetMoved() { m_bHasMoved = true; }

    protected:
        // Value number
        int  m_nValue;

    private:
        bool m_bHasMoved {false}; // TODO: For castling
};

#endif // GAMEPIECE_H
