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

#include "Card.h"

// Initial static int to zero; used to generate unique identifier numbers for cards
int Card::m_nCardCount {0};

bool Card::SetRankAndSuit(const std::string &sRankAndSuit)
{
    if (sRankAndSuit.empty())
        return false;

    std::string sSuit(1, sRankAndSuit.back());
    if (!SetSuit(sSuit))
        return false;

    std::string sRank(sRankAndSuit.substr(0, sRankAndSuit.length() - 1));
    if (!SetRank(sRank))
        return false;

    return true;
}

/**
  * Serialize the class into a Json object.
  *
  * \return The Json Value object representing the class.
  */

Json::Value Card::JsonSerialization() const
{
    Json::Value jValue;

    jValue["Rank"]        = m_sRank;
    jValue["Suit"]        = m_sSuit;
    jValue["RankValue"]   = m_nRankValue;
    jValue["SortValue"]   = m_nSortValue;
    jValue["ID"]          = m_nID;
    jValue["TurnedUp"]    = m_bTurnedUp;
    jValue["Probability"] = m_fProbability;
    jValue["Eligible"]    = m_bEligible;

    return jValue;
}

/**
  *Deserialize the class from a JSON string.
  *
  * \param sJsonCard     A JSON string representing a Card.
  * \param sErrorMessage A string to return an error message if needed
  *
  * \return True if deserialization is successful, false otherwise
  */

bool Card::JsonDeserialization(const std::string &sJsonCard, std::string &sErrorMessage)
{
    Json::Value  jCard;
    Json::Reader jReader;

     if (jReader.parse(sJsonCard, jCard, false))
    {
        return JsonDeserialization(jCard);
    }
    else
    {
        sErrorMessage = jReader.getFormattedErrorMessages();
        return false;
    }
}

/**
  * Deserialize the class from a Json::Value object.
  *
  * \param jCard A Json::Value representing a Card.
  *
  * \return True.
  */

bool Card::JsonDeserialization(const Json::Value &jCard)
{
    m_sRank        = jCard["Rank"].asString();
    m_sSuit        = jCard["Suit"].asString();
    m_nRankValue   = jCard["RankValue"].asInt();
    m_nSortValue   = jCard["SortValue"].asInt();
    m_nID          = jCard["ID"].asInt();
    m_bTurnedUp    = jCard["TurnedUp"].asBool();
    m_fProbability = jCard["Probability"].asFloat();
    m_bEligible    = jCard["Eligible"].asBool();

    return true;
}
