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

#include "CardGame.h"

/**
  * Display the cards.
  *
  * Display the cards in each player's hand.
  *
  */

void CardGame::Display() const
{
    for (const Hand &cHand : m_vHands)
    {
        std::cout << "P" << cHand.ID() << ": " << cHand.DisplayCards() << std::endl;
    }
}

/**
  * Return a string of valid moves.
  *
  * This is NOP.  Dervied classes may override.
  *
  * \param nPlayer The player whose turn it is.
  *
  * \return A string of valid moves.
  */

std::string CardGame::ValidMoves(int nPlayer) const
{
    (void) nPlayer;

    return "";
}

/**
  * Get the player's move.
  *
  * From std::cin, generate a GameMove object.  Provide a default move.
  *
  * \param nPlayer The player whose turn it is.
  *
  * \return A GameMove object.
  */

GameMove CardGame::GetMove(int nPlayer) const
{
    (void)nPlayer;

    static std::string sDefaultMove = DefaultMove();

    std::string sMove {};

    if (!sDefaultMove.empty())
    {
        std::cout << "[" << sDefaultMove << "] ";
        std::getline(std::cin, sMove);
        if (sMove.empty())
        {
            sMove = sDefaultMove;
        }
    }
    else
    {
        std::cin >> sMove; // (F)OLD, (D)RAW, (H)IT, (ST)AND, (DO)UBLE-DOWN, (SP)LIT, ...
    }

    return GenerateMove(sMove);
}

/**
  * Return a GameMove object from a string representing a game move.
  *
  * Create a GameMove object from the passed string.  Check for a resignation.
  *
  * \param sMove The string representing a game move.
  *
  * \return A GameMove object.
  */

GameMove CardGame::GenerateMove(std::string sMove) const
{
    // Generic GamevMove
    GameMove cGameMove(-1, -1, -1, -1, false, false);

    // Upper case the move string
    std::locale loc;
    for (std::string::size_type iii = 0; iii < sMove.length(); ++iii)
        sMove[iii] = std::toupper(sMove[iii], loc);

    // Compare move against resignation
    if (sMove.compare(GameVocabulary::RESIGN) == 0)
    {
        cGameMove.SetResignation(true);
        return cGameMove;
    }

    // Compare move against folding
    if (FoldingAllowed())
    {
        if (sMove.compare(GameVocabulary::FOLD) == 0)
        {
            cGameMove.SetFold(true);
            return cGameMove;
        }
    }

    // Compare move against drawing
    if (DrawingAllowed())
    {
        if (sMove.compare(GameVocabulary::DRAW) == 0)
        {
            cGameMove.SetDraw(true);
            return cGameMove;
        }
    }

    return cGameMove;
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

int CardGame::PreferredMove(const GameMove &cGameMove) const
{
    (void)cGameMove;

    return 0;
}

/**
  * Announce game move.
  *
  * This function is a NOP and should be overridden in derived classes.
  *
  * \param nPlayer   The player whose turn it is.
  * \param cGameMove The player's move
  *
  * \return A string containing the move.
  */

std::string CardGame::AnnounceMove(int nPlayer, const GameMove &cGameMove) const
{
    (void) nPlayer;
    (void) cGameMove;

    return "";
}

/**
  * Return a  string providing a current score of the game.
  *
  * This function is a NOP and should be overridden in derived classes.
  *
  * \return ""
  */

std::string CardGame::GameScore() const
{
    return "";
}

/**
  * Check to see if a player has won the game.
  *
  * If the last move made was a resignation, return true.
  *
  * \param nPlayer The player
  *
  * \return True, if any player has won the game.  False otherwise.
  */

bool CardGame::GameEnded(int nPlayer)
{
    if (!m_vGameMoves.empty())
    {
        GameMove cGameMove = m_vGameMoves.back();
        if (cGameMove.Resignation())
        {
            m_nWinner = nPlayer;
            m_sWinBy.assign("resignation");
            return true;
        }
    }

    return false;
}
