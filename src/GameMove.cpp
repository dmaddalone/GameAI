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

#include "GameMove.h"

/**
  * Announce the from-move.
  *
  * Construct a string containing the to-move.
  *
  * \return A string containing the to-move.
  */

std::string GameMove::AnnounceFromMove() const
{
    std::string sMove {};

    if (!m_bUseFrom)
        return sMove;

    if (m_bUseY)
    {
        sMove = m_cFromX;
        sMove += m_cFromY;
    }
    else
    {
        sMove = m_cFromX;
    }

    return sMove;
}

/**
  * Announce the to-move.
  *
  * Construct a string containing the to-move.
  *
  * \return A string containing the to-move.
  */

std::string GameMove::AnnounceToMove() const
{
    std::string sMove;

    if (m_bNoMove)
    {
        sMove = "NO MOVE";
    }
    else if (m_bResignation)
    {
        sMove = "RESIGN";
    }
    else if (m_bUseY)
    {
        sMove = m_cToX;
        sMove += m_cToY;
    }
    else
    {
        sMove = m_cToX;
    }

    return sMove;
}

/**
  * Announce the card object.
  *
  * Construct a string containing the card object.
  *
  * \return A string containing the card rank and suit.
  */

std::string GameMove::AnnounceCard() const
{
    return m_cCard.DisplayShortName();
}

/**
  * Announce the card object - Rank only.
  *
  * Construct a string containing the card object.
  *
  * \return A string containing the card rank.
  */

std::string GameMove::AnnounceCardRank() const
{
    return m_cCard.Rank();
}

/**
  * Return the game move argument
  *
  * Construct a string containing the argument.
  *
  * \return If this is a 'Move', return the FromMove and ToMove.
  * Otherwise return the m_sArgument.
  */

std::string GameMove::Argument()
{
    if (Move())
    {
        return AnnounceFromMove() + AnnounceToMove();
    }
    else
    {
        return m_sArgument;
    }
}

Json::Value GameMove::JsonSerialization() const
{
    Json::Value jValue(Json::objectValue);

    jValue["sCommand"]     = m_sCommand;
    jValue["sArgument"]    = m_sArgument;

    jValue["nFromX"]       = m_nFromX;
    jValue["cFromX"]       = m_cFromX;
    jValue["nFromY"]       = m_nFromY;
    jValue["cFromY"]       = m_cFromY;
    jValue["nToX"]         = m_nToX;
    jValue["cToX"]         = m_cToX;
    jValue["nToY"]         = m_nToY;
    jValue["cToY"]         = m_cToY;

    jValue["bUseY"]        = m_bUseY;
    jValue["bUseFrom"]     = m_bUseFrom;
    jValue["bMove"]        = m_bMove;
    jValue["bNoMove"]      = m_bNoMove;
    jValue["bResignation"] = m_bResignation;
    jValue["bFold"]        = m_bFold;
    jValue["bDraw"]        = m_bDraw;
    jValue["nDraw"]        = m_nDraw;
    jValue["bAsk"]         = m_bAsk;
    jValue["bShow"]        = m_bShow;
    jValue["bScore"]       = m_bScore;
    jValue["bAnotherTurn"] = m_bAnotherTurn;
    jValue["bTestMove"]    = m_bTestMove;

    //Json::Value jCardValue(Json::objectValue);
    Json::Value jCardValue;
    jCardValue = m_cCard.JsonSerialization();

    jValue["Card"] = jCardValue;

    return jValue;
}

bool GameMove::JsonDeserialization(const std::string &sJsonGameMove, std::string &sErrorMessage)
{
    Json::Value jValue(Json::objectValue);
    Json::Reader jReader;

    if (jReader.parse(sJsonGameMove, jValue, false))
    {
        m_sCommand     = jValue["sCommand"].asString();
        m_sArgument    = jValue["sArgument"].asString();

        m_nFromX       = jValue["nFromX"].asInt();
        m_cFromX       = jValue["cFromX"].asInt();
        m_nFromY       = jValue["nFromY"].asInt();
        m_cFromY       = jValue["cFromY"].asInt();
        m_nToX         = jValue["nToX"].asInt();
        m_cToX         = jValue["cToX"].asInt();
        m_nToY         = jValue["nToY"].asInt();
        m_cToY         = jValue["cToY"].asInt();

        m_bUseY        = jValue["bUseY"].asBool();
        m_bUseFrom     = jValue["bUseFrom"].asBool();
        m_bMove        = jValue["bMove"].asBool();
        m_bNoMove      = jValue["bNoMove"].asBool();
        m_bResignation = jValue["bResignation"].asBool();
        m_bFold        = jValue["bFold"].asBool();
        m_bDraw        = jValue["bDraw"].asBool();
        m_bAsk         = jValue["bAsk"].asBool();
        m_bShow        = jValue["bShow"].asBool();
        m_bScore       = jValue["bScore"].asBool();
        m_bAnotherTurn = jValue["bAnotherTurn"].asBool();
        m_bTestMove    = jValue["bTestMove"].asBool();

        //std::cout << "{DEBUG] jValue[CARD]=" << jValue["Card"] << std::endl;
        //std::cout << "{DEBUG] jValue[CARD].toStyledString()=" << jValue["Card"].toStyledString() << std::endl;
        if (m_cCard.JsonDeserialization(jValue["Card"].toStyledString(), sErrorMessage))
        {
            std::cout << "[DEBUG] m_cCard.JsonDeserialization is good" << std::endl;
            return true;
        }
        else
        {
            std::cout << "[DEBUG] m_cCard.JsonDeserialization is bad" << std::endl;
            return false;
        }
    }

    sErrorMessage = jReader.getFormattedErrorMessages();
    return false;
}
