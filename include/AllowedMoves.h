#ifndef ALLOWEDMOVES_H
#define ALLOWEDMOVES_H

#include <string>
#include <vector>

#include "GameMove.h"

class AllowedMoves
{
    public:
        bool AddMovesInSequence(const int &nSeqNum, const std::string &sMove, const std::string &sArg="");
        bool AddMove(const std::string &sMove, const std::string &sArg="");
        void NextMoveInSequence(std::string &sMoves) const;
        void NextMoveInSequence(std::vector<GameMove> &vMoves) const;
        bool ValidMove(const std::string &sMove, const std::string &sArg="") const;
        void ProgressSequence() { if (++m_nMoveIndex == m_nAddMoveIndex) ResetSequence(); }
        void RevertSequence() { if (--m_nMoveIndex <= m_knLowestSequenceNumber) ResetSequence(); }
        void ResetSequence()  { m_nMoveIndex = m_knLowestSequenceNumber + 1; }
        bool InUse() const   { return m_bInitialized; }

    private:
        std::multimap<int, std::string> m_mmMoves                {};
        const int                       m_knLowestSequenceNumber {0};
        int                             m_nAddMoveIndex          {m_knLowestSequenceNumber  + 1};
        mutable int                     m_nMoveIndex             {m_nAddMoveIndex};
        bool                            m_bInitialized           {false};

};

#endif // ALLOWEDMOVES_H
