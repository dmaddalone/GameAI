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
    TYPE_TTT,
    TYPE_REVERSI
};

class Game
{
    public:
        Game(GameType ecGameType) { m_ecGameType = ecGameType; }
        virtual ~Game() {};

        virtual void Display(bool bDisplayCoordinates = false) const  = 0;
        virtual void DisplayValidMoves(int nPlayer, int nOpponent) const = 0;

        virtual GameMove GetMove(int nPlayer, int nOpponent) const = 0;
        virtual void AnnounceMove(int nPlayer, const GameMove &cGameMove) = 0;

        virtual int  ApplyMove(int nPlayer, GameMove &cGameMove) = 0;

        virtual bool RetractMove(int nPlayer, const GameMove &cGameMove) = 0;

        virtual int  PreferredMove(const GameMove &cGameMove) const = 0;
        virtual bool GameEnded(int nPlayer, int nOpponent) = 0;

        virtual std::vector<GameMove> GenerateMoves(int nPlayer, int nOpponent) const = 0;
        virtual int  EvaluateGameState(int nPlayer) = 0;

        static std::unique_ptr<Game> MakeGame(GameType ecGameType);

        virtual std::string Title() = 0;

        int  Player1()       { return m_knPlayer1; }
        int  Player2()       { return m_knPlayer2; }
        int  Winner()        { return m_nWinner; };
        int  NumberOfMoves() { return m_nNumberOfMoves; }
        std::string WinBy()  { return m_sWinBy; }

    protected:
        //void UpdatePlayerTurn() { if (m_nPlayerTurn == m_kPlayer1) m_nPlayerTurn = m_kPlayer2; else m_nPlayerTurn = m_kPlayer1; }

        const int m_knPlayer1  {1};
        const int m_knPlayer2  {2};

        //int m_nPlayerTurn     {m_kPlayer1};

        GameType m_ecGameType {GameType::TYPE_NONE};

        int m_nWinner         {0};
        int m_nNumberOfMoves  {0};
        std::string m_sWinBy  {};
};

#endif // GAME_H
