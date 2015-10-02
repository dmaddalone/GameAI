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

std::string Hand::DisplayCards(bool bForce) const
{
    std::string sCards {};

    for (const Card &cCard : m_vCards)
    {
        sCards.append(cCard.DisplayShortName(bForce));
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

std::string Hand::DisplayRanks(bool bForce) const
{
    std::string sCards {};

    for (const Card &cCard : m_vCards)
    {
        sCards.append(cCard.DisplayRank(bForce));
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
    Book cBook;
    std::vector<Card> vCards {};

    for (const std::string &sRank : asRank)
    {
        if (HasCardsOfRank(sRank) == nSizeOfBook)
        {
            vCards = RemoveCardsOfRank(sRank);
            cBook.AddCards(vCards);
            return cBook;
        }
    }

    return cBook;
}

Match Hand::RemoveMatch(std::vector<Card> &vCards, const int nCount, const bool bEvalSequence, const bool bEvalBook)
{
    Match cMatch;

    // If not enough cards in vector to consider, return empty match
    if (static_cast<int>(vCards.size()) < nCount)
        return cMatch;

    // Create a hand for the vector of cards to evaluate match opportunities
    Hand cHand;
    cHand.AddCards(vCards);

    //
    // Evaluate meld opportunities; if none return empty match
    //
    bool bGoodMatch {false};

    // Evaluate for sequence first
    if (bEvalSequence)
    {
        if (cHand.MatchOpportunities(nCount, true, false))
        {
            bGoodMatch = true;
            cMatch.SetTypeSequence();
        }
    }

    // If no sequence, then evaluate for book
    if (bEvalBook && !bGoodMatch)
    {
        if (cHand.MatchOpportunities(nCount, false, true))
        {
            bGoodMatch = true;
            cMatch.SetTypeSameRank();
        }
    }

    // If not sequence or book, return empty match
    if (!bGoodMatch)
    {
        return cMatch;
    }

    //
    // Remove cards from hand and insert into the match
    //
    Card cMatchedCard;
    for (const Card &cCard : vCards)
    {
        cMatchedCard = RemoveCard(cCard);
        // Evaluate matched card
        // If valid, add it to match
        if (cMatchedCard.RankValid() && cMatchedCard.SuitValid())
        {
            cMatch.AddCard(cMatchedCard);
        }
        // Else remove all previously added cards from match,
        // add them back to the hand
        // and return an emoty match
        else
        {
            std::vector<Card> vPreviouslyAddedCards = cMatch.RemoveAllCards();
            AddCards(vPreviouslyAddedCards);
            return cMatch;
        }
    }

    return cMatch;
}


bool Hand::MatchOpportunities(const int nCount, const bool bEvalSequence, const bool bEvalBook)
{
    // Evaluate for books of the same rank
    if (bEvalBook)
    {
        for (const Card &cCard : m_vCards)
        {
            if (HasCardsOfRank(cCard.Rank()) >= nCount) //TODO: Need to be able to evaluate all cards in the hand
                return true;
        }
    }

    // Evaluate for sequence in same suit
    if (bEvalSequence)
    {
        int  nSeqCount {1};
        Card cLastCard;

        SortByRank();

        for (const Card &cCard : m_vCards)
        {
            // Same suit?
            if (cCard.Suit() == cLastCard.Suit())
            {
                // If Rank / Value is one more than last card
                if (cCard.SortValue() == cLastCard.SortValue() + 1)
                {
                    ++nSeqCount;
                    if (nSeqCount >= nCount) //TODO: Need to be able to evaluate all cards in the hand
                    {
                        return true;
                    }
                }
                else
                {
                    // Start new count
                    nSeqCount = 1;
                }
            }
            else
            {
                // Start new count
                nSeqCount = 1;
            }

            cLastCard = cCard;
        }
    }

    return false;
}

bool Hand::RemoveLayoffs(std::unordered_multimap<int, Match> &uommMatches, Card &cCard, const bool bEvalSequence, const bool bEvalBook)
{
    // Create a hand for the vector of cards to evaluate layoff opportunities
    Hand cHand;
    cHand.AddCard(cCard);

    //  Add a Card to a Hand
    //  if LayoffOpportunity for sequence is true
    //      find eligible match and add card to uommMatches and remove it from vCards
    if (cHand.LayoffOpportunities(uommMatches, bEvalSequence, bEvalBook))
    {
        for (auto &PlayerMatch : uommMatches)
        {
            if (PlayerMatch.second.Eligible())
            {
                PlayerMatch.second.AddCard(cCard);
                PlayerMatch.second.SortByRank();
                return true;
            }
        }
    }

    return false;
}

bool Hand::LayoffOpportunities(std::unordered_multimap<int, Match> &uommMatches, const bool bEvalSequence, const bool bEvalBook)
{
    // Loop through all matches
    for (auto &PlayerMatch : uommMatches)
    {
        // Set eligibility of the match to false until proven otherwise
        PlayerMatch.second.SetEligibility(false);

        // If evaluating for sequences
        if (bEvalSequence)
        {
            // If this match is a sequence
            if (PlayerMatch.second.TypeSequence())
            {
                // Loop through all cards in this hand
                for (const Card &cCard : m_vCards)
                {
                    // If a card suit matches the sequence match suit
                    if (PlayerMatch.second.HasSuit(cCard.Suit()))
                    {
                        // Sort the sequence match
                        PlayerMatch.second.SortByRank();

                        // If Card value is one less than first card, we have a layoff opportunity
                        if (cCard.SortValue() == PlayerMatch.second.PeekAtTopCard().SortValue() - 1)
                        {
                            PlayerMatch.second.SetEligibility(true);
                            return true;
                        }


                        // If Card value is one more than last card, we have a layoff opportunity
                        if (cCard.SortValue() == PlayerMatch.second.PeekAtBottomCard().SortValue() + 1)
                        {
                            PlayerMatch.second.SetEligibility(true);
                            return true;
                        }

                    }
                }
            }
        }

        // If evaluating for books of rank
        if (bEvalBook)
        {
            // If this match is a sequence
            if (PlayerMatch.second.TypeSameRank())
            {
                // Loop through all cards in this hand
                for (const Card &cCard : m_vCards)
                {
                    // If rank match matches this card rank, we have a layoff opportuity
                    if (PlayerMatch.second.HasRank(cCard.Rank()))
                    {
                        PlayerMatch.second.SetEligibility(true);
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void Hand::Discard(PlayingCards &PlayingCards, Card &cCard)
{
    cCard.TurnUp(true);
    PlayingCards.AddCardToTop(cCard);
    RemoveCard(cCard);
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
    jMatch["Eligibility"]  = m_bEligibleMatch;

    return jMatch;
}

/**
  * Deserialize the class from a JSON string
  *
  * \param sJsonMatch     A JSON string representing a Match.
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
        m_bEligibleMatch      = jMatch["Eligibility"].asBool();

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
    m_bEligibleMatch      = jMatch["Eligibility"].asBool();

    return true;
}

