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

#include "ChessGame.h"

/**
  * Generate a GameMove from a string.
  *
  * From a string, generate a GameMove object.
  *
  * move      ::= <square><square>[promotion]
  * square    ::= <'a'|'b'| ...><'1'|'2'|...>
  * promotion ::= <'='><piece>      // TODO
  * piece     ::= <'R'|'N'|'B'|'Q'> // TODO
  *
  * \param sMove A string representing a game move.
  *
  * \return A GameMove object.
  */

GameMove ChessGame::GenerateMove(std::string sMove) const
{
    GameMove cGameMove (sMove[0], sMove[1], sMove[2], sMove[3]);

    return cGameMove;
}

/**
  * Return a vector of valid game moves.
  *
  * Review and collect all valid moves for a player into a vector.
  *
  * \param nPlayer The player whose turn it is.
  *
  * \return A vector of valid moves.
  */

std::vector<GameMove> ChessGame::GenerateMoves(int nPlayer) const
{
    std::vector<GameMove> vGameMoves {};
    char cToken;

    for (int yyy = 0; yyy < m_knY; ++yyy)
    {
        for (int xxx = 0; xxx < m_knX; ++xxx)
        {
            if (cBoard.PositionOccupiedByPlayer(xxx, yyy, nPlayer))
            {
                cToken = cBoard.Token(xxx, yyy);
                if (cToken == m_kcPawnToken)
                    GeneratePawnMoves(xxx, yyy, nPlayer, vGameMoves);
                if (cToken == m_kcRookToken)
                    GenerateRookMoves(xxx, yyy, nPlayer, vGameMoves);
                if (cToken == m_kcKnightToken)
                    GenerateKnightMoves(xxx, yyy, nPlayer, vGameMoves);
                if (cToken == m_kcBishopToken)
                    GenerateKnightMoves(xxx, yyy, nPlayer, vGameMoves);
                if (cToken == m_kcQueenToken)
                    GenerateQueenMoves(xxx, yyy, nPlayer, vGameMoves);
                if (cToken == m_kcKingToken)
                    GenerateKingMoves(xxx, yyy, nPlayer, vGameMoves);
            }
        }
    }

    return vGameMoves;
}

/**
  * Generate moves for a pawn.
  *
  * Review and collect all valid moves for a pawn.
  *
  * \param nX The X-Coordinate of the pawn
  * \param nY The Y-Coordinate of the pawn
  * \param nPlayer The player whose turn it is.
  * \param vGameMoves The vector to add valid moves to
  */

void ChessGame::GeneratePawnMoves(int nX, int nY, int nPlayer, std::vector<GameMove> &vGameMoves) const
{
    int nNewY;

    // Push
    if (nPlayer == 1)
        nNewY = nY + 1;
    else
        nNewY = nY - 1;

    if (!cBoard.PositionOccupied(nX, nNewY))
        vGameMoves.emplace_back(nX, nY, nX, nNewY, true);

    // Capture

    if (cBoard.PositionOccupiedByPlayer(nX - 1, nNewY, 1 - nPlayer + 2))
        vGameMoves.emplace_back(nX, nY, nX - 1, nNewY, true);
    if (cBoard.PositionOccupiedByPlayer(nX + 1, nNewY, 1 - nPlayer + 2))
        vGameMoves.emplace_back(nX, nY, nX + 1, nNewY, true);

    // TODO: Promotions
    // TODO: Pawn double move
    // TODO: En passant
}

/**
  * Generate moves for a Rook, Queen, or King.
  *
  * Review and collect all valid moves for a Rook, Queen, or King
  *
  * \param nX The X-Coordinate of the Rook, Queen, or King
  * \param nY The Y-Coordinate of the Rook, Queen, or King
  * \param nPlayer The player whose turn it is.
  * \param vGameMoves The vector to add valid moves to
  */

