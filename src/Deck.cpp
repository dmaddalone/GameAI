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

Deck::Deck() : PlayingCards()
{
    // Create standard 52 card deck
    m_vCards.clear();

    //for (std::size_t iii = 0; iii < sizeof(asRank); ++iii)
    for (std::size_t iii = 0; iii < asRank.size(); ++iii)
    {
        //for (std::size_t jjj = 0; jjj < sizeof(asSuit); ++jjj)
        for (std::size_t jjj = 0; jjj < asSuit.size(); ++jjj)
        {
            m_vCards.emplace_back(asRank[iii], asSuit[jjj], aiValue[iii]);
        }
    }
}

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

void Deck::Deal(int nNumberOfCardsPerHand, std::vector<Hand> &vHands, bool bDealCardsEqually)
{
    std::vector<Card>::size_type nTotalCards {0};

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

    //while (nTotalCards && nTotalCards >= vHands.size())
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

void ProbableDeck::UpdateRankProbabilities(Hand &cProbableOpponentHand)
{
    int nCardsInDeck = HasCards();
    int nCardsInHand = cProbableOpponentHand.HasCards();
    int nTotalCards  = nCardsInDeck + nCardsInHand;
    int nCardsOfRankInDeck {};
    int nCardsOfRankInHand {};

    std::sort(m_vCards.begin(), m_vCards.end());

    Card cLastProbableCard;

    // Loop through all cards in probable deck
    for (Card &cProbableCard : m_vCards)
    {
        // If a new rank is found, evaluate
        if (cProbableCard.Rank() != cLastProbableCard.Rank())
        {
            cLastProbableCard = cProbableCard;

            // If probability is less than 100% (certainty), update probabilities
            if (cProbableCard.Probability() < 1.0)
            {
                // Collect number of cards of rank in probable deck and probable opponent's hand
                nCardsOfRankInDeck = HasCardsOfRank(cProbableCard.Rank());
                nCardsOfRankInHand = cProbableOpponentHand.HasCardsOfRank(cProbableCard.Rank());

                // If cards are found in probable opponent's hands, check current probabilities
                if (nCardsOfRankInHand > 0)
                {
                    // Get vector of cards
                    std::vector<Card> vProbableCards = cProbableOpponentHand.RemoveCardsOfRank(cProbableCard.Rank());

                    // Check first card's probability in vector.  If less than certainty, update.
                    if (vProbableCards[0].Probability() < 1.0)
                    {
                        // Update probabilities in probable opponent's hand
                        // Update probabilities in probable deck
                    }

                    // Put cards back
                    cProbableOpponentHand.AddCards(vProbableCards);
                }
                else
                {

                }
            }
        }
    }

    return;
}
