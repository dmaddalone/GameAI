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
 * \brief The GameBoard class represents a game board for two players.  The
 * game board is a multi-dimensional vector, loaded with game pieces.
 *
 */

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#if defined(_WIN32)
#include <windows.h>
#endif

#include "GameMove.h"
#include "GamePiece.h"
#include "GameAIException.h"

class GameBoard
{
    public:
        // Construct a game board of size X & Y, and display characteristics
        GameBoard(int nX, int nY, int nNumberOfTypesOfPieces,
                  bool bDisplayGrid, bool bDisplayXCoordinates, bool bDisplayYCoordinates) :
            m_vBoard(m_knMaxY, Row(m_knMaxX)),
            m_knX(nX),
            m_knY(nY),
            m_knNumberOfTypesOfPieces(nNumberOfTypesOfPieces),
            m_kbDisplayGrid(bDisplayGrid),
            m_kbDisplayXCoordinates(bDisplayXCoordinates),
            m_kbDisplayYCoordinates(bDisplayYCoordinates)
        {
            // Ensure X- and Y-coordinates do not exceed maximum board sizes
            if (m_knX > m_knMaxX)
            {
                std::string sErrorMessage = "X-coordinate " + std::to_string(m_knX) + " exceeds max X-coordinate " + std::to_string(m_knMaxX);
                throw GameAIException(sErrorMessage);
            }
            if (m_knY > m_knMaxY)
            {
                std::string sErrorMessage = "Y-coordinate " + std::to_string(m_knY) + " exceeds max Y-coordinate " + std::to_string(m_knMaxY);
                throw GameAIException(sErrorMessage);
            }

            // Ensure that the number of types of pieces does not exceed the maximum number
            if (m_knNumberOfTypesOfPieces > m_knMaxNumberOfTypesOfPieces)
            {
                std::string sErrorMessage = "Number of game pieces " + std::to_string(m_knNumberOfTypesOfPieces) + " exceeds max number of game pieces " + std::to_string(m_knMaxNumberOfTypesOfPieces);
                throw GameAIException(sErrorMessage);
            }
        }

        // Destructor
        ~GameBoard() {}

        // Evaluate location validity
        bool ValidLocation(int nX, int nY) const;
        // Clear the board
        void Clear();
        // Display the board
        void Display() const;
        // Set a piece on the board
        bool SetPiece(int nX, int nY, const GamePiece &cGamePiece);
        // Move a piece on the board
        bool MovePiece(const GameMove &cGameMove);
        // Return the piece at the location coordinates
        const GamePiece & Piece(int nX, int nY) const { return m_vBoard[nY][nX].Piece(); }
        // Evaluate locations on the board
        bool PositionOccupied(int nX, int nY) const;
        int  PositionOccupiedBy(int nX, int nY) const;
        bool PositionOccupiedByPlayer(int nX, int nY, int nPlayer) const;
        // Return the token for a board location
        char Token(int nX, int nY) const { return m_vBoard[nY][nX].Token(); }
        // Return the player for a board location
        char Player(int nX, int nY) const { return m_vBoard[nY][nX].Player(); }
        // Return the value of the piece
        int PieceValue(int nX, int nY) const { return m_vBoard[nY][nX].Value();}
        // Return the general number of the piece
        int PieceNumber(int nX, int nY) const { return m_vBoard[nY][nX].Number();}

        // Initialize the Zobrist key
        void InitializeZobrist();
        // Update the Zobrist key
        void UpdateZobristKey(int nPX, int nPY, int nSX, int nSY);
        // Return the Zobrist key
        uint64_t ZKey() const { return m_uiZobristKey; }

        // Reverse token colors for players
#if defined(_WIN32)
        void ReverseColors() { int nColor = m_nPlayer1TokenColor; m_nPlayer1TokenColor = m_nPlayer2TokenColor; m_nPlayer2TokenColor = nColor; }
#else
        void ReverseColors() { std::string sColor = m_sPlayer1TokenColor; m_sPlayer1TokenColor = m_sPlayer2TokenColor; m_sPlayer2TokenColor = sColor; }
#endif

        // Reverse Y-Coordinates of the board
        void ReverseY()      { m_bReverseY = true; }

    protected:
        // Vector of vectors to represent the board
        typedef std::vector<GamePiece> Row;
        typedef std::vector<Row>       Board;
        Board m_vBoard;

    private:
        // Max X-coordinate for any game
        static const int m_knMaxX {8};
        // Max Y-coordinate for any game
        static const int m_knMaxY {8};

        // Max X-coordinate for this game
        const int  m_knX;
        // Max Y-coordinate this game
        const int  m_knY;

        // Max number of types of pieces allowed
        static const int m_knMaxNumberOfTypesOfPieces {12};

        // Number of types of pieces for this game
        const int m_knNumberOfTypesOfPieces;

        // Booleans to display game information
        const bool m_kbDisplayGrid;
        const bool m_kbDisplayXCoordinates;
        const bool m_kbDisplayYCoordinates;

        // Used to convert alpha X coordinates to integer values for internal representation
        const char m_kcXCoordinate {'a'};

        // Used to provide color for game pieces
#if defined(_WIN32)
        int m_nPlayer1TokenColor      {FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE}; // White
        int m_nPlayer2TokenColor      {FOREGROUND_INTENSITY | FOREGROUND_RED};
        const int m_knResetTokenColor {FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE}; // White
#else
        std::string m_sPlayer1TokenColor       {"\033[1;37m"}; // White
        std::string m_sPlayer2TokenColor       {"\033[1;31m"}; // Red
        const std::string m_ksResetTokenColor  {"\033[0m"};    // Reset
#endif

        // Flag for reversing the Y-Coordinates
        bool m_bReverseY = false;

        // Number of squares on the game board
        static const int m_knNumberOfSquares {m_knMaxX * m_knMaxY};

        // Array representing the number of types of positions on each square, assigned to a random number
        // used to create the Zobrist hash
        uint64_t m_auiZobrist[m_knMaxNumberOfTypesOfPieces][m_knMaxX * m_knMaxY] {{}};

        // The Zobrist hash, representing the position of the board
        uint64_t m_uiZobristKey;
};

#endif // GAMEBOARD_H
