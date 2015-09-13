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
 * \brief The Hand class represents a hand of playing cards.
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

        // Deconstructor
        ~Hand() {};

        std::string Ranks() const;
        std::string DisplayCards() const;
        std::string DisplayRanks() const;

        Hand RemoveBookByRank(int nCount);

        void SortByRank();

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
        ProbableHand() :
            Hand()
        {}

        void SetNumberOfCards(int n)    { m_nNumberOfCards = n; }
        void ReduceNumberOfCards(int n) { m_nNumberOfCards -= n; }
        void AddNumberOfCards(int n)    { m_nNumberOfCards += n; }
        int  NumberOfCards()            { return m_nNumberOfCards; }

    private:
        int m_nNumberOfCards {0};

        //void UpdateRankProbabilities(Hand &cProbableOpponentHand);
};

#endif // HAND_H
