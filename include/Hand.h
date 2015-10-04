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
 * \brief The Hand file declares several classes, including Hand,
 * ProbableHand, and Book.
 *
 */

#ifndef HAND_H
#define HAND_H

#include <algorithm>
#include <unordered_map>
#include <vector>

#include "Card.h"
#include "PlayingCards.h"

// Forward declarations
class Book;
class Match;

/** \class Hand
 *
 * \brief The Hand class represents a hand of playing cards.  The
 * ProbableHand class represents a probable hand of playing cards.
 *
 */

class Hand : public PlayingCards
{
    public:
        // Constructor
        Hand() : PlayingCards()
        {
            m_nID = ++m_nCount;
        };

        // Get information about the hand
        std::string DisplayCards(bool bForce=false) const;
        std::string DisplayRanks(bool bForce=false) const;

        // Remove books by rank from the hand
        Book RemoveBookByRank(int nCount);

        // Melding
        bool  MatchOpportunities(const int nCount, const bool bEvalSequence=true, const bool bEvalBook=true);
        Match RemoveMatch(std::vector<Card> &vCards, const int nCount, const bool bEvalSequence=true, const bool bEvalBook=true);

        // Layoff
        bool LayoffOpportunities(std::unordered_multimap<int, Match> &uommMatches, const bool bEvalSequence=true, const bool bEvalBook=true);
        bool RemoveLayoffs(std::unordered_multimap<int, Match> &uommMatches, Card &cCard, const bool bEvalSequence=true, const bool bEvalBook=true);

        // Discard
        void Discard(PlayingCards &cDiscardPile, const Card &cCard);

        // Sorting
        void SortByRank();
        void SortBySuit();

        // Return the ID of the hand
        int ID() const { return m_nID; }

    private:
        // Used to create unique hand identifier numbers
        static int m_nCount;

        // Set the card ID to zero
        int m_nID {0};
};

/** \class ProbableHand
 *
 * \brief The ProbableHand class represents a probable hand of playing cards.
 *
 */

class ProbableHand : public Hand
{
    public:
        // Constructor
        ProbableHand() :
            Hand()
        {}
};

/** \class Book
 *
 * \brief The Book class represents a book of playing cards.
 *
 */

class Book : public Hand
{
    public:
        // Constructor
        Book() :
            Hand()
        {}
};

// Used to identify the type of match
enum class MatchType
{
    TYPE_NONE,
    TYPE_SAME_RANK,
    TYPE_SAME_SUIT,
    TYPE_SEQUENCE
};

/** \class Match
 *
 * \brief The Match class represents a match of playing cards, which
 * includes a set of the same kind (rank or suit) or a sequence.
 *
 */

class Match : public Hand
{
    public:
        // Constructor
        Match() :
            Hand()
        {}

        // Set and get match type
        void SetType(MatchType ecMatchType) { m_ecMatchType = ecMatchType; }
        MatchType Type() const              { return m_ecMatchType; }

        // Eligible for a layoff
        void SetEligibility(bool b) { m_bEligibleMatch = b; }
        bool Eligible() const       { return m_bEligibleMatch; }

        // Json object (de)serialization
        Json::Value JsonSerialization() const;
        bool        JsonDeserialization(const std::string &sJsonMatch, std::string &sErrorMessage);
        bool        JsonDeserialization(const Json::Value &jMatch, std::string &sErrorMessage);

    private:
        MatchType m_ecMatchType    {MatchType::TYPE_NONE};
        bool      m_bEligibleMatch {false};
};

#endif // HAND_H

