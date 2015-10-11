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
 * \brief The AIPlayer class represents a computer game player, based
 * on the Minimax algorithm, using alpha-beta pruning, or a blackboard
 * approach.
 *
 */

#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <climits>
#include <iomanip>
#include <iostream>

#include "Blackboard.h"
#include "Game.h"
#include "Player.h"

class AIPlayer : public Player
{
    public:
        // Construct a AIPlayer object
        AIPlayer(PlayerType ecPlayerType) : Player(ecPlayerType) {};

        // Destructor
        ~AIPlayer() {};

        // Initializer
        virtual void Initialize(std::string sHost, int nPort, bool &bSwap) override { (void)sHost; (void)nPort; (void)bSwap; };

        // Generate the next game move
        virtual bool Move(Game &cGame) override;

        // Player actions at end of game
        virtual bool Finish(Game &cGame) override { (void)cGame; return true; }

        // Announce the type pf player
        std::string TypeName() const override { return "AIPlayer"; }

    private:
        // Generate the best move using the minimax algorithm
        GameMove MinimaxMove(int nPlayer, Game &cGame, int nDepth);
        // Generate a move by the opponent player
        int MinMove(int nPlayer, Game &cGame, int nDepth, int nAlpha, int nBeta);
        // Generate a move for the player
        int MaxMove(int nPlayer, Game &cGame, int nDepth, int nAlpha, int nBeta);

        // Blackboard
        Blackboard m_cBlackBoard;
};

#endif // AIPLAYER_H
