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

#ifndef ALLOWEDMOVES_H
#define ALLOWEDMOVES_H

#include <string>
#include <vector>

#include "GameMove.h"

/** \file
 *
 * \brief The AllowedMoves class represents a set of allowed game moves based on
 * sequence.
 *
 */

class AllowedMoves
{
    public:
        // Add allowed moves
        bool AddMovesInSequence(const int &nSeqNum, const std::string &sMove, const std::string &sArg="");
        bool AddMove(const std::string &sMove, const std::string &sArg="");

        // Return a set of allowed moves
        void NextMoveInSequence(std::string &sMoves) const;
        void NextMoveInSequence(std::vector<GameMove> &vMoves) const;

        // Validate a move
        bool ValidMove(const std::string &sMove, const std::string &sArg="") const;

        // Set start of sequence, move to or back to the next set of allowed moves
        void ProgressSequence() { if (++m_nMoveIndex == m_nAddMoveIndex) ResetSequence(); }
        void RevertSequence() { if (--m_nMoveIndex <= m_knLowestSequenceNumber) ResetSequence(); }
        void ResetSequence()  { m_nMoveIndex = m_knLowestSequenceNumber + 1; }

        // Indicate whether allowed moves are in use
        bool InUse() const   { return m_bInitialized; }

    private:
        // Multimap of moves <Sequence Number, Move>
        std::multimap<int, std::string> m_mmMoves                {};

        // Indices to manage the sequences
        const int                       m_knLowestSequenceNumber {0};
        int                             m_nAddMoveIndex          {m_knLowestSequenceNumber  + 1};
        mutable int                     m_nMoveIndex             {m_nAddMoveIndex};

        // Indicates whether allowed moves is in use
        bool                            m_bInitialized           {false};
};

#endif // ALLOWEDMOVES_H
