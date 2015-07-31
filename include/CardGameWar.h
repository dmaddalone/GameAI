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
 * \brief The CardGameWar class represents the War card game with two or more
 * players.
 *
 */


#ifndef CARDGAMEWAR_H
#define CARDGAMEWAR_H

#include <map>

#include "CardGame.h"

class CardGameWar : public CardGame
{
    public:
        // Construct a War card game
        CardGameWar(GameType ecGameType, int nNumberOfHands) :
            CardGame(ecGameType, nNumberOfHands)
        {
            // Deal cards
            m_cDeck.Deal(0, m_vHands);
        }

        // Destructor
        virtual ~CardGameWar();

        // Generate a vector of valid moves
        virtual std::vector<GameMove> GenerateMoves(int nPlayer) const override;

        // Apply the move to the game
        virtual bool ApplyMove(int nPlayer, GameMove &cGameMove) override;

        // Announce the move made
        virtual std::string AnnounceMove(int nPlayer, const GameMove &cGameMove) const override;

        // Evaluate the game state from the perspective of the nPlayer
        virtual int  EvaluateGameState(int nPlayer) override;

        // Return a list of valid moves in string format
        virtual std::string ValidMoves(int nPlayer) const override;

        // Clone the current game
        virtual std::unique_ptr<Game> Clone() const override { return std::unique_ptr<Game>(new CardGameWar(*this)); }

        // Return the title of the game
        virtual std::string Title() override { return "War"; }

        // Add player's card to the battle


    protected:
    private:
        std::map<int, Card> m_mBattle;
};

#endif // CARDGAMEWAR_H