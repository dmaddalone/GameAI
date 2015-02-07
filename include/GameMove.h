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

class GameMove
{
    public:
        GameMove(int nFromX=0, int nFromY=0, int nToX=0, int nToY=0) :
            m_nFromX(nFromX),
            m_nFromY(nFromY),
            m_nToX(nToX),
            m_nToY(nToY)
        {}
        virtual ~GameMove() {};

        void SetFromX(int nX) { m_nFromX = nX; }
        void SetFromY(int nY) { m_nFromX = nY; }
        void SetToX(int nX)   { m_nToX = nX; }
        void SetToY(int nY)   { m_nToY = nY; }

        int FromX() const { return m_nFromX; }
        int FromY() const { return m_nFromY; }
        int ToX()   const { return m_nToX; }
        int ToY()   const { return m_nToY; }

        bool ValidMove() { return m_bValidMove; }

    private:
        int m_nFromX {0};
        int m_nFromY {0};
        int m_nToX   {0};
        int m_nToY   {0};

        bool m_bValidMove {false};
};

#endif // GAMEMOVE_H
