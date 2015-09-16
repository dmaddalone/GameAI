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

/** \file
 *
 * \brief The Hand class represents a hand of playing cards.  The
 * ProbableHand class represents a probable hand of playing cards.
 *
 */

#ifndef HAND_H
#define HAND_H

#include <algorithm>
#include <vector>

#include "Card.h"
#include "PlayingCards.h"

class Hand : public PlayingCards
{
    public:
        // Constructor
        Hand() : PlayingCards()
        {
            m_nID = ++m_nCount;
        };

        // Get information about the hand
        std::string Ranks() const;
        std::string DisplayCards() const;
        std::string DisplayRanks() const;

        // Remove books by rank from the hand
        Hand RemoveBookByRank(int nCount);

        // Sort the hand by rank
        void SortByRank();

        // Return the ID of the hand
        int ID() const { return m_nID; }

    private:
        // Used to create unique hand identifier numbers
        static int m_nCount;

        // Set the card ID to zero
        int m_nID {0};
};

class ProbableHand : public Hand
{
    public:
        // Constructor
        ProbableHand() :
            Hand()
        {}
};

#endif // HAND_H
