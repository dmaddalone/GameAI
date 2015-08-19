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

#include <vector>

#include "Card.h"

class PlayingCards
{
    public:
        // Constructor
        PlayingCards() {};

        ~PlayingCards() {};

        int  HasCards() const;
        bool HasRank(const std::string &sRank) const;
        bool HasSuit(const std::string &sSuit) const;
        int  HasCardsOfRank(const std::string &sRank) const;
        int  HasCardsOfSuit(const std::string &sSuit) const;

        void AddCard(Card &cCard);
        void AddCardToTop(Card &cCard);
        void AddCards(std::vector<Card> &vCards);

        Card              DrawTopCard();
        Card              PeekAtBottomCard() const;
        Card              RemoveCard(std::string sRank, std::string sSuit);
        std::vector<Card> RemoveCardsOfRank(std::string sRank);
        std::vector<Card> RemoveCardsOfSuit(std::string sSuit);

        // Json object serialization
        Json::Value JsonSerialization() const;
        bool        JsonDeserialization(const std::string &sJsonPlayingCards, std::string &sErrorMessage);

    protected:
        std::vector<Card> m_vCards {};
};

#endif // PLAYINGCARDS_H
