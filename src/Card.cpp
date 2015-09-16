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
    jValue["Value"]       = m_nValue;
    jValue["ID"]          = m_nID;
    jValue["TurnedUp"]    = m_bTurnedUp;
    jValue["Probability"] = m_fProbability;

    return jValue;
}

/**
  * Deserialize the class from a Json object.
  *
  * \param sJsonCard     A JSON string representing a Card.
  * \param sErrorMessage A string to return an error message if needed
  *
  * \return True if deserialization is successful, false otherwise
  */

bool Card::JsonDeserialization(const std::string &sJsonCard, std::string &sErrorMessage)
{
    Json::Value jValue;
    Json::Reader jReader;

     if (jReader.parse(sJsonCard, jValue, false))
    {
        m_sRank        = jValue["Rank"].asString();
        m_sSuit        = jValue["Suit"].asString();
        m_nValue       = jValue["Value"].asInt();
        m_nID          = jValue["ID"].asInt();
        m_bTurnedUp    = jValue["TurnedUp"].asBool();
        m_fProbability = jValue["Probability"].asFloat();

        return true;
    }
    else
    {
        sErrorMessage = jReader.getFormattedErrorMessages();
        return false;
    }
}
