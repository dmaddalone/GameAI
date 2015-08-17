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

Json::Value Card::JsonSerialization()
{
    Json::Value jValue(Json::objectValue);

    jValue["Rank"]  = m_sRank;
    jValue["Suit"]  = m_sSuit;
    jValue["Value"] = m_nValue;
    jValue["nID"]   = m_nID;

    return jValue;
}

bool Card::JsonDeserialization(const std::string &sJsonGameMove, std::string &sErrorMessage)
{
    Json::Value jValue(Json::objectValue);
    Json::Reader jReader;

    if (jReader.parse(sJsonGameMove, jValue, false))
    {
        m_sRank  = jValue["sRank"].asString();
        m_sSuit  = jValue["sSuit"].asString();
        m_nValue = jValue["nValue"].asInt();
        m_nID    = jValue["nID"].asInt();

        return true;
    }
    else
    {
        sErrorMessage = jReader.getFormattedErrorMessages();
        return false;
    }
}

