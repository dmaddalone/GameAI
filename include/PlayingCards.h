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
 * \brief The PlayingCards class represents playing cards.
 *
 */

#ifndef PLAYINGCARDS_H
#define PLAYINGCARDS_H

#include <algorithm>
#include <vector>

#include "Card.h"

class PlayingCards
{
    public:
        // Constructor
        PlayingCards() {};

        // Destructor
        virtual ~PlayingCards() {};

        // Return copy cards
        std::vector<Card> Cards() const { return m_vCards; }

        // Return miscellaneous information on the playing cards
        int  HasCards() const;
        bool HasCard(const Card &cCardInQuestion) const;
        bool HasRank(const std::string &sRank) const;
        bool HasSuit(const std::string &sSuit) const;
        int  HasCardsOfRank(const std::string &sRank) const;
        int  HasCardsOfSuit(const std::string &sSuit) const;

        // Return a string of cards
        std::string Ranks() const;
        std::string RanksAndSuits() const;

        // Add cards to playing cards
        void AddCard(const Card &cCard);
        void AddCardToTop(const Card &cCard);
        void AddCards(const std::vector<Card> &vCards);

        // Return a card or a vector of cards
        Card              DrawTopCard();
        Card              PeekAtTopCard() const;
        Card              PeekAtBottomCard() const;
        Card              RemoveCard(const std::string &sRank, const std::string &sSuit);
        Card              RemoveCard(const Card &cCard);
        std::vector<Card> RemoveCardsOfRank(const std::string &sRank, int nNumberToRemove=0);
        std::vector<Card> RemoveCardsOfSuit(const std::string &sSuit, int nNumberToRemove=0);
        std::vector<Card> RemoveCards(const std::vector<Card> &vCardsToRemove);
        std::vector<Card> RemoveAllCards();

        // Json object serialization
        Json::Value JsonSerialization() const;
        bool        JsonDeserialization(const std::string &sJsonPlayingCards, std::string &sErrorMessage);
        bool        JsonDeserialization(const Json::Value jCards, std::string &sErrorMessage);

        // Setting and getting the nominal number of cards; used for probabilities
        void SetNumberOfCards(int n)      { m_nNumberOfCards = n; }
        //void ReduceNumberOfCards(int n)   { m_nNumberOfCards -= n; }
        //void IncreaseNumberOfCards(int n) { m_nNumberOfCards += n; }
        int  NumberOfCards()              { return m_nNumberOfCards; }

        // Update probabilities that the ranks exist
        void UpdateRankProbabilities(int nOtherCards);
        void SortByProbability();

    protected:
        // The playing cards
        std::vector<Card> m_vCards {};

        // The nominal number of cards held in playing cards
        int m_nNumberOfCards {0};
};

#endif // PLAYINGCARDS_H
