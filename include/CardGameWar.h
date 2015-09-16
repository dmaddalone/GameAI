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
 * \brief The CardGameWar class represents the War card game with two
 * players.
 *
 */

#ifndef CARDGAMEWAR_H
#define CARDGAMEWAR_H

#include <unordered_map>

#include "CardGame.h"

class CardGameWar : public CardGame
{
    public:
        // Construct a War card game
        CardGameWar(GameType ecGameType) :
            CardGame(ecGameType, 2)
        {
            // Shuffle and Deal cards
            m_cDeck.Shuffle();
            m_cDeck.Deal(0, m_vHands);
            SetDrawingAllowed(true);
            SetDefaultMove(GameVocabulary::DRAW);

            m_cLogger.UseTag(false);
            m_cLogger.UseLevelIndent(false);
        }

        // Destructor
        ~CardGameWar() {}

        // Display the game
        virtual void Display() const override;

        // Synchronize the game environment across networked players
        virtual bool GetSyncInfo(std::string &sGameInformation) override;
        virtual bool ApplySyncInfo(const std::string &sGameInformation, std::string &sErrorMessage) override;

        // Generate a vector of valid moves
        virtual std::vector<GameMove> GenerateMoves(int nPlayer) const override;

        // Apply the move to the game
        virtual bool ApplyMove(int nPlayer, GameMove &cGameMove) override;

        // Return the score of the game
        virtual std::string GameScore() const override;

        // Check to see if the game has ended
        virtual bool GameEnded(int nPlayer) override;

        // Clone the current game
        virtual std::unique_ptr<Game> Clone() const override { return std::unique_ptr<Game>(new CardGameWar(*this)); }

        // Return the title of the game
        virtual std::string Title() const override { return "War"; }

        // Return the description of the game
        virtual std::string Description() const override { return "Aces are High.  Valid moves are " + GameVocabulary::DRAW + ", " + GameVocabulary::RESIGN + "."; }

        // Override Game::SetSync()
        void SetSync(bool b)  { CardGame::SetSync(b); m_bSyncBattle = m_bSyncWarCards = m_bSyncWar = b; }

    protected:
    private:
        // Return all ranks in the Battle
        std::string BattleRanks() const;

        // Serialize and deserialize Battle
        Json::Value BattleJsonSerialization() const;
        bool        BattleJsonDeserialization(const std::string &sJsonPlayingCards, std::string &sErrorMessage);

        // Return all ranks in the WarCards
        std::string WarCardsRanks() const;

        // Serialize and deserialize WarCards
        Json::Value WarCardsJsonSerialization() const;
        bool        WarCardsJsonDeserialization(const std::string &sJsonPlayingCards, std::string &sErrorMessage);

        // Battle used to hold cards ready to go to battle
        std::unordered_map<int, Card> m_uomBattle;

        // WarCards used to hold cards after battle but before distribution to winning player
        std::vector<Card>             m_vWarCards;

        // Flag whether a war has started
        bool                          m_bWar {false};

        // Sync flags
        bool m_bSyncBattle   { false };
        bool m_bSyncWarCards { false };
        bool m_bSyncWar      { false };
};

#endif // CARDGAMEWAR_H
