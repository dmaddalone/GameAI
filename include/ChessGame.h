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
#include <random>
#include <string>
#include <unordered_set>

#include "BoardGame.h"
#include "GameAIException.h"

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
        // Return the title of the game
        virtual std::string Title() override;

        //void SetBoard() { BoardGame::SetBoard(); cBoard.ReverseY(); }
        void SetBoard();

    protected:
        void GeneratePawnMoves(GameMove cGameMove,   int nPlayer, std::vector<GameMove> &vGameMoves) const;
        void GenerateRookMoves(GameMove cGameMove,   int nPlayer, std::vector<GameMove> &vGameMoves, bool bUnlimitedMoves=true) const;
        void GenerateBishopMoves(GameMove cGameMove, int nPlayer, std::vector<GameMove> &vGameMoves, bool bUnlimitedMoves=true) const;
        void GenerateQueenMoves(GameMove cGameMove,  int nPlayer, std::vector<GameMove> &vGameMoves) const;
        void GenerateKingMoves(GameMove cGameMove,   int nPlayer, std::vector<GameMove> &vGameMoves) const;
        void GenerateCastleMoves(GameMove cGameMove, int nPlayer, std::vector<GameMove> &vGameMoves) const;
        bool GenerateLinearMove(GameMove cGameMove,  int nPlayer, std::vector<GameMove> &vGameMoves) const;
        void GenerateKnightMoves(GameMove cGameMove, int nPlayer, std::vector<GameMove> &vGameMoves) const;
        void GenerateKnightMove(GameMove cGameMove,  int nPlayer, std::vector<GameMove> &vGameMoves) const;
        std::vector<GameMove> GenerateMovesForPiece(int nPlayer, const GameMove &cGameMove) const;
        bool FindPiece(int &nX, int &nY, int nPlayer, char cToken) const;

        bool TestForAdjacentKings(const GameMove &cGameMove, int nPlayer) const;
        void TestForCheck(int nPlayer, GameMove cGameMove, std::vector<GameMove> &vGameMoves) const;
        bool KingInCheck(int nPlayer) const;
        bool AttackingTheKing(int nKX, int nKY, int nPlayer, int nX, int nY) const;

        // Create a checksum of the game board
        int CheckSum() const;

        // Count the value of pieces for nPlayer
        int  CountEvaluation(int nPlayer) const;
        void CountPawns(int nPlayer, int &nDoubled, int &nIsolated, int &nPassed) const;

        std::unordered_multiset<int> m_uomsCheckSums {};
        static const int m_knMaxCheckSums {3};
        std::unordered_multiset<uint64_t> m_uomsZobrist {};
        static const int m_knMaxRepetition {3};
        uint64_t m_uiZobristKey;

        static const int m_knNumberOfPieces  {12};
        static const int m_knNumberOfSquares {64};
        uint64_t m_auiZobrist[12][64] {{}};

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

        static const int  m_knPawnValue   {1};
        static const int  m_knRookValue   {5};
        static const int  m_knKnightValue {3};
        static const int  m_knBishopValue {3};
        static const int  m_knQueenValue  {9};
        static const int  m_knKingValue   {4};

        static const int m_knWhitePawnIndex   {0};
        static const int m_knWhiteRookIndex   {1};
        static const int m_knWhiteKnightIndex {2};
        static const int m_knWhiteBishopIndex {3};
        static const int m_knWhiteQueenIndex  {4};
        static const int m_knWhiteKingIndex   {5};
        static const int m_knBlackPawnIndex   {6};
        static const int m_knBlackRookIndex   {7};
        static const int m_knBlackKnightIndex {8};
        static const int m_knBlackBishopIndex {9};
        static const int m_knBlackQueenIndex  {10};
        static const int m_knBlackKingIndex   {11};

        bool m_abCastlingAllowed[2]     { true };
        bool m_bDoublePawnMoveAllowed   { true };
        bool m_bEnPassantAllowed        { true };
        bool m_bAutomaticPromoteToQueen { true };
};

#endif // CHESSGAME_H
