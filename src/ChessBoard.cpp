#include "ChessBoard.h"

/**
  * Set a piece on the board.
  *
  * If a valid location, set the token and the player.
  *
  * \param nX X-Coordinate
  * \param nY Y-Coordinate
  * \param cGamePiece Game piece object
  *
  * \return True if piece set, false otherwise.
  */

//bool GameBoard::SetPiece(int nX, int nY, const GamePiece &cGamePiece)
bool ChessBoard::SetPiece(int nX, int nY, const ChessPiece &cGamePiece)
{
    if (!ValidLocation(nX, nY))
        return false;

    m_vBoard[nY][nX].Set(cGamePiece.Token(), cGamePiece.Player(), cGamePiece.Value());

    return true;
}

/**
  * Move a piece on the board.
  *
  * Call GameBoard to move the piece, then mark it
  * as having moved.
  *
  * \param GameMove The game move
  *
  * \return True if valid move, false otherwise.
  */

bool ChessBoard::MovePiece(const GameMove &cGameMove)
{
    if (!GameBoard::MovePiece(cGameMove))
        return false;

    ChessPiece cChessPiece = m_vBoard[cGameMove.ToY()][cGameMove.ToX()].Piece();
    cChessPiece.SetMoved();

    return true;
}
