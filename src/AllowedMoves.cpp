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

#include "AllowedMoves.h"

/**
  * Add a move to allowed moves in a particular sequence.
  *
  * Moves may be added only in sequence, e.g., Draw, Meld, Discard.  The method
  * adds the move and increments the sequence move counter.
  *
  * \param nSeqNum The sequence number associate with the move
  * \param sMove   The move
  * \param sArg    The argument for the move
  *
  * \return True if move was added, false otherwise.
  */

bool AllowedMoves::AddMovesInSequence(const int &nSeqNum, const std::string &sMove, const std::string &sArg)
{
    // May only insert moves with sequence less-than or equal to current index
    // and sequence number must be greater than zero.
    if ((nSeqNum > m_nAddMoveIndex) || (nSeqNum <= m_knLowestSequenceNumber))
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

/**
  * Add a move to allowed moves.  These are moves that may be played
  * at any time.
  *
  * \param sMove The move
  * \param sArg  The argument for the move
  *
  * \return True.
  */

bool AllowedMoves::AddMove(const std::string &sMove, const std::string &sArg)
{
    std::string sMoveArg = sMove + GameVocabulary::DELIMETER + sArg;
    m_mmMoves.insert(std::pair<int, std::string>(m_knLowestSequenceNumber, sMoveArg));
    return true;
}

/**
  * Return, as a output parameter, a string of game moves representing
  * the set of allowed moves in the sequence.
  *
  * Based on the current sequence index, return the allowed moves.
  *
  * \param sMoves String to hold the allowed game moves
  *
  */

void AllowedMoves::NextMoveInSequence(std::string &sMoves) const
{
    sMoves.clear();

    std::vector<GameMove> vGameMoves {};
    NextMoveInSequence(vGameMoves);

    for (const auto &cGameMove : vGameMoves)
    {
        sMoves += cGameMove.Command() + GameVocabulary::DELIMETER + cGameMove.Argument() + "\n";
    }
}

/**
  * Return, as a output parameter, a vector of game moves representing
  * the set of allowed moves in the sequence.
  *
  * Based on the current sequence index, return the allowed moves.
  *
  * \param vGameMoves Vector to hold the allowed game moves
  *
  */

void  AllowedMoves::NextMoveInSequence(std::vector<GameMove> &vGameMoves) const
{
    vGameMoves.clear();

    // Get iterator range for moves associated with current move index
    std::pair <std::multimap<int, std::string>::const_iterator,
               std::multimap<int, std::string>::const_iterator> itRange = m_mmMoves.equal_range(m_nMoveIndex);

    // Add moves to vector
    for (std::multimap<int, std::string>::const_iterator it = itRange.first; it != itRange.second; ++it)
    {
        vGameMoves.emplace_back();
        vGameMoves.back().SetCommand(it->second);
    }
}

/**
  * Validate that a move is part of the allowed moves.
  *
  * \param sMove The move to validate
  * \param sArg  The argument to validate
  *
  * \return True if move and argument are valid, false otherwise.
  */

bool AllowedMoves::ValidMove(const std::string &sMove, const std::string &sArg) const
{
    // Find move and argument parameters in the allowed moves
    for (const auto &SequenceMove : m_mmMoves)
    {
        std::size_t nPos     = SequenceMove.second.find(GameVocabulary::DELIMETER);
        std::string sSeqMove = SequenceMove.second.substr(0,nPos - 1);
        std::string sSeqArg  = SequenceMove.second.substr(nPos + 1);
        if ((sSeqMove.compare(sMove) == 0) && (sSeqArg.compare(sArg)))
        {
            return true;
        }
    }

    return false;
}

