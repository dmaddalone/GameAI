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

bool GameMove::SetCommand(const std::string &sCommand)
{
    std::string sMove {};
    std::string sArg{};

    std::size_t nPos = sCommand.find(GameVocabulary::DELIMETER);
    if (nPos != std::string::npos)
    {
        sMove = sCommand.substr(0, nPos - 1);
        sArg  = sCommand.substr(nPos + 1);
    }
    else
    {
        sMove = sCommand;
    }

    if (sMove.compare(GameVocabulary::MOVE))
    {
        SetMove(true);
        return true;
    }

    if (sMove.compare(GameVocabulary::NO_MOVE))
    {
        SetNoMove(true);
        return true;
    }

    if (sMove.compare(GameVocabulary::RESIGN))
    {
        SetResignation(true);
        return true;
    }

    if (sMove.compare(GameVocabulary::FOLD))
    {
        SetFold(true);
        return true;
    }

    if (sMove.compare(GameVocabulary::DRAW))
    {
        SetDraw(true);
        SetArgument(sArg);
        return true;
    }

    if (sMove.compare(GameVocabulary::ASK))
    {
        SetAsk(true);
        SetArgument(sArg);
        return true;
    }

    if (sMove.compare(GameVocabulary::SHOW))
    {
        SetShow(true);
        return true;
    }

    if (sMove.compare(GameVocabulary::SCORE))
    {
        SetScore(true);
        return true;
    }

    return false;
}

std::string GameMove::AnnounceMove() const
{
    std::string sMove {};

    if (IsMove())
    {
        sMove = AnnounceFromMove() + " " + AnnounceToMove();
        return sMove;
    }
    else
    {
        sMove = m_sCommand + " " + Argument();
    }

    return sMove;
}

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

std::string GameMove::Argument() const
{
    if (IsMove())
    {
        return AnnounceFromMove() + AnnounceToMove();
    }
    else
    {
        return m_sArgument;
    }
}

/**
  * Serialize the class into a Json object.
  *
  * \return The Json Value object representing the class.
  */

Json::Value GameMove::JsonSerialization() const
{
    Json::Value jValue(Json::objectValue);

    jValue["nPlayerNumber"] = m_nPlayerNumber;

    jValue["sCommand"]      = m_sCommand;
    jValue["sArgument"]     = m_sArgument;

    jValue["nFromX"]        = m_nFromX;
    jValue["cFromX"]        = m_cFromX;
    jValue["nFromY"]        = m_nFromY;
    jValue["cFromY"]        = m_cFromY;
    jValue["nToX"]          = m_nToX;
    jValue["cToX"]          = m_cToX;
    jValue["nToY"]          = m_nToY;
    jValue["cToY"]          = m_cToY;

    jValue["bUseY"]         = m_bUseY;
    jValue["bUseFrom"]      = m_bUseFrom;
    jValue["bMove"]         = m_bMove;
    jValue["bNoMove"]       = m_bNoMove;
    jValue["bResignation"]  = m_bResignation;
    jValue["bFold"]         = m_bFold;
    jValue["bDraw"]         = m_bDraw;
    jValue["nDraw"]         = m_nDraw;
    jValue["bAsk"]          = m_bAsk;
    jValue["bShow"]         = m_bShow;
    jValue["bScore"]        = m_bScore;
    jValue["bAnotherTurn"]  = m_bAnotherTurn;
    jValue["bTestMove"]     = m_bTestMove;
    jValue["bSuccess"]      = m_bSuccess;

    jValue["nCards"]        = m_nCards;

    Json::Value jCardValue;
    jCardValue = m_cCard.JsonSerialization();
    jValue["Card"] = jCardValue;

    jValue["Cards"] = m_cCards.JsonSerialization();

    return jValue;
}

/**
  * Deserialize the class from a Json object.
  *
  * \param sJsonGameMove A JSON string representing a game move
  * \param sErrorMessage A string to return an error message if needed
  *
  * \return True if deserialization is successful, false otherwise
  */

