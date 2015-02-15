/*
    Copyright 2014 Dom Maddalone

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
        GameMove(int nFromX=-1, int nFromY=-1, int nToX=-1, int nToY=-1, bool bAnnounceY=true) :
            m_nFromX(nFromX),
            m_nFromY(nFromY),
            m_nToX(nToX),
            m_nToY(nToY),
            m_bAnnounceY(bAnnounceY)
        {}

        virtual ~GameMove() {};

        void SetFromX(int nX) { m_nFromX = nX; }
        void SetFromY(int nY) { m_nFromX = nY; }
        void SetToX(int nX)   { m_nToX = nX; }
        void SetToY(int nY)   { m_nToY = nY; }

        int FromX() const     { return m_nFromX; }
        int FromY() const     { return m_nFromY; }
        int ToX()   const     { return m_nToX; }
        int ToY()   const     { return m_nToY; }

        bool ValidMove()      { return m_bValidMove; }

        bool SameTo(const GameMove &cGameMove) { if ((cGameMove.ToX() == m_nToX) && (cGameMove.ToY() == m_nToY)) return true; else return false;}

        std::string AnnounceToMove() const;

    private:
        int m_nFromX {-1};
        int m_nFromY {-1};
        int m_nToX   {-1};
        int m_nToY   {-1};

        bool m_bAnnounceY{true};

        bool m_bValidMove {false};
};

#endif // GAMEMOVE_H
