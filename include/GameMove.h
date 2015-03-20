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
        GameMove(int nFromX=-1, int nFromY=-1, int nToX=-1, int nToY=-1, bool bUseY=true) :
            m_nFromX(nFromX),
            m_nFromY(nFromY),
            m_nToX(nToX),
            m_nToY(nToY),
            m_bUseY(bUseY)
        {}

        // Destructor
        virtual ~GameMove() {};

        // Modify the coordinates of a move
        void SetFromX(int nX)  { m_nFromX = nX; }
        void SetFromY(int nY)  { m_nFromX = nY; }
        void SetToX(int nX)    { m_nToX = nX; }
        void SetToY(int nY)    { m_nToY = nY; }

        // Return the coordinates of a move
        int FromX() const      { return m_nFromX; }
        int FromY() const      { return m_nFromY; }
        int ToX()   const      { return m_nToX; }
        int ToY()   const      { return m_nToY; }

        // Return whether the Y-coordinate is used
        bool UseY() const      { return m_bUseY; }

        // Set and return whether a move has been made
        void SetNoMove(bool b) { m_bNoMove = b; }
        bool NoMove()          { return m_bNoMove; }

        // Compare two moves and whether their to-moves are the same
        bool SameTo(const GameMove &cGameMove) { if ((cGameMove.ToX() == m_nToX) && (cGameMove.ToY() == m_nToY)) return true; else return false;}

        // Announce the to-move
        std::string AnnounceToMove() const;

    private:
        // Game move coordinates
        int m_nFromX;
        int m_nFromY;
        int m_nToX;
        int m_nToY;

        // Whether the Y-coordinates are used
        bool m_bUseY;

        // Whether this is a move or not; used if no move is possible
        bool m_bNoMove {false};
};

#endif // GAMEMOVE_H
