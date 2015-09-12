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

#include "Human.h"

/**
  * Generate the next game move.
  *
  * Use GameMove to collect the next game move from a human player.
  *
  * \param cGame The game.
  *
  * \return True, if a move has been made or no move to make.
  */

bool Human::Move(Game &cGame)
{
    GameMove cGameMove;

    // If game requires synchronization, pass turn to next player to synchronize information
    if (cGame.Sync())
    {
        return true;
    }

    // If Another Turn is set for opponent, skip this player's turn
    cGameMove = cGame.LastMove();
    if (cGameMove.AnotherTurn() && (cGameMove.PlayerNumber() != m_nPlayerNumber))
    {
        return true;
    }

    // Show valid moves at proper logging level.
    if (m_cLogger.Level() >= 1)
    {
        cGame.Display();
        std::string sMoves = cGame.ValidMoves(m_nPlayerNumber);
        if (sMoves.size() > 0)
        {
            std::cout << "Valid moves: " << cGame.ValidMoves(m_nPlayerNumber) << std::endl;
            //TODO: change ValidMoves to use vector of moves as parameter (from Generate Moves)
        }
    }

    // Generate all possible valid moves for this player
    std::vector<GameMove> vGameMoves = cGame.GenerateMoves(m_nPlayerNumber);

    // If no valid moves are possible, return true
    if (vGameMoves.empty())
    {
        return true;
    }

    do
    {
        std::cout << "Player " << m_nPlayerNumber << ", enter move: ";

        // Get move from human player.
        cGameMove = cGame.GetMove(m_nPlayerNumber);

        // Test move for validity.
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
