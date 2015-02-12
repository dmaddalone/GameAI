/*
    Copyright 2014 Dom Maddalone

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
 * \brief The Game class represents a virtual game.
 *
 */

#ifndef GAME_H
#define GAME_H

#include <memory>
#include <vector>

#include "GameMove.h"

enum class GameType
{
    TYPE_NONE,
    TYPE_CONNECT_FOUR,
    TYPE_TTT
};

class Game
{
    public:
        Game(GameType ecGameType) { m_ecGameType = ecGameType; }
        virtual ~Game() {};

        virtual void Display(const bool bDisplayCoordinates = false) const  = 0;
        virtual void DisplayValidMoves() const = 0;

        virtual GameMove GetMove() const = 0;
        virtual void AnnounceMove(const int nPlayer, const GameMove &cGameMove) = 0;

        virtual int  ApplyMove(const int nPlayer, GameMove &cGameMove) = 0;

        virtual bool RetractMove(const int nPlayer, const GameMove &cGameMove) = 0;

        virtual int  PreferredMove(const GameMove &cGameMove) const = 0;
        virtual bool GameEnded() = 0;

        virtual std::vector<GameMove> GenerateMoves() const = 0;
        virtual int  EvaluateGameState(const int nPlayer) = 0;

        static std::unique_ptr<Game> MakeGame(GameType ecGameType);

        virtual std::string Title() = 0;
        int  Winner() { return m_nWinner; };
        int  NumberOfMoves() { return m_nNumberOfMoves; }
        std::string WinBy() { return m_sWinBy; }

    protected:
        GameType m_ecGameType {GameType::TYPE_NONE};
        int m_nWinner {0};
        int m_nNumberOfMoves {0};
        std::string m_sWinBy {};
};

#endif // GAME_H
