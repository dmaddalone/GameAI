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

    // Create a new hand to evaluate match opportunities by removing the
    // matched cards from this hand.  If no opportunities, put the cards back.
    Hand cPossibleMatchHand;
    std::vector<Card> vMatchedCards = RemoveCards(vCards);
    cPossibleMatchHand.AddCards(vMatchedCards);

    //
    // Evaluate meld opportunities; if none return empty match
    //
    bool bGoodMatch {false};

    // Evaluate for sequence first
    if (bEvalSequence)
    {
        if (cPossibleMatchHand.MatchOpportunities(nCount, true, false))
        {
            bGoodMatch = true;
            cMatch.SetType(MatchType::TYPE_SEQUENCE);
        }
    }

    // If no sequence, then evaluate for book
    if (bEvalBook && !bGoodMatch)
    {
        if (cPossibleMatchHand.MatchOpportunities(nCount, false, true))
        {
            bGoodMatch = true;
            cMatch.SetType(MatchType::TYPE_SAME_RANK);
        }
    }

    // If not sequence or book, return empty match
    if (!bGoodMatch)
    {
        // Put cards back into this had from possible match hand.
        AddCards(vMatchedCards);
        return cMatch;
    }

    // Insert cards into the match
    cMatch.AddCards(vMatchedCards);

    return cMatch;
}


bool Hand::MatchOpportunities(const int nCount, const bool bEvalSequence, const bool bEvalBook)
{
    bool bOpportunity {false};

    // Set all cards to ineligible
    for (Card &cCard : m_vCards)
    {
        cCard.SetEligibility(false);
    }

    // Ensure we have enough cards to satisfy the nCount
    if (static_cast<int>(m_vCards.size()) < nCount)
        return false;

    // Evaluate for books of the same rank
    if (bEvalBook)
    {
        for (Card &cCard : m_vCards)
        {
            if (HasCardsOfRank(cCard.Rank()) >= nCount)
            {
                bOpportunity = true;
                cCard.SetEligibility(true);
            }
        }
    }

    // Evaluate for sequence in same suit
    if (bEvalSequence)
    {
        int  nSeqCount {1};
        //Card &cLastCard = m_vCards[0];
        Card cLastCard;
        int  nCardCounter {0};

        SortBySuit();

        for (Card &cCard : m_vCards)
        {
            // Same suit?
            if (cCard.Suit() == cLastCard.Suit())
            {
                // If Rank / Value is one more than last card
                if (cCard.SortValue() == cLastCard.SortValue() + 1)
                {
                    ++nSeqCount;
                    if (nSeqCount >= nCount)
                    {
                        bOpportunity = true;
                        cCard.SetEligibility(true);
                        for (int iii = nSeqCount - 1; iii > 0; --iii)
                        {
                            m_vCards[nCardCounter - iii].SetEligibility(true);
                        }
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
            ++nCardCounter;
        }
    }

    return bOpportunity;
}

bool Hand::RemoveLayoffs(std::unordered_multimap<int, Match> &uommMatches, Card &cCard, const bool bEvalSequence, const bool bEvalBook)
{
    // Create a hand for the layoff card to evaluate layoff opportunities
    Hand cProbableLayoffHand;
    Card cLayoffCard = RemoveCard(cCard);
    cProbableLayoffHand.AddCard(cLayoffCard);

    //  Add a Card to a Hand
    //  if LayoffOpportunity for sequence is true
    //      find eligible match and add card to uommMatches and remove it from vCards
    if (cProbableLayoffHand.LayoffOpportunities(uommMatches, bEvalSequence, bEvalBook))
    {
        for (auto &PlayerMatch : uommMatches)
        {
            if (PlayerMatch.second.Eligible())
            {
                PlayerMatch.second.AddCard(cLayoffCard);
                PlayerMatch.second.SortByRank();
                return true;
            }
        }
    }

    // Place layoff card back into hand
    AddCard(cLayoffCard);
    return false;
}

bool Hand::LayoffOpportunities(std::unordered_multimap<int, Match> &uommMatches, const bool bEvalSequence, const bool bEvalBook)
{
    bool bOpportunity {false};

    SortBySuit();

    // Set all cards to ineligible
    for (Card &cCard : m_vCards)
    {
        cCard.SetEligibility(false);
    }

    // Loop through all matches
    for (auto &PlayerMatch : uommMatches)
    {
        // Set eligibility of the match to false until proven otherwise
        PlayerMatch.second.SetEligibility(false);

        // If evaluating for sequences
        if (bEvalSequence)
        {
            // If this match is a sequence
            if (PlayerMatch.second.Type() == MatchType::TYPE_SEQUENCE)
            {
                // Loop through all cards in this hand
                for (Card &cCard : m_vCards)
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
                            cCard.SetEligibility(true);
                            bOpportunity =  true;
                        }

                        // If Card value is one more than last card, we have a layoff opportunity
                        if (cCard.SortValue() == PlayerMatch.second.PeekAtBottomCard().SortValue() + 1)
                        {
                            PlayerMatch.second.SetEligibility(true);
                            cCard.SetEligibility(true);
                            bOpportunity = true;
                        }
                    }
                }
            }
        }

        // If evaluating for books of rank
        if (bEvalBook)
        {
            // If this match is a sequence
            if (PlayerMatch.second.Type() == MatchType::TYPE_SAME_RANK)
            {
                // Loop through all cards in this hand
                for (Card &cCard : m_vCards)
                {
                    // If rank match matches this card rank, we have a layoff opportunity
                    if (PlayerMatch.second.HasRank(cCard.Rank()))
                    {
                        PlayerMatch.second.SetEligibility(true);
                        cCard.SetEligibility(true);
                        bOpportunity = true;
                    }
                }
            }
        }
    }

    return bOpportunity;
}

void Hand::Discard(PlayingCards &cDiscardPile, const Card &cCard)
{
    // Get the matching card, cDiscardCard, from the hand (matching card has
    // all properties, while cCard has only rank and suit)
    Card cDiscardCard = RemoveCard(cCard);

    // Copy turn up property
    cDiscardCard.TurnUp(cCard.TurnedUp());

    // Add matched cDiscardCard to discard pile
    cDiscardPile.AddCardToTop(cDiscardCard);
}

/**
  * Sort hand by rank.
  *
  */

void Hand::SortByRank()
{
    std::sort(m_vCards.begin(), m_vCards.end());
}

void Hand::SortBySuit()
{
    SortByRank();

    std::sort(m_vCards.begin(), m_vCards.end(),
        [](const Card &cFirstCard, const Card &cSecondCard) -> bool
        {
            //return cFirstCard.SortValue() > cSecondCard.SortValue() &&
            //       cFirstCard.Suit() > cSecondCard.Suit();

            return cFirstCard.Suit() > cSecondCard.Suit();
        });
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
    jMatch["Type"]         = static_cast<int>(m_ecMatchType);
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
        return JsonDeserialization(jMatch, sErrorMessage);
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

    m_ecMatchType    = static_cast<MatchType>(jMatch["Type"].asInt());
    m_bEligibleMatch = jMatch["Eligibility"].asBool();

    return true;
}
