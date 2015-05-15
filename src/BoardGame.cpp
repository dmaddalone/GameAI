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

#include "BoardGame.h"

/**
  * Display the game board.
  *
  * Run through every space on the board and display it's token.  Also display
  * coordinates and grid lines, depending on settings.
  */

void BoardGame::Display() const
{
    cBoard.Display();
}

/**
  * Return a string of valid moves.
  *
  * Call GenerateMoves and collect information into a string.
  *
  * \param nPlayer The player whose turn it is.
  *
  * \return A string of valid moves.
  */

std::string BoardGame::ValidMoves(int nPlayer) const
{
    std::string sValidMoves {};

    std::vector<GameMove> vGameMoves = GenerateMoves(nPlayer);

    if (vGameMoves.empty())
    {
        sValidMoves = "NO VALID MOVE";
    }
    else
    {
        for (GameMove cGameMove : vGameMoves)
        {
            sValidMoves += cGameMove.AnnounceToMove() + " ";
        }
    }

    return sValidMoves;
}

/**
  * Get the player's move.
  *
  * From std::cin, generate a GameMove object.
  *
  * \param nPlayer The player whose turn it is.
  *
  * \return A GameMove object.
  */

GameMove BoardGame::GetMove(int nPlayer) const
{
    (void)nPlayer;

    std::string sMove {};

    std::cin >> sMove;

    return GenerateMove(sMove);
}


/**
  * Return a measure of the preference of a move.
  *
  * This function is a NOP and should be overridden in derived classes.
  *
  * \param cGameMove  The GameMove to be evaluated.
  *
  * \return 0
  */

int BoardGame::PreferredMove(const GameMove &cGameMove) const
{
    (void)cGameMove;

    return 0;
}

/**
  * Return a  string providing a current score of the game.
  *
  * This function is a NOP and should be overridden in derived classes.
  *
  * \return ""
  */

std::string BoardGame::GameScore() const
{
    return "";
}

