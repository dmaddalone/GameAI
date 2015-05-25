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

std::string ChessGame::Title()
{
    std::string sMessage;

    if (m_abCastlingAllowed[0])
    {
        sMessage += "\nCastling is Allowed";
    }
    else
    {
        sMessage += "\nCastling is Not Allowed";
    }

    if (m_bDoublePawnMoveAllowed)
    {
        sMessage += "\nDouble Pawn is Allowed";
    }
    else
    {
        sMessage += "\nDouble Pawn is Not Allowed";
    }


    return sMessage;
}

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
                    GenerateBishopMoves(xxx, yyy, nPlayer, vGameMoves);

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
        vGameMoves.emplace_back(nX, nY, nX, nNewY, true, true);

    // Capture
    if (cBoard.PositionOccupiedByPlayer(nX - 1, nNewY, 1 - nPlayer + 2))
        vGameMoves.emplace_back(nX, nY, nX - 1, nNewY, true, true);
    if (cBoard.PositionOccupiedByPlayer(nX + 1, nNewY, 1 - nPlayer + 2))
        vGameMoves.emplace_back(nX, nY, nX + 1, nNewY, true, true);

    // Double move
    if (m_bDoublePawnMoveAllowed)
    {
        GamePiece cPiece = cBoard.Piece(nX, nY);
        if (!cPiece.HasMoved())
        {
            if (nPlayer == 1)
                nNewY = nY + 2;
            else
                nNewY = nY - 2;

            if (!cBoard.PositionOccupied(nX, nNewY))
                vGameMoves.emplace_back(nX, nY, nX, nNewY, true, true);
        }
    }

    // TODO: Promotions
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
            --nNewX;
            --nNewY;
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
            --nNewX;
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

void ChessGame::GenerateKingMoves(int nKX, int nKY, int nPlayer, std::vector<GameMove> &vGameMoves) const
{
    GenerateRookMoves(nKX, nKY, nPlayer, vGameMoves, false);
    GenerateBishopMoves(nKX, nKY, nPlayer, vGameMoves, false);
    GenerateCastleMoves(nKX, nKY, nPlayer, vGameMoves);

    // TODO: Evaluate moves for check
    // TODO: Castling
    // TODO: Evaluate moves for stalemate
}

void ChessGame::GenerateCastleMoves(int nKX, int nKY, int nPlayer, std::vector<GameMove> &vGameMoves) const
{
    if (m_abCastlingAllowed[nPlayer - 1])
    {
        int nRX    {0};
        int nRY    {0};
        int nNewKX {0};
        bool bCastleValid {false};

        while (FindPiece(nRX, nRY, nPlayer, m_kcRookToken))
        {
            GamePiece cRook = cBoard.Piece(nRX, nRY);
            if (!cRook.HasMoved())
            {
                bCastleValid = true;

                if (nRX < nKX)
                {
                    nNewKX = nKX - 2;
                    for (int xxx = nRX + 1; xxx != nKX; ++xxx)
                    {
                        if (cBoard.PositionOccupied(xxx, nRY))
                        {
                            bCastleValid = false;
                            break;
                        }
                    }
                }
                else
                {
                    nNewKX = nKX + 2;
                    for (int xxx = nRX - 1; xxx != nKX; --xxx)
                    {
                        if (cBoard.PositionOccupied(xxx, nRY))
                        {
                            bCastleValid = false;
                            break;
                        }
                    }
                }

                if (bCastleValid)
                {
                    vGameMoves.emplace_back(nKX, nKY, nNewKX, nKY, true, true);
                }
            }

            if (nRX < m_knX - 1)
            {
                ++nRX;
            }
            else
            {
                nRX = 0;
                ++nRY;
            }

        }
    }
}

