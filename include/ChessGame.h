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

/** \file
 *
 * \brief The ChessGame class represents a virtual chess game.
 *
 */

#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <climits>
//#include <cstdlib>
#include <iostream>
#include <string>

#include "BoardGame.h"
//#include "ChessPiece.h"
//#include "ChessBoard.h"

class ChessGame : public BoardGame
{
    public:
        // Construct a LinearGame
        ChessGame(GameType ecGameType, int nX, int nY, bool bDisplayGrid, bool bDisplayXCoordinates, bool bDisplayYCoordinates) :
            BoardGame(ecGameType, nX, nY, bDisplayGrid, bDisplayXCoordinates, bDisplayYCoordinates)
            { SetBoard(); }

        // Destructor
        virtual ~ChessGame() {}

        // Generate a GameMove from text input
        virtual GameMove GenerateMove(std::string sMove) const override;
        // Apply the move to the game
        virtual bool ApplyMove(int nPlayer, GameMove &cGameMove) override;
        // Announce the move made
        virtual std::string AnnounceMove(int nPlayer, const GameMove &cGameMove) const override;
        // Evaluate the game state from the perspective of the nPlayer
        virtual int  EvaluateGameState(int nPlayer) override;
        // Check to see if the game has ended
        virtual bool GameEnded(int nPlayer) override;
        // Generate a vector of valid moves
        virtual std::vector<GameMove> GenerateMoves(int nPlayer) const override;
        // Clone the current game
        virtual std::unique_ptr<Game> Clone() const = 0;
        virtual std::string Title() override;

        std::vector<GameMove> GenerateMovesForPiece(int nPlayer, const GameMove &cGameMove);

        void SetBoard() { BoardGame::SetBoard(); cBoard.ReverseY(); }

    protected:
        void GeneratePawnMoves(int nX, int nY, int nPlayer, std::vector<GameMove> &vGameMoves) const;
        void GenerateRookMoves(int nX, int nY, int nPlayer, std::vector<GameMove> &vGameMoves, bool bUnlimitedMoves=true) const;
        void GenerateBishopMoves(int nX, int nY, int nPlayer, std::vector<GameMove> &vGameMoves, bool bUnlimitedMoves=true) const;
        void GenerateQueenMoves(int nX, int nY, int nPlayer, std::vector<GameMove> &vGameMoves) const;
        void GenerateKingMoves(int nX, int nY, int nPlayer, std::vector<GameMove> &vGameMoves) const;
        void GenerateCastleMoves(int nKX, int nKY, int nPlayer, std::vector<GameMove> &vGameMoves) const;
        bool GenerateLinearMove(int nFromX, int nFromY, int nToX, int nToY, int nPlayer, std::vector<GameMove> &vGameMoves) const;
        void GenerateKnightMoves(int nX, int nY, int nPlayer, std::vector<GameMove> &vGameMoves) const;
        void GenerateKnightMove(int nFromX, int nFromY, int nToX, int nToY, int nPlayer, std::vector<GameMove> &vGameMoves) const;
        bool FindPiece(int &nX, int &nY, int nPlayer, char cToken) const;

        //const std::string sPieceSymbols {"RNBKQ"};
        //const std::string sFiles        {"abcdefgh"};
        //const std::string sRanks        {"12345678"};
        //const std::string sCaptures     {"x"};

        static const char m_kcPawnToken   {'P'};
        static const char m_kcRookToken   {'R'};
        static const char m_kcKnightToken {'N'};
        static const char m_kcBishopToken {'B'};
        static const char m_kcQueenToken  {'Q'};
        static const char m_kcKingToken   {'K'};

        bool m_abCastlingAllowed[2]   { true };
        bool m_bDoublePawnMoveAllowed { true };
};

#endif // CHESSGAME_H
