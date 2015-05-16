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

#include <iostream>
#include <string>
#include <vector>

#if defined(_WIN32)
//#include <winsock2.h>
#include <windows.h>
//#include <wincon.h>
#endif

#include "GameMove.h"
#include "GamePiece.h"

class GameBoard
{
    public:
        // Construct a game board of size X & Y, and display characteristics
        GameBoard(int nX, int nY, bool bDisplayGrid, bool bDisplayXCoordinates, bool bDisplayYCoordinates) :
            m_vBoard(m_knMaxY, Row(m_knMaxX)),
            m_knX(nX),
            m_knY(nY),
            m_kbDisplayGrid(bDisplayGrid),
            m_kbDisplayXCoordinates(bDisplayXCoordinates),
            m_kbDisplayYCoordinates(bDisplayYCoordinates)
        {}

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
        // Evaluate locations on the board
        bool PositionOccupied(int nX, int nY) const;
        int  PositionOccupiedBy(int nX, int nY) const;
        bool PositionOccupiedByPlayer(int nX, int nY, int nPlayer) const;
        // Return the token for a board location
        char Token(int nX, int nY) const { return m_vBoard[nY][nX].Token(); }

        // Reverse token colors for players
        void ReverseColors() { std::string sColor = m_sPlayer1TokenColor; m_sPlayer1TokenColor = m_sPlayer2TokenColor; m_sPlayer2TokenColor = sColor; }
        // Reverse Y-Coordinates of the board
        void ReverseY()      { m_bReverseY = true; }

    private:
        // Max X-coordinate for any game
        static const int m_knMaxX {8};
        // Max Y-coordinate for any game
        static const int m_knMaxY {8};

        // Vector of vectors to represent the board
        typedef std::vector<GamePiece> Row;
        typedef std::vector<Row>       Board;
        Board m_vBoard;

        // Max X-coordinate for this game
        const int  m_knX;
        // Max Y-coordinate this game
        const int  m_knY;

        // Booleans to display game information
        const bool m_kbDisplayGrid;
        const bool m_kbDisplayXCoordinates;
        const bool m_kbDisplayYCoordinates;

        // Used to convert alpha X coordinates to integer values for internal representation
        const char m_kcXCoordinate {'a'};

        // Used to provide color for game pieces
#if defined(_WIN32)
        int m_sPlayer1TokenColor     {FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE}; // White
        int m_sPlayer2TokenColor     {FOREGROUND_RED};
        const int m_sResetTokenColor {FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE}; // White
#else
        std::string m_sPlayer1TokenColor       {"\033[1;37m"}; // White
        std::string m_sPlayer2TokenColor       {"\033[1;31m"}; // Red
        const std::string m_sResetTokenColor   {"\033[0m"};    // Reset
#endif

        bool m_bReverseY = false;
};

#endif // GAMEBOARD_H
