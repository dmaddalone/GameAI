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

//#include <algorithm>
//#include <chrono>
//#include <random>
#include <vector>

#include "Card.h"

class Hand
{
    public:
        // Constructor
        Hand()
        {
            m_nID = ++m_nHandCount;
        }

        void AddCard(Card cCard);

        std::string DisplayCards() const;

        int ID() const { return m_nID; }

    private:
        std::vector<Card> m_vCards;

        // Used to create unique hand identifier numbers
        static int m_nHandCount;

        // Set the card ID to zero
        int m_nID {0};
};

#endif // HAND_H
