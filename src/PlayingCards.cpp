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

#include "PlayingCards.h"

/**
  * Return the number of cards in possession.
  *
  * \return The number of cards in possession.
  */

int PlayingCards::HasCards() const
{
    return m_vCards.size();
}

/**
  * Evaluate whether a rank is in possession.
  *
  * Evaluate all cards in possession for the rank.
  *
  * \param sRank The rank in question.
  *
  * \return True if the rank is in possession, false otherwise.
  */

bool PlayingCards::HasRank(const std::string &sRank) const
{
    for (const Card &cCard : m_vCards)
    {
        if (sRank == cCard.Rank())
        {
            return true;
        }
    }

    return false;
}

/**
  * Evaluate whether a suit is in possession.
  *
  * Evaluate all cards in possession for the suit.
  *
  * \param sSuit The suit in question.
  *
  * \return True if the suit is in possession, false otherwise.
  */

bool PlayingCards::HasSuit(const std::string &sSuit) const
{
    for (const Card &cCard : m_vCards)
    {
        if (sSuit == cCard.Suit())
        {
            return true;
        }
    }

    return false;
}

/**
  * Evaluate how many cards of a rank are in possession.
  *
  * Evaluate all cards in possession for the rank.
  *
  * \param sRank The rank in question.
  *
  * \return The number of cards of rank in possession.
  */

int PlayingCards::HasCardsOfRank(const std::string &sRank) const
{
    int nCount {0};

    for (const Card &cCard : m_vCards)
    {
        if (sRank == cCard.Rank())
        {
            ++nCount;
        }
    }

    return nCount;
}

/**
  * Evaluate how many cards of a suit are in possession.
  *
  * Evaluate all cards in possession for the suit.
  *
  * \param sSuit The suit in question.
  *
  * \return The number of cards of suit in possession.
  */

int PlayingCards::HasCardsOfSuit(const std::string &sSuit) const
{
    int nCount {0};

    for (const Card &cCard : m_vCards)
    {
        if (sSuit == cCard.Suit())
        {
            ++nCount;
        }
    }

    return nCount;
}

/**
  * Return string representing ranks in possession.
  *
  * \return String of ranks.
  */

std::string PlayingCards::Ranks() const
{
    std::string sRanks {};

    for (const Card &cCard : m_vCards)
    {
        sRanks += cCard.Rank() + " ";
    }

    if (sRanks.empty())
        sRanks = "empty";

    return sRanks;
}

/**
  * Add a card to possession.
  *
  * The card is added to the bottom (last) of the possession.
  *
  * \param cCard The card to add.
  */

void PlayingCards::AddCard(Card &cCard)
{
    m_vCards.push_back(cCard);
}

/**
  * Add a card to possessions as the top or first card.
  *
  * \param cCard The card to add.
  */

void PlayingCards::AddCardToTop(Card &cCard)
{
    std::vector<Card>::iterator it = m_vCards.begin();
    m_vCards.insert(it, cCard);
}

/**
  * Add a cards to possession.
  *
  * The cards are added to the bottom (last) of the possession.
  *
  * \param vCards Vector of cards to add.
  */

void PlayingCards::AddCards(std::vector<Card> &vCards)
{
    for (Card &cCard : vCards)
    {
        m_vCards.push_back(cCard);
    }
}

/**
  * Draw the top card from the possession.
  *
  * The top card is removed from the possession and returned
  * to the caller.
  *
  * \return The top card.
  */

Card PlayingCards::DrawTopCard()
{
    Card cCard = m_vCards.front();

    m_vCards.erase(m_vCards.begin());

    return cCard;
}

/**
  * Peek at the bottom card from the possession.
  *
  * A copy of the bottom card is returned to the caller.
  *
  * \return A copy of the bottom card.
  */

Card PlayingCards::PeekAtBottomCard() const
{
    Card cCard = m_vCards.back();

    return cCard;
}

/**
  * Remove a specific card from the possession.
  *
  * The card matching the rank and suit are removed from the
  * possession and returned to the caller.
  *
  * \param sRank The rank in question
  * \param sSuit The suit in question
  *
  * \return The matching card.
  */

Card PlayingCards::RemoveCard(std::string sRank, std::string sSuit)
{
    Card cCard;

    for (std::vector<Card>::iterator it = m_vCards.begin(); it != m_vCards.end(); ++it)
    {
        if ((it->Rank() == sRank) && (it->Suit() == sSuit))
        {
            cCard = *it;
            m_vCards.erase(it);
        }
    }

    return cCard;
}

