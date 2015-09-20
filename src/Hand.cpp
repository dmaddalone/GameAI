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
  * Remove books of ranks from the hand.
  *
  * Remove books from hand and return as a hand.
  *
  * \param nSizeOfBook Number representing how many cards make a book.
  *
  * \return Hand representing the book.
  */

Book Hand::RemoveBookByRank(int nSizeOfBook)
{
    //Hand cHand {};
    Book cBook;
    std::vector<Card> vCards {};

    for (const std::string &sRank : asRank)
    {
        if (HasCardsOfRank(sRank) == nSizeOfBook)
        {
            vCards = RemoveCardsOfRank(sRank);
            //cHand.AddCards(vCards);
            cBook.AddCards(vCards);
            //return cHand;
            return cBook;
        }
    }

    return cBook;
}

/**
  * Sort hand by rank.
  *
  */

void Hand::SortByRank()
{
    std::sort(m_vCards.begin(), m_vCards.end());
}

/**
  * Serialize the class into a Json object.
  *
  * \return The Json Value object representing the class.
  */

Json::Value Match::JsonSerialization() const
{
    Json::Value jCards = Hand::JsonSerialization();
    Json::Value jMatch;

    jMatch["Cards"]        = jCards;
    jMatch["TypeSameRank"] = m_stType.TypeSameRank;
    jMatch["TypeSameSuit"] = m_stType.TypeSameSuit;
    jMatch["TypeSequence"] = m_stType.TypeSequence;

    return jMatch;
}

/**
  * Deserialize the class from a JSON string
  *
  * \param sJsonMatch     A JSON string representing a PlayingCards.
  * \param sErrorMessage  A string to return an error message if needed
  *
  * \return True if deserialization is successful, false otherwise
  */

bool Match::JsonDeserialization(const std::string &sJsonMatch, std::string &sErrorMessage)
{
    Json::Reader jReader;
    Json::Value  jMatch;
    Json::Value  jCards;

    // Deserialize match specifics
    if (jReader.parse(sJsonMatch, jMatch, false))
    {
        // Deserialize cards first
        jCards = jMatch["Cards"];

        if (!Hand::JsonDeserialization(jCards, sErrorMessage))
        {
            return false;
        }

        m_stType.TypeSameRank = jMatch["TypeSameRank"].asBool();
        m_stType.TypeSameRank = jMatch["TypeSameSuit"].asBool();
        m_stType.TypeSameRank = jMatch["TypeSequence"].asBool();

        return true;
    }
    else
    {
        sErrorMessage = jReader.getFormattedErrorMessages();
        return false;
    }
}

/**
  * Deserialize the class from a Json object.
  *
  * \param sJsonPlayingCards A JSON string representing a PlayingCards.
  * \param sErrorMessage     A string to return an error message if needed
  *
  * \return True if deserialization is successful, false otherwise
  */

bool Match::JsonDeserialization(const Json::Value &jMatch, std::string &sErrorMessage)
{
    Json::Value  jCards;

    // Deserialize cards first
    jCards = jMatch["Cards"];

    if (!Hand::JsonDeserialization(jCards, sErrorMessage))
    {
        return false;
    }

    m_stType.TypeSameRank = jMatch["TypeSameRank"].asBool();
    m_stType.TypeSameRank = jMatch["TypeSameSuit"].asBool();
    m_stType.TypeSameRank = jMatch["TypeSequence"].asBool();

    return true;
}