bool GameMove::JsonDeserialization(const std::string &sJsonGameMove, std::string &sErrorMessage)
{
    Json::Value jValue(Json::objectValue);
    Json::Reader jReader;

    if (jReader.parse(sJsonGameMove, jValue, false))
    {
        m_nPlayerNumber = jValue["nPlayerNumber"].asInt();

        m_sCommand      = jValue["sCommand"].asString();
        m_sArgument     = jValue["sArgument"].asString();

        m_nFromX        = jValue["nFromX"].asInt();
        m_cFromX        = jValue["cFromX"].asInt();
        m_nFromY        = jValue["nFromY"].asInt();
        m_cFromY        = jValue["cFromY"].asInt();
        m_nToX          = jValue["nToX"].asInt();
        m_cToX          = jValue["cToX"].asInt();
        m_nToY          = jValue["nToY"].asInt();
        m_cToY          = jValue["cToY"].asInt();

        m_bUseY         = jValue["bUseY"].asBool();
        m_bUseFrom      = jValue["bUseFrom"].asBool();
        m_bMove         = jValue["bMove"].asBool();
        m_bNoMove       = jValue["bNoMove"].asBool();
        m_bResignation  = jValue["bResignation"].asBool();
        m_bFold         = jValue["bFold"].asBool();
        m_bDraw         = jValue["bDraw"].asBool();
        m_bAsk          = jValue["bAsk"].asBool();
        m_bShow         = jValue["bShow"].asBool();
        m_bScore        = jValue["bScore"].asBool();
        m_bAnotherTurn  = jValue["bAnotherTurn"].asBool();
        m_bTestMove     = jValue["bTestMove"].asBool();
        m_bSuccess      = jValue["bSuccess"].asBool();

        m_nCards        = jValue["nCards"].asInt();

        if (!m_cCard.JsonDeserialization(jValue["Card"].toStyledString(), sErrorMessage))
        {
            return false;
        }

        if (m_cCards.JsonDeserialization(jValue["Cards"].toStyledString(), sErrorMessage))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    sErrorMessage = jReader.getFormattedErrorMessages();
    return false;
}

bool AllowedMoves::AddMovesInSequence(const int &nSeqNum, const std::string &sMove, const std::string &sArg)
{
    // May only insert moves with sequence less-than or equal to current index
    // and sequence number must be greater than zero.
    if ((nSeqNum >= m_nAddMoveIndex) || (nSeqNum <= m_knLowestSequenceNumber))
        return false;

    std::string sMoveArg = sMove + GameVocabulary::DELIMETER + sArg;
    m_mmMoves.insert(std::pair<int, std::string>(nSeqNum, sMoveArg));

    // Index is incremented to one more than sequence number to add
    // additional moves at a higher sequence number.
    m_nAddMoveIndex = nSeqNum + 1;

    // Allow callers to know that AllowedMoves is in use
    m_bInitialized = true;

    return true;
}

bool AllowedMoves::AddMove(const std::string &sMove, const std::string &sArg)
{
    std::string sMoveArg = sMove + GameVocabulary::DELIMETER + sArg;
    m_mmMoves.insert(std::pair<int, std::string>(m_knLowestSequenceNumber, sMove));
    return false;
}

void AllowedMoves::NextMoveInSequence(std::string &sMoves, const bool bIncrementIndex) const
{
    sMoves.clear();

    std::vector<GameMove> vGameMoves {};
    NextMoveInSequence(vGameMoves, bIncrementIndex);

    for (const auto &cGameMove : vGameMoves)
    {
        sMoves += cGameMove.Command() + GameVocabulary::DELIMETER + cGameMove.Argument() + "\n";
    }
}

void  AllowedMoves::NextMoveInSequence(std::vector<GameMove> &vGameMoves, const bool bIncrementIndex) const
{
    GameMove              cGameMove;
    vGameMoves.clear();

    // Get iterator range for moves associated with current move index
    std::pair <std::multimap<int, std::string>::const_iterator,
               std::multimap<int, std::string>::const_iterator> itRange = m_mmMoves.equal_range(m_nMoveIndex);

    // Add moves to to return variable
    for (std::multimap<int, std::string>::const_iterator it = itRange.first; it != itRange.second; ++it)
    {
        cGameMove.SetCommand(it->second);
        vGameMoves.push_back(cGameMove);
    }

    // Evaluate need to increment move index
    if (bIncrementIndex)
    {
        ++m_nMoveIndex;
        if (m_nMoveIndex == m_nAddMoveIndex)
            m_nMoveIndex = m_knLowestSequenceNumber + 1;
    }
}

bool AllowedMoves::ValidMove(const std::string &sMove, const std::string &sArg) const
{
    std::string sSeqMove {};
    std::string sSeqArg  {};
    std::size_t nPos     {};

    for (const auto &SequenceMove : m_mmMoves)
    {
        nPos     = SequenceMove.second.find(GameVocabulary::DELIMETER);
        sSeqMove = SequenceMove.second.substr(0,nPos - 1);
        sSeqArg  = SequenceMove.second.substr(nPos + 1);
        if ((sSeqMove.compare(sMove) == 0) && (sSeqArg.compare(sArg)))
        {
            return true;
        }
    }

    return false;
}