bool ChessGame::GenerateLinearMove(int nFromX, int nFromY, int nToX, int nToY, int nPlayer, std::vector<GameMove> &vGameMoves) const
{
    // Move
    if (!cBoard.PositionOccupied(nToX, nToY))
    {
        vGameMoves.emplace_back(nFromX, nFromY, nToX, nToY, true, true);
        return true;
    }
    else if (cBoard.PositionOccupiedByPlayer(nToX, nToY, nPlayer))
        return false;

    // Capture
    else //if (cBoard.PositionOccupiedByPlayer(nToX, nToY, 1 - nPlayer + 2))
    {
        vGameMoves.emplace_back(nFromX, nFromY, nToX, nToY, true, true);
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
            vGameMoves.emplace_back(nFromX, nFromY, nToX, nToY, true, true);
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
    char cToken;

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

    // Perform Pawn Promotion
    cToken = cBoard.Token(cGameMove.ToX(), cGameMove.ToY());
    if ((cToken == m_kcPawnToken) && (cGameMove.ToY() == m_knY -1))
    {
        bool bGoodToken = false;
        char cPromotion {};

        GamePiece cGamePiece = cBoard.Piece(cGameMove.ToX(), cGameMove.ToY());

        while (!bGoodToken)
        {
            std::cout << "\nPAWN PROMOTION ("
                << m_kcRookToken << ", "
                << m_kcKnightToken << ", "
                << m_kcBishopToken << ", "
                << m_kcQueenToken << "): ";

            std::cin >> cPromotion;
            cPromotion = toupper(cPromotion);

            switch (cPromotion)
            {
                case m_kcRookToken:
                    cToken = cPromotion;
                    cGamePiece.Set(cPromotion, nPlayer, m_knRookValue);
                    cBoard.SetPiece(cGameMove.ToX(), cGameMove.ToY(), cGamePiece);
                    bGoodToken = true;
                    break;
                case m_kcKnightToken:
                    cToken = cPromotion;
                    cGamePiece.Set(cPromotion, nPlayer, m_knKnightValue);
                    cBoard.SetPiece(cGameMove.ToX(), cGameMove.ToY(), cGamePiece);
                    bGoodToken = true;
                    break;
                case m_kcBishopToken:
                    cToken = cPromotion;
                    cGamePiece.Set(cPromotion, nPlayer, m_knBishopValue);
                    cBoard.SetPiece(cGameMove.ToX(), cGameMove.ToY(), cGamePiece);
                    bGoodToken = true;
                    break;
                case m_kcQueenToken:
                    cToken = cPromotion;
                    cGamePiece.Set(cPromotion, nPlayer, m_knQueenValue);
                    cBoard.SetPiece(cGameMove.ToX(), cGameMove.ToY(), cGamePiece);
                    bGoodToken = true;
                    break;
                default:
                    std::cout << "Invalid entry: " << cPromotion << std::endl;
                    break;
            }
        }
    }

    // Perform Castling Move
    if (m_abCastlingAllowed[nPlayer -1])
    {
        cToken = cBoard.Token(cGameMove.ToX(), cGameMove.ToY());
        if (cToken == m_kcKingToken)
        {
            int nX;
            int nY;
            bool bCastleWest = false;

            if (abs(cGameMove.FromX() - cGameMove.ToX()) > 1)
            {
                bValidMove = false;

                //
                // Find Rook
                //

                // If King moved more than one square to the west
                if (cGameMove.FromX() - cGameMove.ToX() > 1)
                {
                    nX = 0;
                    bCastleWest = true;
                }
                // Else King moved more than one square to the west
                else
                {
                    nX = cGameMove.FromX() + 2;
                }

                nY = cGameMove.FromY();

                if (FindPiece(nX, nY, nPlayer, m_kcRookToken))
                {
                    //
                    // Move Rook
                    //
                    cGameMove.SetFromX(nX);
                    if (bCastleWest)
                    {
                        cGameMove.SetToX(cGameMove.ToX() + 1);
                    }
                    else
                    {
                        cGameMove.SetToX(cGameMove.ToX() - 1);
                    }

                    if (cBoard.MovePiece(cGameMove))
                    {
                        bValidMove = true;
                        m_abCastlingAllowed[nPlayer - 1] = false;
                    }
                }
            }
        }
    }

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
  * Find a chess piece.
  *
  * Using the passed coordinates as a starting place, find a piece on the
  * board moving left to right and up each row.  If the peice is found, update
  * the passed coordinates with the location and return true.
  *
  * \param nX      The X-Coordinate location to being the search
  * \param nY      The Y-Coordinate location to being the search
  * \param nPlayer The player to search for
  * \param cToken  The piece token to search for
  *
  * \return True if piece found, false otherwise.
  */

bool ChessGame::FindPiece(int &nX, int &nY, int nPlayer, char cToken) const
{
    for (int yyy = nY; yyy < m_knY; ++yyy)
    {
        for (int xxx = nX; xxx < m_knX; ++xxx)
        {
            if ((cBoard.Token(xxx, yyy) == cToken) && (cBoard.Player(xxx, yyy) == nPlayer))
            {
                nX = xxx;
                nY = yyy;
                return true;
            }
        }
    }

    return false;
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
