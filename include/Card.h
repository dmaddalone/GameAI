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

const std::array<std::string, 13>  asRank      { { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" } };
const std::array<int, 13>          aiRankValue { {  2,   3,   4,   5,   6,   7,   8,   9,   10,   10,  10,  10,  11 } };
const std::array<int, 13>          aiSortValue { {  2,   3,   4,   5,   6,   7,   8,   9,   10,   11,  12,  13,  14 } };
const std::array<std::string, 4>   asSuit      { { "H", "D", "C", "S" } };


class Card
{
    public:
        // Standard constructor
        Card(std::string sRank, std::string sSuit, int nRankValue, int nSortValue) :
            m_sRank(sRank),
            m_sSuit(sSuit),
            m_nRankValue(nRankValue),
            m_nSortValue(nSortValue)
        {
            m_nID = ++m_nCardCount;
        }

        // Non standard constructor
        Card() :
            m_sRank(""),
            m_sSuit(""),
            m_nRankValue(0),
            m_nSortValue(0)
        {
            m_nID = 0;
        }

        // Get miscellaneous data about the card
        std::string Rank() const             { return m_sRank; }
        std::string Suit() const             { return std::string(1, static_cast<char>(::tolower(m_sSuit[0]))); }
        std::string DisplayRank(bool bForce=false) const { if (m_bTurnedUp || bForce) return m_sRank; else return m_sNotTurnedUp; }
        std::string DisplaySuit(bool bForce=false) const { if (m_bTurnedUp || bForce) return std::string(1, static_cast<char>(::tolower(m_sSuit[0]))); else return m_sNotTurnedUp; }
        std::string DisplayShortName(bool bForce=false) const { return DisplayRank(bForce) + DisplaySuit(bForce); }
        int         RankValue() const        { return m_nRankValue; }
        int         SortValue() const        { return m_nSortValue; }
        bool        SameRankAndSuit(const Card &cCard) { return cCard.Rank() == m_sRank && cCard.Suit() == m_sSuit; }

        // Set Rank and Suit
        bool        SetRank(std::string &sRank) { m_sRank = sRank; return RankValid(); }
        bool        RankValid() const          { for (const std::string &sRank : asRank) { if (sRank == m_sRank) { return true; } }  return false; }
        bool        SetSuit(std::string &sSuit) { m_sSuit = sSuit; return SuitValid(); }
        bool        SuitValid() const          { for (const std::string &sSuit : asSuit) { if (sSuit == m_sSuit) { return true; } }  return false; }
        bool        SetRankAndSuit(const std::string &sRankAndSuit);

        // Set Value
        void        SetValuesToLowest() { m_nRankValue = aiRankValue[0] - 1; m_nSortValue = aiSortValue[0] - 1; }

        // Set and get whether card is turned up
        void        TurnUp(bool b)    { m_bTurnedUp = b; }
        bool        TurnedUp() const  { return m_bTurnedUp; }

        // Overloaded operators for sorting by card sort value
        bool operator<(const Card &cCard) const { return SortValue() < cCard.SortValue(); }
        bool operator>(const Card &cCard) const { return SortValue() > cCard.SortValue(); }

        // Get the card ID
        int   ID() const { return m_nID; }

        // Used for Bayesian probabilities
        void  SetProbability(float fProb) { m_fProbability = fProb; }
        float Probability() const         { return m_fProbability; }

        // Set and get eligibility
        void SetEligibility(bool b) { m_bEligible = b; }
        bool Eligible() const       { return m_bEligible; }

        // Json object serialization and deserialization
        Json::Value JsonSerialization() const;
        bool        JsonDeserialization(const std::string &sJsonCard, std::string &sErrorMessage);
        bool        JsonDeserialization(const Json::Value &jCard);

    private:
        // Rank and Suit
        std::string m_sRank;
        std::string m_sSuit;
        int         m_nRankValue {0};
        int         m_nSortValue {0};

        // Whether the card is turned up
        mutable bool m_bTurnedUp { false };
        // If not turned up
        std::string m_sNotTurnedUp {"X"};

        // Used to create unique card identifier numbers
        static int m_nCardCount;

        // Set the card ID to zero
        int m_nID {0};

        // Used for Bayesian probabilities
        float m_fProbability {0.0};

        // Used to generically mark cards
        bool m_bEligible {false};
};

#endif // CARD_H
