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

#include "GameMove.h"

std::string GameMove::AnnounceToMove() const
{
    char c = 'a';
    std::string sMove;

    if (m_bNoMove)
    {
        sMove = "NO MOVE";
    }
    else if (m_bUseY)
    {
        sMove = static_cast<char>(m_nToX + c);
        sMove.append("," + std::to_string(m_nToY + 1));
    }
    else
    {
        sMove = static_cast<char>(m_nToX + c);
    }

    return sMove;
}

