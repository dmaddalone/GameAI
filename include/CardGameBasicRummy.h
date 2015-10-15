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
 * \brief The CardGameBasicRummy class represents the Basic Rummy card game with
 * two players.
 *
 */


#ifndef CARDGAMEBASICRUMMY_H
#define CARDGAMEBASICRUMMY_H

#include <unordered_map>

#include "CardGame.h"

class CardGameBasicRummy : public CardGame
{
    public:
        // Construct a Basic Rummy card game
        explicit CardGameBasicRummy(GameType ecGameType) :
            CardGame(ecGameType, 2)
        {
            // Stochastic game
            SetEnvironmentDeterministic(false);

            // Set flags
            SetDrawingAllowed(true);
            SetDrawingFromStockAllowed(true);
            SetDrawingFromDiscardPileAllowed(true);
            SetMeldingAllowed(true);
            SetLayingOffAllowed(true);
            SetDiscardingAllowed(true);
            SetShowingAllowed(true);
            SetScoringAllowed(true);

            // Set Allowed Moves
            // TODO: Duplicative to the above Set flags
            m_cAllowedMoves.AddMovesInSequence(1, GameVocabulary::DRAW, GameVocabulary::ARG_STOCK);
            m_cAllowedMoves.AddMovesInSequence(1, GameVocabulary::DRAW, GameVocabulary::ARG_DISCARD);
            m_cAllowedMoves.AddMovesInSequence(2, GameVocabulary::MELD);
            m_cAllowedMoves.AddMovesInSequence(2, GameVocabulary::LAYOFF);
            m_cAllowedMoves.AddMovesInSequence(2, GameVocabulary::DISCARD);
            m_cAllowedMoves.AddMove(GameVocabulary::SHOW);
            m_cAllowedMoves.AddMove(GameVocabulary::SCORE);
            //m_cAllowedMoves.AddMove(GameVocabulary::RESIGN);

            // Shuffle and Deal cards
            BeginHand();

            // Configure logger
            m_cLogger.UseTag(false);
            m_cLogger.UseLevelIndent(false);
        }

        // Destructor
        ~CardGameBasicRummy() {}

        // Display the game
        virtual void Display() const override;

        // Synchronize the game environment across networked players
        virtual bool GetSyncInfo(std::string &sGameInformation) override;
        virtual bool ApplySyncInfo(const std::string &sGameInformation, std::string &sErrorMessage) override;

        // Return a list of valid moves in string format
        virtual std::string ValidMoves(int nPlayer) override;

        // Generate a vector of valid moves
        virtual std::vector<GameMove> GenerateMoves(int nPlayer) override;

        // Apply the move to the game
        virtual bool ApplyMove(int nPlayer, GameMove &cGameMove) override;

        // Announce the move made
        virtual std::string AnnounceMove(int nPlayer, const GameMove &cGameMove) const override;

        // Return the score of the game
        virtual std::string GameScore() const override;

        // Return the stats of the game
        virtual std::string GameStatistics() const override;

        // Check to see if the game has ended
        virtual bool GameEnded(int nPlayer) override;

        // Update Blackboard
        virtual void BlackboardUpdate(int nPlayer, Blackboard &cBlackboard) override;

        // Generate a move from the Blackboard
        virtual GameMove BlackboardMove(int nPlayer, Blackboard &cBlackboard, int nProbability) override;

        // Clone the current game
        virtual std::unique_ptr<Game> Clone() const override { return std::unique_ptr<Game>(new CardGameBasicRummy(*this)); }

        // Return the title of the game
        virtual std::string Title() const override { return "Basic Rummy"; }

        // Return the description of the game
        virtual std::string Description() const override { return "Valid moves are " +
            GameVocabulary::DRAW + " " + GameVocabulary::ARG_STOCK + ", " +
            GameVocabulary::DRAW + " " + GameVocabulary::ARG_DISCARD + ", " +
            GameVocabulary::MELD + ", " +
            GameVocabulary::DISCARD + ", " +
            GameVocabulary::SHOW + ", " +
            GameVocabulary::SCORE + ", " +
            GameVocabulary::RESIGN + "."; }

        // Override Game::SetSync()
        void SetSync(bool b)  { CardGame::SetSync(b); m_bSyncMatches = b; }

    protected:
        // Initialize Blackboard
        virtual void BlackboardInitialize(int nPlayer, Blackboard &cBlackboard) const override;

        // Begin a hand of Rummy
        void BeginHand();

        // Based on the state of the game, eliminate moves
        void EvaluatePossibleMoves(int nPlayer, std::vector<GameMove> &vGameMoves);

        // Rummy moves
        bool DrawCard(int nPlayer, GameMove &cGameMove);
        bool MeldCards(int nPlayer, GameMove &cGameMove);
        bool LayoffCard(int nPlayer, GameMove &cGameMove);
        bool Discard(int nPlayer, GameMove &cGameMove);

        // Calculate the score of the game
        int  ScoreHand(int nPlayer, int nRummyMultiplier=1);

        // Manage a player's ability to Go Rummy
        void InitializeRummy()       { m_abRummy[0] = m_abRummy[1] = true; }
        void SetRummyOff(int nIndex) { m_abRummy[nIndex - 1] = false; }
        bool Rummy(int nIndex)       { return m_abRummy[nIndex - 1]; }

    private:
        // Matches are used to hold matches for each player
        std::unordered_multimap<int, Match> m_uommMatches;

        // Discard Pile
        PlayingCards m_cDiscardPile;

        // Rummy flags
        bool m_abRummy[2] {true};

        // Number of cards to make a match
        const int m_knMatchNumber {3};

        // Return matches as a string
        std::string MatchesTypes(const MatchType ecMatchType=MatchType::TYPE_NONE) const;

        //// Serialize and deserialize Matches
        Json::Value MatchesJsonSerialization() const;
        bool        MatchesJsonDeserialization(const std::string &sJsonPlayingCards, std::string &sErrorMessage);

        // Sync flags
        bool m_bSyncMatches { false };

        // Stats Per player:
        //   The number of melds
        //   The number of layoffs
        //   The number of draws from the discard
        //   The number of draws from the stock
        //   The number of hands won
        int m_aiNumberOfMelds[2]             {0};
        int m_aiNumberOfLayoffs[2]           {0};
        int m_aiNumberOfDrawsFromDiscard[2]  {0};
        int m_aiNumberOfDrawsFromStock[2]    {0};
        int m_aiNumberOfHandsWon[2]          {0};
};

#endif // CARDGAMEBASICRUMMY_H
