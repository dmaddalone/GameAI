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

void ProbableDeck::UpdateRankProbabilities(ProbableHand &cProbableOpponentHand)
{
    int nCardsOfRankInDeck {};
    int nCardsOfRankInHand {};
    std::vector<Card> vProbableCards {};

    // Sort cards by rank
    std::sort(m_vCards.begin(), m_vCards.end());

    // Keep track of last rank to be evaluated
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
                nCardsOfRankInDeck = this->HasCardsOfRank(cProbableCard.Rank());
                nCardsOfRankInHand = cProbableOpponentHand.HasCardsOfRank(cProbableCard.Rank());

                //
                // Update probabilities in ProbableDeck
                //

                // Remove cards from ProbableDeck
                vProbableCards = this->RemoveCardsOfRank(cProbableCard.Rank());

                // Loop through all cards
                for (Card &cCard : vProbableCards)
                {
                    // Update probability = num of cards in deck divided by sum of num cards in deck and num cards in opponent's hand
                    cCard.SetProbability(static_cast<float>(this->NumberOfCards() / (this->NumberOfCards() + cProbableOpponentHand.NumberOfCards())));
                }

                // Add cards back to ProbableDeck
                this->AddCards(vProbableCards);

                //
                // Update probabilities in ProbableOpponentHand
                //

                // If cards are found in probable opponent's hands, evaluate
                if (nCardsOfRankInHand > 0)
                {
                    // Get vector of cards from ProbableOpponentHand
                    std::vector<Card> vProbableCards = cProbableOpponentHand.RemoveCardsOfRank(cProbableCard.Rank());

                    // If probability is not certainty, update
                    if (vProbableCards[0].Probability() < 1.0)
                    {
                        // Loop through all cards
                        for (Card &cCard : vProbableCards)
                        {
                            // Update probability = num cards in opponent's hand divided by sum of num cards in deck and num cards in opponent's hand
                            cCard.SetProbability(static_cast<float>(cProbableOpponentHand.NumberOfCards() / (this->NumberOfCards() + cProbableOpponentHand.NumberOfCards())));
                        }
                    }

                    // Add cards back to ProbableOpponentHand
                    cProbableOpponentHand.AddCards(vProbableCards);
                }
            } // if (cProbableCard.Probability() < 1.0)
        } // if (cProbableCard.Rank() != cLastProbableCard.Rank())
    } // for (Card &cProbableCard : m_vCards)

    return;
}
