#include "AllowedMoves.h"

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

bool AllowedMoves::AddMove(const std::string &sMove, const std::string &sArg)
{
    std::string sMoveArg = sMove + GameVocabulary::DELIMETER + sArg;
    m_mmMoves.insert(std::pair<int, std::string>(m_knLowestSequenceNumber, sMoveArg));
    return true;
}

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

void  AllowedMoves::NextMoveInSequence(std::vector<GameMove> &vGameMoves) const
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

