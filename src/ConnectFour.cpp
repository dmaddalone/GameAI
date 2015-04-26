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

#include "ConnectFour.h"

/**
  * Return a vector of valid game moves.
  *
  * Review and collect all valid moves for a player into a vector.  For
  * Connect Four, the nPlayer is ignored because valid moves are the same
  * between both players.
  *
  * \param nPlayer The player whose turn it is.
  *
  * \return A vector of valid moves.
  */

std::vector<GameMove> ConnectFour::GenerateMoves(int nPlayer) const
{
    (void)nPlayer;

    std::vector<GameMove> vGameMoves {};

    for (int xxx = 0; xxx < m_knX; ++xxx)
    {
        if (FindBottom(xxx) >= 0)
        {
            //vGameMoves.emplace_back(-1,-1,xxx,-1,false);
            //vGameMoves.emplace_back(xxx,false);
            //vGameMoves.emplace_back(xxx,m_kc false);
            vGameMoves.emplace_back(0, 0, xxx, 0, m_kcXCoordinate, false);
        }
    }

    return vGameMoves;
}


/**
  * Get the player's next move.
  *
  * Using std::cin, capture the player's move, create a GameMove object, and
  * return it.
  *
  * \param nPlayer The player whose turn it is.
  *
  * \return A GameMove object.
  */

GameMove ConnectFour::GetMove(int nPlayer) const
{
    (void)nPlayer;

    //char     cMove {};
    std::string  sMove {};
    //GameMove cGameMove;

    //std::cin >> cMove;
    std::cin >> sMove;

    //return GenerateMove(cMove);
    return GenerateMove(sMove);
    /*

    //cGameMove.SetToX(cMove - m_kcXCoordinate);
    cGameMove.SetToX(cMove, m_kcXCoordinate);

    return cGameMove;
    */
}

/*
* TODO
*/
GameMove ConnectFour::GenerateMove(std::string sMove) const
{
    GameMove cGameMove;

    cGameMove.SetToX(sMove[0], m_kcXCoordinate);

    return cGameMove;
}

/**
  * Apply the move to the game.
  *
  * Find the bottom of the grid for the selected move, update the GameMove
  * object with that location, and pass it to LinearGame for application.
  *
  * \param nPlayer The player whose turn it is.
  * \param cGameMove The game move
  *
  * \return True if the move is valid, false otherwise
  */

bool ConnectFour::ApplyMove(int nPlayer, GameMove &cGameMove)
{
    cGameMove.SetToY(FindBottom(cGameMove.ToX()));

    return LinearGame::ApplyMove(nPlayer, cGameMove);
}

/**
  * Find the bottom of the Connect Four grid.
  *
  * For the selected Connect Four vertical slot (X-coordinate), review all locations beginning
  * from the bottom, and stopping when a location is clear (no piece exists in
  * that location.)
  *
  * \param x The slot (X-coordinate) of the Conect Four grid.
  *
  * \return The Y-coordinate of the clear location for the selected slot.
  */

int ConnectFour::FindBottom(int x) const
{
    for (int yyy = m_knY - 1; yyy >= 0; --yyy)
    {
        if (m_acGrid[yyy][x] == m_kcClear)
            return yyy;
    }

    return -1;
}

/**
  * Return a preferred move.
  *
  * Prefer a move located near the center of the grid.
  *
  * \param cGameMove The game move.
  *
  * \return An absolute distance from the center slot of the Connect Four grid.
  */

int ConnectFour::PreferredMove(const GameMove &cGameMove) const
{
    return std::abs(cGameMove.ToX() - 3);
}
