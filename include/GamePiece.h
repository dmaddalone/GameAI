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
 * \brief The GamePiece class represents a game piece.
 */

#ifndef GAMEPIECE_H
#define GAMEPIECE_H

class GamePiece
{
    public:
        // Constructor with no arguments - blank or clear piece
        GamePiece() :
            m_cToken(m_kcNoToken),
            m_nPlayer(m_knNoValue),
            m_nValue(m_knNoValue),
            m_nNumber(m_knNoValue)
        {}

        // Constructor with token and player number
        GamePiece(char cToken, int nPlayer) :
            m_cToken(cToken),
            m_nPlayer(nPlayer),
            m_nValue(m_knNoValue),
            m_nNumber(m_knNoValue)
        {}

        // Constructor with token, player number, and value
        GamePiece(char cToken, int nPlayer, int nValue) :
            m_cToken(cToken),
            m_nPlayer(nPlayer),
            m_nValue(nValue),
            m_nNumber(m_knNoValue)
        {}

        // Constructor with token, player number, value, and number
        GamePiece(char cToken, int nPlayer, int nValue, int nNumber) :
            m_cToken(cToken),
            m_nPlayer(nPlayer),
            m_nValue(nValue),
            m_nNumber(nNumber)
        {}

        // Deconstructor
        ~GamePiece() {}

        // Return token
        char Token() const    { return m_cToken; }
        // Return player number
        int  Player() const   { return m_nPlayer; }
        // Return value
        int  Value() const    { return m_nValue; }
        // Return general number
        int  Number() const    { return m_nNumber; }
        // Return piece
        const GamePiece & Piece() const { return *this; }
        // Evaluate whether piece is a game pice (vs a non-game piece or clear)
        bool IsAGamePiece() const { return m_cToken != m_kcNoToken; }

        // Set game piece
        void Set(char cToken, int nPlayer)
            { m_cToken = cToken; m_nPlayer = nPlayer; m_nValue = m_knNoValue; m_nNumber = m_knNoValue; }
        void Set(char cToken, int nPlayer, int nValue)
            { m_cToken = cToken; m_nPlayer = nPlayer; m_nValue = nValue; m_nNumber = m_knNoValue; }
        void Set(char cToken, int nPlayer, int nValue, int nNumber)
            { m_cToken = cToken; m_nPlayer = nPlayer; m_nValue = nValue; m_nNumber = nNumber; }

        // Clear a game piece
        void Clear()                       { m_cToken = m_kcNoToken; m_nPlayer = m_knNoValue; m_nValue = m_knNoValue; m_bHasMoved = false; }

        // Flag for piece movement
        bool HasMoved() const              { return m_bHasMoved; }
        void SetMoved(bool bMoved=true)    { m_bHasMoved = bMoved; }

    private:
        // Constants for clear pieces
        static const char m_kcNoToken {32};
        static const int  m_knNoValue {-1};

        // Board representation of a piece, for example 'X' and 'O' for tic-tac-toe
        char m_cToken;
        // Player number
        int  m_nPlayer;
        // Value number
        int  m_nValue;
        // General number
        int m_nNumber;
        // Flag for whether the piece has moved
        bool m_bHasMoved {false};
};

#endif // GAMEPIECE_H
