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

int PlayingCards::HasCards() const
{
    return m_vCards.size();
}

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

void PlayingCards::AddCard(Card &cCard)
{
    m_vCards.push_back(cCard);
}

void PlayingCards::AddCardToTop(Card &cCard)
{
    std::vector<Card>::iterator it = m_vCards.begin();
    m_vCards.insert(it, cCard);
}

void PlayingCards::AddCards(std::vector<Card> &vCards)
{
    for (Card &cCard : vCards)
    {
        m_vCards.push_back(cCard);
    }
}

Card PlayingCards::DrawTopCard()
{
    Card cCard = m_vCards.front();

    m_vCards.erase(m_vCards.begin());

    return cCard;
}

Card PlayingCards::PeekAtBottomCard() const
{
    Card cCard = m_vCards.back();

    return cCard;
}

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
