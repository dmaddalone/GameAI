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

#include "GameMove.h"

/**
  * Announce the from-move.
  *
  * Construct a string containing the to-move.
  *
  * \return A string containing the to-move.
  */

std::string GameMove::AnnounceFromMove() const
{
    std::string sMove {};

    if (!m_bUseFrom)
        return sMove;

    if (m_bUseY)
    {
        sMove = m_cFromX;
        sMove += m_cFromY;
    }
    else
    {
        sMove = m_cFromX;
    }

    return sMove;
}

/**
  * Announce the to-move.
  *
  * Construct a string containing the to-move.
  *
  * \return A string containing the to-move.
  */

std::string GameMove::AnnounceToMove() const
{
    std::string sMove;

    if (m_bNoMove)
    {
        sMove = "NO MOVE";
    }
    else if (m_bResignation)
    {
        sMove = "RESIGN";
    }
    else if (m_bUseY)
    {
        sMove = m_cToX;
        sMove += m_cToY;
    }
    else
    {
        sMove = m_cToX;
    }

    return sMove;
}

/**
  * Announce the card object.
  *
  * Construct a string containing the card object.
  *
  * \return A string containing the card rank and suit.
  */

std::string GameMove::AnnounceCard() const
{
    return m_cCard.DisplayShortName();
}

/**
  * Announce the card object - Rank only.
  *
  * Construct a string containing the card object.
  *
  * \return A string containing the card rank.
  */

std::string GameMove::AnnounceCardRank() const
{
    return m_cCard.Rank();
}

