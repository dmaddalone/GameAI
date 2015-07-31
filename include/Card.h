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
 * \brief The Card class represents a playing card.
 *
 */

#ifndef CARD_H
#define CARD_H

#include <string>

const std::string asRank[]  { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
const std::string asSuit[]  { "h", "d", "c", "s" };
const int         aiValue[] {  2,   3,   4,   5,   6,   7,   8,   9,   10,   11,  12,  13,  14 };

class Card
{
    public:
        Card(std::string sRank, std::string sSuit, int nValue) :
            m_sRank(sRank),
            m_sSuit(sSuit),
            m_nValue(nValue)
        {
            m_nID = ++m_nCardCount;
        }

        std::string Suit() const { return m_sSuit; }
        std::string Rank() const { return m_sRank; }
        std::string ShortName() const { return m_sRank + m_sSuit; }
        void TurnUp()            { m_bTurnedUp = true; }
        bool TurnedUp()    const { return m_bTurnedUp; }
        int ID()           const { return m_nID; }

    private:
        // Rank and Suit
        std::string m_sRank;
        std::string m_sSuit;
        int         m_nValue {0};

        bool m_bTurnedUp { false };

        // Used to create unique card identifier numbers
        static int m_nCardCount;

        // Set the card ID to zero
        int m_nID {0};
};

#endif // CARD_H
