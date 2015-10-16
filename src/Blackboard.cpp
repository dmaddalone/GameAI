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

#include "Blackboard.h"

/**
  * Update asks.
  *
  * Update the number of times this card rank has been asked for.
  *
  * \param sRank The card rank being asked for.
  */

void Blackboard::UpdateAsks(const std::string &sRank)
{
    int nAsks {1};

    // Find the rank
    std::map<std::string, int>::iterator it = m_mAsks.find(sRank);

    // If found, update
    if (it != m_mAsks.end())
    {
        nAsks = it->second;
        m_mAsks.erase(sRank);
        m_mAsks.insert(std::pair<std::string, int>(sRank, ++nAsks));
    }
    // Else insert
    else
    {
        m_mAsks.insert(std::pair<std::string, int>(sRank, nAsks));
    }
}

/**
  * Find the number of asks for a rank.
  *
  * \param sRank The card rank being asked for.
  *
  * \return The number of times this rank has been asked for
  */

int Blackboard::Asks(const std::string &sRank) const
{
    // Find number of asks for this rank
    std::map<std::string, int>::const_iterator it = m_mAsks.find(sRank);

    // If found, return number of Asks
    if (it != m_mAsks.end())
    {
        return it->second;
    }
    else
    {
        return 0;
    }
}
/**
  * Evaluate whether a card of rank and suit has been recently discarded.
  *
  * \param sRankAndSuit The card rank and suit being evaluated
  * \param nCards       The number of cards to evaluate
  *
  * \return True if rank and suit found within nCards, false otherwise
  */

bool Blackboard::RecentlyDiscarded(const std::string&sRankAndSuit, const int nCards) const
{
    std::vector<std::string>::const_reverse_iterator rit = m_vDiscards.rbegin();
    int nCount {0};

    while (rit != m_vDiscards.rend() && nCount < nCards)
    {
        if (sRankAndSuit.compare(*rit) == 0)
            return true;

        ++rit;
        ++nCount;
    }

    return false;
}
