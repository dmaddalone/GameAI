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
 * \brief The CardGame class represents a virtual card game with two
 * players.
 *
 */

#ifndef CARDGAME_H
#define CARDGAME_H

#include <climits>
#include <unordered_map>

#include "Game.h"
#include "GameMove.h"
#include "Deck.h"
#include "Hand.h"

#include "json/json.h"

class CardGame : public Game
{
    public:
        // Construct a CardGame
        CardGame(GameType ecGameType, int nNumberOfHands) :
            Game(ecGameType)
        {
            // Set sync to true to sync game information between networked players
            SetSync(true);

            // Setup hands (players)
            for (int iii = 0; iii < nNumberOfHands; ++iii)
            {
                m_vHands.emplace_back();
            }
        }

        // Destructor
        virtual ~CardGame() {}

        // Display the game
        virtual void Display() const override;

        // Synchronize the game environment across networked players
        virtual bool GetSyncInfo(std::string &sGameInformation) override;
        virtual bool ApplySyncInfo(const std::string &sGameInformation, std::string &sErrorMessage) override;

        // Return a list of valid moves in string format
        virtual std::string ValidMoves(int nPlayer) const override;

        // Get the move from the designated player
        virtual GameMove GetMove(int nPlayer) const override;

        // Generate a GameMove from text input
        virtual GameMove GenerateMove(std::string sMove) const override;

        // Provide a preferred move
        virtual int  PreferredMove(const GameMove &cGameMove) const override;

        // Announce the move made
        virtual std::string AnnounceMove(int nPlayer, const GameMove &cGameMove) const override;

        // Evaluate the game state from the perspective of the nPlayer
        virtual int  EvaluateGameState(int nPlayer) override;

        // Update Blackboard
        virtual void BlackboardUpdate(int nPlayer, Blackboard &cBlackboard) override { (void)nPlayer; (void)cBlackboard; return; }

        // Generate a move from the Blackboard
        virtual GameMove BlackboardMove(int nPlayer, Blackboard &cBlackboard, int nProbability) const override { (void)nPlayer; (void)cBlackboard; (void)nProbability; GameMove cGameMove; return cGameMove; }

        // Return the score of the game
        virtual std::string GameScore() const override;

        // Return the stats of the game
        virtual std::string GameStatistics() const override { return ""; };

        // Check to see if the game has ended
        virtual bool GameEnded(int nPlayer) override;

        // Clone the current game
        virtual std::unique_ptr<Game> Clone() const = 0;

        // Get flags
        bool FoldingAllowed() const                { return m_bFoldingAllowed; }
        bool DrawingAllowed() const                { return m_bDrawingAllowed; }
        bool DrawingFromStockAllowed() const       { return m_bDrawFromStockAllowed; }
        bool DrawingFromDiscardPileAllowed() const { return m_bDrawFromDiscardPileAllowed; }
        bool AskingAllowed() const                 { return m_bAskingAllowed; }
        bool MeldingAllowed() const                { return m_bMeldingAllowed; }
        bool DiscardingAllowed() const             { return m_bDiscardingAllowed; }
        bool ShowingAllowed() const                { return m_bShowingAllowed; }
        bool ScoringAllowed() const                { return m_bScoringAllowed; }

        // Override Game::SetSync()
        void SetSync(bool b)  { Game::SetSync(b); m_bSyncDeck = m_bSyncFirstHand = m_bSyncSecondHand = b; }

        // Get the default move for the game
        std::string DefaultMove() const { return m_sDefaultMove; }

    protected:
        // Initialize Blackboard
        virtual void BlackboardInitialize(int nPlayer, Blackboard &cBlackboard) const override { (void)nPlayer; (void)cBlackboard; return; }

        // Set flags
        void SetFoldingAllowed(bool b)                { m_bFoldingAllowed = b; }
        void SetDrawingAllowed(bool b)                { m_bDrawingAllowed = b; }
        void SetDrawingFromStockAllowed(bool b)       { m_bDrawFromStockAllowed = b; }
        void SetDrawingFromDiscardPileAllowed(bool b) { m_bDrawFromDiscardPileAllowed = b; }
        void SetAskingAllowed(bool b)                 { m_bAskingAllowed = b; }
        void SetMeldingAllowed(bool b)                { m_bMeldingAllowed = b; }
        void SetDiscardingAllowed(bool b)             { m_bDiscardingAllowed = b; }
        void SetShowingAllowed(bool b)                { m_bShowingAllowed = b; }
        void SetScoringAllowed(bool b)                { m_bScoringAllowed = b; }

        // AllowedMoves used for multiple moves with a single turn, i.e.,
        // Draw, Meld, Discard
        AllowedMoves m_cAllowedMoves;

        // Set the default move for the game
        void SetDefaultMove(std::string sMove) { m_sDefaultMove = sMove; }

        // Card game deck, hands, and books
        Deck m_cDeck;
        std::vector<Hand>             m_vHands {};

        // Used to initialize comparison variables
        const int m_knUnknownValue {-1};

    private:
        // Play option flags
        bool m_bFoldingAllowed             { false };
        bool m_bDrawingAllowed             { false };
        bool m_bDrawFromStockAllowed       { false };
        bool m_bDrawFromDiscardPileAllowed { false };
        bool m_bAskingAllowed              { false };
        bool m_bMeldingAllowed             { false };
        bool m_bDiscardingAllowed          { false };
        bool m_bShowingAllowed             { false };
        bool m_bScoringAllowed             { false };

        // Sync flags
        bool m_bSyncDeck       { false };
        bool m_bSyncFirstHand  { false };
        bool m_bSyncSecondHand { false };

        // Default move for the game
        std::string m_sDefaultMove {};
};

#endif // CARDGAME_H
