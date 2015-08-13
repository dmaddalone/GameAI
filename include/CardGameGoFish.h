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
 * \brief The CardGameGoFish class represents the Go Fish card game with
 * two players.
 *
 */


#ifndef CARDGAMEGOFISH_H
#define CARDGAMEGOFISH_H

#include <unordered_map>

#include "CardGame.h"

class CardGameGoFish : public CardGame
{
    public:
        // Construct a War card game
        CardGameGoFish(GameType ecGameType) :
            CardGame(ecGameType, 2)
        {
            // Shuffle and Deal cards
            m_cDeck.Shuffle();
            m_cDeck.Deal(7, m_vHands);
            SetAskingAllowed(true);
            //SetDefaultMove(GameVocabulary::ASK);
        }

        // Destructor
        ~CardGameGoFish() {}

        // Display the game
        virtual void Display() const override;

        // Return a list of valid moves in string format
        virtual std::string ValidMoves(int nPlayer) const override;

        // Generate a vector of valid moves
        virtual std::vector<GameMove> GenerateMoves(int nPlayer) const override;

        // Apply the move to the game
        virtual bool ApplyMove(int nPlayer, GameMove &cGameMove) override;

        // Announce the move made
        virtual std::string AnnounceMove(int nPlayer, const GameMove &cGameMove) const override;

        // Return the score of the game
        virtual std::string GameScore() const override;

        // Check to see if the game has ended
        virtual bool GameEnded(int nPlayer) override;

        // Clone the current game
        virtual std::unique_ptr<Game> Clone() const override { return std::unique_ptr<Game>(new CardGameGoFish(*this)); }

        // Return the title of the game
        virtual std::string Title() override { return "Go Fish"; }

        // Return the description of the game
        virtual std::string Description() override { return "Valid moves are " + GameVocabulary::ASK + ", " + GameVocabulary::RESIGN + "."; }

        bool GoFish(int nPlayer);

    protected:
    private:
        std::unordered_multimap<int, Hand> m_uommBooks;
        std::unordered_map<int, Card> m_uomBattle;
        std::vector<Card>             m_vWarCards;
        bool                          m_bWar {false};
};

#endif // CARDGAMEGOFISH_H
