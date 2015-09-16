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

// Initial static int to zero; used to generate unique identifier numbers
int Hand::m_nCount {0};

/**
  * Display cards in the hand.
  *
  * Return a string represent the cards in the hand.  If cards are not turned
  * up, their suit and rank will be blocked from display.
  *
  * \return String of cards.
  */

std::string Hand::DisplayCards() const
{
    std::string sCards {};

    for (const Card &cCard : m_vCards)
    {
        sCards.append(cCard.DisplayShortName());
        sCards.append(" ");
    }

    return sCards;
}

/**
  * Display ranks of cards in the hand.
  *
  * Return a string represent the ranks of cards in the hand. If cards are not turned
  * up, their rank will be blocked from display.
  *
  * \return String of ranks of cards.
  */

std::string Hand::DisplayRanks() const
{
    std::string sCards {};

    for (const Card &cCard : m_vCards)
    {
        sCards.append(cCard.DisplayRank());
        sCards.append(" ");
    }

    return sCards;
}

/**
  * Display cards in the hand.
  *
  * Return a string represent the cards in the hand.
  *
  * \return String of cards.
  */

std::string Hand::Ranks() const
{
    std::string sCards {};

    for (const Card &cCard : m_vCards)
    {
        sCards.append(cCard.Rank());
        sCards.append(" ");
    }

    return sCards;
}

/**
  * Remove books of ranks from the hand.
  *
  * Remove books from hand and return as a hand.
  *
  * \param nSizeOfBook Number representing how many cards make a book.
  *
  * \return Hand representing the book.
  */

Hand Hand::RemoveBookByRank(int nSizeOfBook)
{
    Hand cHand {};
    std::vector<Card> vCards {};

    for (const std::string &sRank : asRank)
    {
        if (HasCardsOfRank(sRank) == nSizeOfBook)
        {
            vCards = RemoveCardsOfRank(sRank);
            cHand.AddCards(vCards);
            return cHand;
        }
    }

    return cHand;
}

/**
  * Sort hand by rank.
  *
  */

void Hand::SortByRank()
{
    std::sort(m_vCards.begin(), m_vCards.end());
}
