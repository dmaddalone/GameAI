#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "GameBoard.h"
#include "ChessPiece.h"

class ChessBoard : public GameBoard
{
    public:
        // Construct a game board of size X & Y, and display characteristics
        ChessBoard(int nX, int nY, bool bDisplayGrid, bool bDisplayXCoordinates, bool bDisplayYCoordinates) :
            GameBoard(nX, nY, bDisplayGrid, bDisplayXCoordinates, bDisplayYCoordinates)
        {}

        // Destructor
        ~ChessBoard() {}

        // Set a piece on the board
        bool SetPiece(int nX, int nY, const ChessPiece &cChessPiece);
        // Move a piece on the board
        bool MovePiece(const GameMove &cGameMove);

    protected:
        // Vector of vectors to represent the board
        typedef std::vector<ChessPiece> Row;
        typedef std::vector<Row>        Board;
        Board m_vBoard;
};

#endif // CHESSBOARD_H