void ChessGame::GenerateRookMoves(int nX, int nY, int nPlayer, std::vector<GameMove> &vGameMoves, bool bUnlimitedMoves) const
{
    int nNewX;
    int nNewY;

    // Evaluate north along file
    nNewY = nY + 1;
    while (cBoard.ValidLocation(nX, nNewY))
    {
        if (GenerateLinearMove(nX, nY, nX, nNewY, nPlayer, vGameMoves))
            ++nNewY;
        else
            break;

        if (!bUnlimitedMoves) // Limit the King move to one square
            break;
    }

    // Evaluate south along file
    nNewY = nY - 1;
    while (cBoard.ValidLocation(nX, nNewY))
    {
        if (GenerateLinearMove(nX, nY, nX, nNewY, nPlayer, vGameMoves))
            --nNewY;
        else
            break;

        if (!bUnlimitedMoves) // Limit the King move to one square
            break;
    }

    // Evaluate east along rank
    nNewX = nX - 1;
    while (cBoard.ValidLocation(nNewX, nY))
    {
        if (GenerateLinearMove(nX, nY, nNewX, nY, nPlayer, vGameMoves))
            --nNewX;
        else
            break;

        if (!bUnlimitedMoves) // Limit the King move to one square
            break;
    }

    // Evaluate west along rank
    nNewX = nX + 1;
    while (cBoard.ValidLocation(nNewX, nY))
    {
        if (GenerateLinearMove(nX, nY, nNewX, nY, nPlayer, vGameMoves))
            ++nNewX;
        else
            break;

        if (!bUnlimitedMoves) // Limit the King move to one square
            break;
    }
}

/**
  * Generate moves for a Bishop, Queen or King.
  *
  * Review and collect all valid moves for a Bishop, Queen, or a King
  *
  * \param nX The X-Coordinate of the Bishop, Queen, or King
  * \param nY The Y-Coordinate of the Bishop, Queen, or King
  * \param nPlayer The player whose turn it is.
  * \param vGameMoves The vector to add valid moves to
  */

void ChessGame::GenerateBishopMoves(int nX, int nY, int nPlayer, std::vector<GameMove> &vGameMoves, bool bUnlimitedMoves) const
{
    int nNewX;
    int nNewY;

    // Evaluate north-west
    nNewX = nX + 1;
    nNewY = nY + 1;
    while (cBoard.ValidLocation(nNewX, nNewY))
    {
        if (GenerateLinearMove(nX, nY, nNewX, nNewY, nPlayer, vGameMoves))
        {
            ++nNewX;
            ++nNewY;
        }
        else
            break;

        if (!bUnlimitedMoves) // Limit the King move to one square
            break;
    }

    // Evaluate south-west
    nNewX = nX + 1;
    nNewY = nY - 1;
    while (cBoard.ValidLocation(nNewX, nNewY))
    {
        if (GenerateLinearMove(nX, nY, nNewX, nNewY, nPlayer, vGameMoves))
        {
            ++nNewX;
            --nNewY;
        }
        else
            break;

        if (!bUnlimitedMoves) // Limit the King move to one square
            break;
    }

    // Evaluate south-east
    nNewX = nX - 1;
    nNewY = nY - 1;
    while (cBoard.ValidLocation(nNewX, nNewY))
    {
        if (GenerateLinearMove(nX, nY, nNewX, nNewY, nPlayer, vGameMoves))
        {
            ++nNewX;
            ++nNewY;
        }
        else
            break;

        if (!bUnlimitedMoves) // Limit the King move to one square
            break;
    }

    // Evaluate north-east
    nNewX = nX - 1;
    nNewY = nY + 1;
    while (cBoard.ValidLocation(nNewX, nNewY))
    {
        if (GenerateLinearMove(nX, nY, nNewX, nNewY, nPlayer, vGameMoves))
        {
            ++nNewX;
            ++nNewY;
        }
        else
            break;

        if (!bUnlimitedMoves) // Limit the King move to one square
            break;
    }
}

/**
  * Generate moves for a Queen.
  *
  * Review and collect all valid moves for a Queen
  *
  * \param nX The X-Coordinate of the Queen
  * \param nY The Y-Coordinate of the Queen
  * \param nPlayer The player whose turn it is.
  * \param vGameMoves The vector to add valid moves to
  */

void ChessGame::GenerateQueenMoves(int nX, int nY, int nPlayer, std::vector<GameMove> &vGameMoves) const
{
    GenerateRookMoves(nX, nY, nPlayer, vGameMoves);
    GenerateBishopMoves(nX, nY, nPlayer, vGameMoves);
}

/**
  * Generate moves for a King.
  *
  * Review and collect all valid moves for a King
  *
  * \param nX The X-Coordinate of the King
  * \param nY The Y-Coordinate of the King
  * \param nPlayer The player whose turn it is.
  * \param vGameMoves The vector to add valid moves to
  */

