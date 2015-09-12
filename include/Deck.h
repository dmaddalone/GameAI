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
 * \brief The Deck class represents a deck of playing cards.
 *
 */

#ifndef DECK_H
#define DECK_H

#include <algorithm>
#include <chrono>
#include <random>
#include <vector>

#include "Card.h"
#include "GameAIException.h"
#include "Hand.h"
#include "PlayingCards.h"

class Deck : public PlayingCards
{
    public:
        // Constructor
        Deck();

        // Destructor
        virtual ~Deck() {}

        // Shuffle the deck
        void Shuffle();

        // Deal the deck
        void Deal(int nNumberOfCardsPerHand, std::vector<Hand> &vHands, bool bDealCardsEqually=true);
};

class ProbableDeck : public Deck
{
    public:
        ProbableDeck() :
            Deck()
        {}

        void SetNumberOfCards(int n)    { m_nNumberOfCards = n; }
        void ReduceNumberOfCards(int n) { m_nNumberOfCards -= n; }
        int  NumberOfCards()            { return m_nNumberOfCards; }

        void UpdateRankProbabilities(ProbableHand &cProbableOpponentHand);

    private:
        int m_nNumberOfCards {0};
};

#endif // DECK_H
