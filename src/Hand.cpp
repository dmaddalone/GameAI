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

#include "Hand.h"

/*
// Initial static int to zero; used to generate unique identifier numbers for hands
int Hand::m_nHandCount {0};

int Hand::HasCards() const
{
    return m_vCards.size();
}

bool Hand::RankInHand(std::string sRank) const
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

void Hand::AddCard(Card &cCard)
{
    m_vCards.push_back(cCard);
}

void Hand::AddCardToTop(Card &cCard)
{
    std::vector<Card>::iterator it = m_vCards.begin();
    m_vCards.insert(it, cCard);
}

void Hand::AddCards(std::vector<Card> &vCards)
{
    for (Card &cCard : vCards)
    {
        m_vCards.push_back(cCard);
    }
}

Card Hand::DrawTopCard()
{
    Card cCard = m_vCards.front();

    m_vCards.erase(m_vCards.begin());

    return cCard;
}

Card Hand::RemoveCard(std::string sRank, std::string sSuit)
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

std::vector<Card> Hand::RemoveCardsOfRank(std::string sRank)
{
    std::vector<Card> vCards {};

    for (std::vector<Card>::iterator it = m_vCards.begin(); it != m_vCards.end(); ++it)
    {
        if (it->Rank() == sRank)
        {
            vCards.push_back(*it);
            m_vCards.erase(it);
        }
    }

    return vCards;
}

std::vector<Card> Hand::RemoveCardsOfSuit(std::string sSuit)
{
    std::vector<Card> vCards {};

    for (std::vector<Card>::iterator it = m_vCards.begin(); it != m_vCards.end(); ++it)
    {
        if (it->Suit() == sSuit)
        {
            vCards.push_back(*it);
            m_vCards.erase(it);
        }
    }

    return vCards;
}

std::string Hand::DisplayCards() const
{
    std::string sCards {};

    for (const Card &cCard : m_vCards)
    {
        sCards.append(cCard.ShortName());
        sCards.append(" ");
    }

    return sCards;
}


*/
