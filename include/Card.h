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

#include <array>
#include <string>

#include "json/json.h"

const std::array<std::string, 13>  asRank  { { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" } };
const std::array<std::string, 4>   asSuit  { { "h", "d", "c", "s" } };
const std::array<int, 13>          aiValue { {  2,   3,   4,   5,   6,   7,   8,   9,   10,   11,  12,  13,  14 } };

class Card
{
    public:
        // Standard constructor
        Card(std::string sRank, std::string sSuit, int nValue) :
            m_sRank(sRank),
            m_sSuit(sSuit),
            m_nValue(nValue)
        {
            m_nID = ++m_nCardCount;
        }

        // Non standard constructor
        Card() :
            m_sRank(""),
            m_sSuit(""),
            m_nValue(0)
        {
            m_nID = 0;
        }

        //virtual ~Card() {}

        std::string Suit() const             { return m_sSuit; }
        std::string Rank() const             { return m_sRank; }
        std::string DisplaySuit() const      { if (m_bTurnedUp) return m_sSuit; else return m_sNotTurnedUp; }
        std::string DisplayRank() const      { if (m_bTurnedUp) return m_sRank; else return m_sNotTurnedUp; }
        std::string DisplayShortName() const { return DisplayRank() + DisplaySuit(); }
        int         Value() const            { return m_nValue; }

        bool        SetRank(std::string sRank) { m_sRank = sRank; return RankValid(); }
        bool        RankValid()       { for (const std::string &sRank : asRank) { if (sRank == m_sRank) { return true; } }  return false; }

        void        TurnUp(bool b)    { m_bTurnedUp = b; }
        bool        TurnedUp() const  { return m_bTurnedUp; }

        bool operator<(const Card &cCard) const { return Value() < cCard.Value(); }
        bool operator>(const Card &cCard) const { return Value() > cCard.Value(); }

        int         ID() const        { return m_nID; }

        // Used for Bayesian probabilities
        void  SetProbability(float fProb) { m_fProbability = fProb; }
        float Probability()               { return m_fProbability; }

        // Json object serialization and deserialization
        Json::Value JsonSerialization() const;
        bool        JsonDeserialization(const std::string &sJsonCard, std::string &sErrorMessage);

    private:
        // Rank and Suit
        std::string m_sRank;
        std::string m_sSuit;
        int         m_nValue {0};

        mutable bool m_bTurnedUp { false };

        // Used to create unique card identifier numbers
        static int m_nCardCount;

        // Set the card ID to zero
        int m_nID {0};

        // If not turned up
        std::string m_sNotTurnedUp {"X"};

        // Used for Bayesian probabilities
        float m_fProbability {0.0};
};

#endif // CARD_H
