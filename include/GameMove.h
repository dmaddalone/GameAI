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
 * \brief The GameMove class represents a game move.
 *
 */

#ifndef GAMEMOVE_H
#define GAMEMOVE_H

#include <string>

class GameMove
{
    public:
        // Construct a GameMove
        GameMove() {}

        GameMove(int nFromX, int nFromY, int nToX, int nToY, bool bUseY) :
            m_nFromX(nFromX),
            m_nFromY(nFromY),
            m_nToX(nToX),
            m_nToY(nToY),
            m_bUseY(bUseY)
        {
            m_cFromX = m_nFromX + m_kcXOffset;
            m_cFromY = m_nFromY + m_kcYOffset;
            m_cToX   = m_nToX + m_kcXOffset;
            m_cToY   = m_nToY + m_kcYOffset;
        }

        // Destructor
        virtual ~GameMove() {};

        // Modify the coordinates of a move
        void SetToX(char cX)             { m_cToX = tolower(cX); m_nToX = m_cToX - m_kcXOffset; }
        void SetToX(char cX, bool bUseY) { SetToX(cX); m_bUseY = bUseY; }
        void SetToY(char cY)             { m_cToY = tolower(cY); m_nToY = m_cToY - m_kcYOffset; }
        void SetToY(int nY)              { m_nToY = nY; m_cToY = m_nToY + m_kcYOffset; }

        // Return the coordinates of a move
        int  FromX() const      { return m_nFromX; }
        int  FromY() const      { return m_nFromY; }
        int  ToX()   const      { return m_nToX; }
        int  ToY()   const      { return m_nToY; }
        char FromInputX() const { return m_cFromX; }
        char FromInputY() const { return m_cFromY; }
        char ToInputX() const   { return m_cToX; }
        char ToInputY() const   { return m_cToY; }

        // Set and return whether the Y-coordinate is used
        void SetUseY(bool b)   { m_bUseY = b; }
        bool UseY() const      { return m_bUseY; }

        // Set and return whether a move has been made
        void SetNoMove(bool b) { m_bNoMove = b; }
        bool NoMove()          { return m_bNoMove; }

        // Compare two moves and whether their to-moves are the same
        bool SameTo(const GameMove &cGameMove) { if ((cGameMove.ToX() == m_nToX) && (cGameMove.ToY() == m_nToY)) return true; else return false;}

        // Announce the moves
        std::string AnnounceFromMove() const;
        std::string AnnounceToMove() const;


    private:
        // Used to convert int coordinates to char values for internal representation using ASCII representation
        static const char m_kcXOffset {'a'};
        static const char m_kcYOffset {'1'};

        // Game move coordinates - initialize to garbage
        int  m_nFromX {-1};   // Start = 0
        char m_cFromX {'?'};  // Start = 'a'

        int  m_nFromY {-1};   // Start = 0
        char m_cFromY {'?'};  // Start = '1'

        int  m_nToX {-1};     // Start = 0
        char m_cToX {'?'};    // Start = 'a'

        int  m_nToY {-1};     // Start = 0
        char m_cToY {'?'};    // Start  ='1'

        // Whether the Y-coordinates are used
        bool m_bUseY {true};

        // Whether this is a move or not; used if no move is possible
        bool m_bNoMove {false};
};

#endif // GAMEMOVE_H
