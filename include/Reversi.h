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
 * \brief The Reversi class represents a Reversi game.
 *
 */

#ifndef REVERSI_H
#define REVERSI_H

#include "LinearGame.h"

class Reversi : public LinearGame
{
    public:
        // Construct a Reversi game, and set up the board
        Reversi(GameType ecGameType) : LinearGame(ecGameType, m_kiDimension, m_kiDimension, 'D', 'L', 0, true, true, true, true)
        { SetBoard(); }

        // Deconstructor
        ~Reversi() {}

        // Apply the move to the game
        virtual bool ApplyMove(int nPlayer, GameMove &cGameMove) override;

        // Evaluate the game state from the perspective of the nPlayer
        virtual int  EvaluateGameState(int nPlayer) override;

        // Return the score of the game
        virtual std::string GameScore() const override;

        // Check to see if the game has ended
        virtual bool GameEnded(int nPlayer) override;

        // Generate a vector of valid moves
        virtual std::vector<GameMove> GenerateMoves(int nPlayer) override;

        // Clone the current game
        virtual std::unique_ptr<Game> Clone() const override { return std::unique_ptr<Game>(new Reversi(*this)); }

        // Return the title of the game
        virtual std::string Title() const override { return "Reversi"; }

        // Return the description of the game
        virtual std::string Description() const override { return ""; }

    private:
        // Set up the board for initial play
        void SetBoard();

        // A set of methods to check for contiguous tokens along all lines
        bool Contiguous(int nX, int nY, int nPlayer) const;
        bool ContiguousUp(int nX, int nY, int nPlayer) const;
        bool ContiguousUpRight(int nX, int nY, int nPlayer) const;
        bool ContiguousRight(int nX, int nY, int nPlayer) const;
        bool ContiguousDownRight(int nX, int nY, int nPlayer) const;
        bool ContiguousDown(int nX, int nY, int nPlayer) const;
        bool ContiguousDownLeft(int nX, int nY, int nPlayer) const;
        bool ContiguousLeft(int nX, int nY, int nPlayer) const;
        bool ContiguousUpLeft(int nX, int nY, int nPlayer) const;
        bool CheckContiguous(int nX, int nY, int nPlayer, bool &bOpponentPieceAdjacent) const;

        // A set of methods to change tokens (aka, flip) along all lines
        void Flip(int nPlayer, const GameMove &cGameMove);
        void FlipUp(int nX, int nY, int nPlayer);
        void FlipUpRight(int nX, int nY, int nPlayer);
        void FlipRight(int nX, int nY, int nPlayer);
        void FlipDownRight(int nX, int nY, int nPlayer);
        void FlipDown(int nX, int nY, int nPlayer);
        void FlipDownLeft(int nX, int nY, int nPlayer);
        void FlipLeft(int nX, int nY, int nPlayer);
        void FlipUpLeft(int nX, int nY, int nPlayer);
        bool FlipToken(int nX, int nY, int nPlayer);

        // Count the number of tokens for nPlayer
        int  CountEvaluation(int nPlayer) const;
        // Count the positional value of tokens for nPlayer
        int  SquareEvaluation(int nPlayer) const;
        // Count the number of valid moves for nPlayer
        int  MobilityEvaluation(int nPlayer);

        // Board dimension
        static const int m_kiDimension {8};

        // Static evaluation table
        const int m_kaiEvalTable[m_kiDimension][m_kiDimension] =
        {
            { 99,  -8,  8,  6,  6,  8,  -8, 99 },
            { -8, -24, -4, -3, -3, -4, -24, -8 },
            {  8,  -4,  7,  4,  4,  7,  -4,  8 },
            {  6,  -3,  4,  0,  0,  4,  -3,  6 },
            {  6,  -3,  4,  0,  0,  4,  -3,  6 },
            {  8   -4,  7,  4,  4,  7,  -4,  8 },
            { -8, -24, -4, -3, -3, -4, -24, -8 },
            { 99,  -8,  8,  6,  6,  8,  -8, 99 }
        };
};

#endif // REVERSI_H
