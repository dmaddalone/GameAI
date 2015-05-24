#include "ChessGardner.h"

void ChessGardner::SetBoard()
{
    GamePiece cPiece;
    //ChessPiece cPiece;

    ChessGame::SetBoard();

    cPiece.Set(m_kcRookToken, 1, 5);
    cBoard.SetPiece(0, 0, cPiece);
    cPiece.Set(m_kcKnightToken, 1, 3);
    cBoard.SetPiece(1, 0, cPiece);
    cPiece.Set(m_kcBishopToken, 1, 3);
    cBoard.SetPiece(2, 0, cPiece);
    cPiece.Set(m_kcQueenToken, 1, 9);
    cBoard.SetPiece(3, 0, cPiece);
    cPiece.Set(m_kcKingToken, 1, 0);
    cBoard.SetPiece(4, 0, cPiece);
    cPiece.Set(m_kcPawnToken, 1, 1);
    cBoard.SetPiece(0, 1, cPiece);
    cBoard.SetPiece(1, 1, cPiece);
    cBoard.SetPiece(2, 1, cPiece);
    cBoard.SetPiece(3, 1, cPiece);
    cBoard.SetPiece(4, 1, cPiece);

    cPiece.Set(m_kcRookToken, 2, 5);
    cBoard.SetPiece(0, 4, cPiece);
    cPiece.Set(m_kcKnightToken, 2, 3);
    cBoard.SetPiece(1, 4, cPiece);
    cPiece.Set(m_kcBishopToken, 2, 3);
    cBoard.SetPiece(2, 4, cPiece);
    cPiece.Set(m_kcQueenToken, 2, 9);
    cBoard.SetPiece(3, 4, cPiece);
    cPiece.Set(m_kcKingToken, 2, 0);
    cBoard.SetPiece(4, 4, cPiece);
    cPiece.Set(m_kcPawnToken, 2, 1);
    cBoard.SetPiece(0, 3, cPiece);
    cBoard.SetPiece(1, 3, cPiece);
    cBoard.SetPiece(2, 3, cPiece);
    cBoard.SetPiece(3, 3, cPiece);
    cBoard.SetPiece(4, 3, cPiece);

    return;
}