void ChessGame::GenerateKingMoves(int nX, int nY, int nPlayer, std::vector<GameMove> &vGameMoves) const
{
    GenerateRookMoves(nX, nY, nPlayer, vGameMoves, false);
    GenerateBishopMoves(nX, nY, nPlayer, vGameMoves, false);

    if (m_bCastlingAllowed)
    {
        //  Find King
        //  If King has not moved
        //      Find West Rook
        //      If found
        //          GenerateCastleMove(nKX, nKY, nRX, nRY, vGameMoves)
        //      Find East Rook
        //      If found
        //          GenerateCastleMove(nKX, nKY, nRX, nRY,  vGameMoves)
        //
        // GenerateCastleMove(nKX, nKY, nRX, nRY, vGameMove)
        //  Rook = cBoard.Piece()
        //  If Rook has not moved and
        //    no piece exists between West Rook and King
        //      vGameMoves.emplace_back(cKing.

        //ChessPiece cKing = cBoard.Piece(nX, nY);
        // if (!cKing.HasMoved())
        // {
        //
        // }
    }

    // TODO: Evaluate moves for check
    // TODO: Castling
    // TODO: Evaluate moves for stalemate
}

bool ChessGame::GenerateLinearMove(int nFromX, int nFromY, int nToX, int nToY, int nPlayer, std::vector<GameMove> &vGameMoves) const
{
    // Move
    if (!cBoard.PositionOccupied(nToX, nToY))
    {
        vGameMoves.emplace_back(nFromX, nFromY, nToX, nToY, true);
        return true;
    }
    else if (cBoard.PositionOccupiedByPlayer(nToX, nToY, nPlayer))
        return false;

    // Capture
    else //if (cBoard.PositionOccupiedByPlayer(nToX, nToY, 1 - nPlayer + 2))
    {
        vGameMoves.emplace_back(nFromX, nFromY, nToX, nToY, true);
        return false;
    }
}

/**
  * Generate moves for a Knight.
  *
  * Review and collect all valid moves for a Knight
  *
  * \param nX The X-Coordinate of the Knight
  * \param nY The Y-Coordinate of the Knight
  * \param nPlayer The player whose turn it is.
  * \param vGameMoves The vector to add valid moves to
  */

void ChessGame::GenerateKnightMoves(int nX, int nY, int nPlayer, std::vector<GameMove> &vGameMoves) const
{
    int nNewX;
    int nNewY;

    // Two up, one right
    nNewX = nX + 2;
    nNewY = nY + 1;
    GenerateKnightMove(nX, nY, nNewX, nNewY, nPlayer, vGameMoves);

    // One up, two right
    nNewX = nX + 1;
    nNewY = nY + 2;
    GenerateKnightMove(nX, nY, nNewX, nNewY, nPlayer, vGameMoves);

    // One down, two right
    nNewX = nX - 1;
    nNewY = nY + 2;
    GenerateKnightMove(nX, nY, nNewX, nNewY, nPlayer, vGameMoves);

    // Two down, one right
    nNewX = nX - 2;
    nNewY = nY + 1;
    GenerateKnightMove(nX, nY, nNewX, nNewY, nPlayer, vGameMoves);

    // Two down, one left
    nNewX = nX - 2;
    nNewY = nY - 1;
    GenerateKnightMove(nX, nY, nNewX, nNewY, nPlayer, vGameMoves);

    // One down, two left
    nNewX = nX - 1;
    nNewY = nY - 2;
    GenerateKnightMove(nX, nY, nNewX, nNewY, nPlayer, vGameMoves);

    // One up, two left
    nNewX = nX + 1;
    nNewY = nY - 2;
    GenerateKnightMove(nX, nY, nNewX, nNewY, nPlayer, vGameMoves);

    // Two up, one left
    nNewX = nX + 2;
    nNewY = nY - 1;
    GenerateKnightMove(nX, nY, nNewX, nNewY, nPlayer, vGameMoves);
}

void ChessGame::GenerateKnightMove(int nFromX, int nFromY, int nToX, int nToY, int nPlayer, std::vector<GameMove> &vGameMoves) const
{
    if (cBoard.ValidLocation(nToX, nToY))
    {
        if ((!cBoard.PositionOccupied(nToX, nToY)) || (cBoard.PositionOccupiedByPlayer(nToX, nToY, 1 - nPlayer + 2)))
            vGameMoves.emplace_back(nFromX, nFromY, nToX, nToY, true);
    }
}

/**
  * Apply a move to the game.
  *
  * After checking to for a move's validity, apply the move to the game.
  *
  * \param nPlayer   The player whose turn it is.
  * \param cGameMove The player's move
  *
  * \return True, if valid.  False otherwise.
  */

