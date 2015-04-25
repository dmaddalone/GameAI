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
        // Construct a GameMove // TODO needed?
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
        //void SetFromX(int nX)  { m_nFromX = nX; }
        void SetFromX(char cX, char cOffset) { m_cFromX = tolower(cX); m_nFromX = m_cFromX - cOffset; }
        //void SetFromY(int nY)  { m_nFromX = nY; }
        void SetFromY(char cY)  { m_cFromY = cY; std::string s(1, m_cFromY); m_nFromY = std::stoi(s, nullptr) - 1; }
        //void SetToX(int nX)    { m_nToX = nX; }
        void SetToX(char cX, char cOffset) { m_cToX = tolower(cX); m_nToX = m_cToX - cOffset; }
        void SetToY(int nY)   { m_nToY = nY; m_cToY = ' ';}
        void SetToY(char cY)  { m_cToY = cY; std::string s(1, m_cToY); m_nToY = std::stoi(s, nullptr) - 1; }

        // Return the coordinates of a move
        int  FromX() const      { return m_nFromX; }
        char FromInputX() const { return m_cFromX; }
        int  FromY() const      { return m_nFromY; }
        char FromInputY() const { return m_cFromY; }
        int  ToX()   const      { return m_nToX; }
        char ToInputX() const   { return m_cToX; }
        int  ToY()   const      { return m_nToY; }
        char ToInputY() const   { return m_cToY; }

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
        int  m_nFromX;
        char m_cFromX;

        int  m_nFromY;
        char m_cFromY;

        int  m_nToX;
        char m_cToX;

        int  m_nToY;
        char m_cToY;

        // Whether the Y-coordinates are used
        bool m_bUseY;

        // Whether this is a move or not; used if no move is possible
        bool m_bNoMove {false};
};

#endif // GAMEMOVE_H
