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

#include "Deck.h"

/**
  * Initialize the deck
  *
  * Create a standard deck of 52 playing cards.
  */

void Deck::Initialize()
{
    // Create standard 52 card deck
    m_vCards.clear();

    for (std::size_t iii = 0; iii < asRank.size(); ++iii)
    {
        for (std::size_t jjj = 0; jjj < asSuit.size(); ++jjj)
        {
            m_vCards.emplace_back(asRank[iii], asSuit[jjj], aiRankValue[iii], aiSortValue[iii]);
        }
    }
}

/**
  * Shuffle the deck.
  *
  * Use std::shuffle and a random number to mix the cards.
  *
  */

void Deck::Shuffle()
{
    // Random number generator
#if defined(_WIN32)
    static unsigned seed  = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    ++seed;
    std::mt19937 RandomNumberGenerator{seed};
#else
    std::random_device RandomDevice{};
    std::mt19937 RandomNumberGenerator{RandomDevice()};
#endif // defined

    std::shuffle(m_vCards.begin(), m_vCards.end(), RandomNumberGenerator);
}

/**
  * Deal cards from the deck.
  *
  * Deal cards into a vector of hands.
  *
  * \param nNumberOfCardsPerHand Number of cards per hand to deal
  * \param vHands                Vector of hands to be dealt to
  * \param bDealCardsEqually     Flag to deal cards equally
  *
  */

void Deck::Deal(int nNumberOfCardsPerHand, std::vector<Hand> &vHands, bool bDealCardsEqually)
{
    std::vector<Card>::size_type nTotalCards {0};

    // Calculate total number of cards to deal
    if (nNumberOfCardsPerHand == 0)
    {
        nTotalCards = m_vCards.size();
    }
    else
    {
        nTotalCards = nNumberOfCardsPerHand * vHands.size();

        // Ensure equal amount of cards are dealt
        if (bDealCardsEqually)
        {
            int nRemainder = nTotalCards % vHands.size();
            if (nRemainder != 0 )
            {
                nTotalCards -= nRemainder;
            }
        }

        // Ensure we have enough cards to deal
        if (nTotalCards > m_vCards.size())
        {
            std::string sErrorMessage = "Total number of cards to be dealt (" + std::to_string(nTotalCards) + ") is greater than the number of cards in the deck (" + std::to_string(m_vCards.size()) + ")";
            throw GameAIException(sErrorMessage);
        }
    }

    // Clear hands
    for (Hand &cHand : vHands)
    {
        cHand.RemoveAllCards();
    }

    // Deal total number of cards
    while (nTotalCards)
    {
        for (Hand &cHand : vHands)
        {
            cHand.AddCard(m_vCards.front());
            m_vCards.erase(m_vCards.begin());
            --nTotalCards;
        }
    }
}

/**
  * Set Aces to lowest value.
  *
  */

void Deck::SetAcesLow()
{
    for (Card &cCard : m_vCards)
    {
        if (cCard.Rank() == "A")
        {
            cCard.SetValuesToLowest();
        }
    }
}