bool ChessGame::ApplyMove(int nPlayer, GameMove &cGameMove)
{
    bool bValidMove = false;

    // Check player number
    if ((nPlayer != m_knPlayer1) && (nPlayer != m_knPlayer2))
        return false;

    // Ensure we are on the board
    if (!cBoard.ValidLocation(cGameMove.ToX(), cGameMove.ToY()))
        return false;

    // Generate a vector of all possible valid moves for the from square
    std::vector<GameMove> vGameMoves = GenerateMovesForPiece(nPlayer, cGameMove);

    // Compare passed GameMove to generated game moves.  If one is found to be
    // the same, make the move on the board.
    for (GameMove &cValidGameMove : vGameMoves)
    {
        if (cValidGameMove.SameTo(cGameMove))
        {
            if (cBoard.MovePiece(cGameMove))
            {
                bValidMove = true;
                break;
            }
            else
                return false;
        }
    }

    // Increment move counter
    ++m_nNumberOfMoves;

    // Capture move for later playback or analysis
    m_vGameMoves.push_back(cGameMove);

/*
    char cToken = cBoard.Token(cGameMove.FromX(), cGameMove.FromY());

    if ((cToken == m_kcRookToken) || (cToken == m_kcKingToken)) // TODO: Need to update for multiple rooks
    {
        if (nPlayer == 1)
            m_bCastlingAllowedForPlayer1 = false;
        else
            m_bCastlingAllowedForPlayer2 = false;
    }
*/

    return bValidMove;
}

/**
  * Generate a vector of possible moves for a piece located at GameMove
  * coordinates.
  *
  * \param nPlayer   The player whose turn it is.
  * \param cGameMove The player's move
  *
  * \return A vector of possible moves.
  */

std::vector<GameMove> ChessGame::GenerateMovesForPiece(int nPlayer, const GameMove &cGameMove)
{
    std::vector<GameMove> vGameMoves {};
    char cToken;

    cToken = cBoard.Token(cGameMove.FromX(), cGameMove.FromY());
    if (cToken == m_kcPawnToken)
        GeneratePawnMoves(cGameMove.FromX(), cGameMove.FromY(), nPlayer, vGameMoves);
    if (cToken == m_kcRookToken)
        GenerateRookMoves(cGameMove.FromX(), cGameMove.FromY(), nPlayer, vGameMoves);
    if (cToken == m_kcKnightToken)
        GenerateKnightMoves(cGameMove.FromX(), cGameMove.FromY(), nPlayer, vGameMoves);
    if (cToken == m_kcBishopToken)
        GenerateBishopMoves(cGameMove.FromX(), cGameMove.FromY(), nPlayer, vGameMoves);
    if (cToken == m_kcQueenToken)
        GenerateQueenMoves(cGameMove.FromX(), cGameMove.FromY(), nPlayer, vGameMoves);
    if (cToken == m_kcKingToken)
        GenerateKingMoves(cGameMove.FromX(), cGameMove.FromY(), nPlayer, vGameMoves);

    return vGameMoves;
}

/**
  * Announce game move.
  *
  * Construct and return a string containing the move.
  *
  * \param nPlayer   The player whose turn it is.
  * \param cGameMove The player's move
  *
  * \return A string containing the move.
  */

std::string ChessGame::AnnounceMove(int nPlayer, const GameMove &cGameMove) const
{
    std::string sMessage = "Move number: " + std::to_string(m_nNumberOfMoves + 1) + "  Player: " + std::to_string(nPlayer) +
        "  Moves From: " + cGameMove.AnnounceFromMove() +" To: " + cGameMove.AnnounceToMove();

    return sMessage;
}

/**
  * Evaluate the game state.
  *
  * From a player's perspective, return a value cooresponding to the player's
  * standing in the game.  If the player has won the game, return a large,
  * positive integer.  If winning, a smaller, positive integer. If lost or
  * losing, a negative integer.
  *
  * \param nPlayer   The player whose turn it is.
  *
  * \return An integer representing game state for the player.
  */

int ChessGame::EvaluateGameState(int nPlayer)
{
    // If won, return largest positive integer // TODO: make these constants
    if (m_nWinner == nPlayer)
        return INT_MAX;

    // If lost, return largest negative integer // TODO: make these constants
    if (m_nWinner == (1 - nPlayer + 2))
        return INT_MIN;

    return 0;
}

bool ChessGame::GameEnded(int nPlayer)
{
    // Clear win variables
    m_nWinner = 0;
    m_sWinBy.assign("nothing");

    // Evaluate whether the player has any valid moves to make
    std::vector<GameMove> vGameMoves = GenerateMoves(nPlayer);
    if (vGameMoves.empty())
    {
        return true;
    }

    return false;
}