/**
  * Remove a cards of rank from the possession.
  *
  * Cards matching the rank are removed from the possession and returned to the caller.
  *
  * \param sRank           The rank in question
  * \param nNumberToRemove The maximum number of cards to remove
  *
  * \return A vector of cards.
  */

std::vector<Card> PlayingCards::RemoveCardsOfRank(std::string sRank, int nNumberToRemove)
{
    std::vector<Card> vCards {};
    int nRemoved {0};

    std::vector<Card>::iterator it = m_vCards.begin();
    while(it != m_vCards.end())
    {
        if (it->Rank() == sRank)
        {
            vCards.push_back(*it);
            it = m_vCards.erase(it);
            if ((nNumberToRemove > 0) && (++nRemoved == nNumberToRemove))
                break;
        }
        else
        {
            ++it;
        }
    }

    return vCards;
}

/**
  * Remove a cards of suit from the possession.
  *
  * Cards matching the suit are removed from the possession and returned to the caller.
  *
  * \param sSuit           The suit in question
  * \param nNumberToRemove The maximum number of cards to remove
  *
  * \return A vector of cards.
  */

std::vector<Card> PlayingCards::RemoveCardsOfSuit(std::string sSuit, int nNumberToRemove)
{
    std::vector<Card> vCards {};
    int nRemoved {0};

    std::vector<Card>::iterator it = m_vCards.begin();
    while(it != m_vCards.end())
    {
        if (it->Suit() == sSuit)
        {
            vCards.push_back(*it);
            it = m_vCards.erase(it);
            if ((nNumberToRemove > 0) && (++nRemoved == nNumberToRemove))
                break;
        }
        else
        {
            ++it;
        }
    }

    return vCards;
}

/**
  * Serialize the class into a Json object.
  *
  * \return The Json Value object representing the class.
  */

Json::Value PlayingCards::JsonSerialization() const
{
    Json::Value jCards;

    // The following are used to create a unique index that will retain order
    // when serialized to JSON.  May be used for up to 260 unique values: a0 to z9.
    int nCounter {0};
    char chLetter = 'a';
    std::string sIndex {};

    for (const Card &cCard : m_vCards)
    {
        // Manage index
        if (nCounter >= 10)
        {
            nCounter = 0;
            ++chLetter;
        }
        sIndex.assign(1, chLetter);
        sIndex.append(std::to_string(nCounter));
        ++nCounter;

        jCards[sIndex] = cCard.JsonSerialization();
    }

    return jCards;
}

/**
  * Deserialize the class from a Json object.
  *
  * \param sJsonPlayingCards A JSON string representing a PlayingCards.
  * \param sErrorMessage     A string to return an error message if needed
  *
  * \return True if deserialization is successful, false otherwise
  */

bool PlayingCards::JsonDeserialization(const std::string &sJsonPlayingCards, std::string &sErrorMessage)
{
    Json::Reader jReader;
    Json::Value  jCards;
    Json::Value  jCard;
    Card cCard;

    if (jReader.parse(sJsonPlayingCards, jCards, false))
    {
        m_vCards.clear();

        for (Json::ValueIterator it = jCards.begin(); it != jCards.end(); ++it)
        {
            jCard = (*it);
            if (cCard.JsonDeserialization(jCard.toStyledString(), sErrorMessage))
            {
                m_vCards.push_back(cCard);
            }
            else
            {
                return false;
            }
        }

        return true;
    }
    else
    {
        sErrorMessage = jReader.getFormattedErrorMessages();
        return false;
    }
}

/**
  * Update probabilities of holding certain ranks in possession.
  *
  * \param nOtherCards  The number of other cards to consider besides
  * what is in possession.
  *
  */

void PlayingCards::UpdateRankProbabilities(int nOtherCards)
{
    // Loop through all cards in probable possession
    for (Card &cProbableCard : m_vCards)
    {
        // If probability is less than 100% (certainty), update probabilities
        if (cProbableCard.Probability() < 1.0)
        {
            // Update probability = num of cards in possession divided by sum of num cards in possessions and num of other cards to consider
            cProbableCard.SetProbability(static_cast<float>(this->NumberOfCards()) /
                static_cast<float>((this->NumberOfCards() + nOtherCards)));
        }
    }

    return;
}
