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

#include "Human.h"

bool Human::Move(Game &cGame)
{
    GameMove cGameMove;

    if (m_nVerbosity >= 1)
    {
        cGame.Display();
        std::cout << "Valid moves: ";
        cGame.DisplayValidMoves(m_nPlayerNumber);
    }

    do
    {
        // TODO: prompt player for move?  cGame.PromptPlayerForMove(m_nPlayerNumber);  OR not ?
        std::cout << "Player " << m_nPlayerNumber << ", enter move: ";

        cGameMove = cGame.GetMove(m_nPlayerNumber);

        if (!cGame.ApplyMove(m_nPlayerNumber, cGameMove))
        {
            std::cout << "Invalid move" << std::endl;
        }
        else
        {
            break;
        }

    } while(true);

    return true;
}
