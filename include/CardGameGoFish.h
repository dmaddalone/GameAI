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
            // Stochastic game
            SetEnvironmentDeterministic(false);

            // Shuffle and Deal cards
            m_cDeck.Shuffle();
            m_cDeck.Deal(7, m_vHands);

            // Sort hands
            for (Hand &cHand : m_vHands)
            {
                cHand.SortByRank();
            }

            // Set flags
            SetAskingAllowed(true);
            SetShowingAllowed(true);
            SetScoringAllowed(true);

            // Configure logger
            m_cLogger.UseTag(false);
            m_cLogger.UseLevelIndent(false);
        }

        // Destructor
        ~CardGameGoFish() {}

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
        virtual std::unique_ptr<Game> Clone() const override { return std::unique_ptr<Game>(new CardGameGoFish(*this)); }

        // Return the title of the game
        virtual std::string Title() const override { return "Go Fish"; }

        // Return the description of the game
        virtual std::string Description() const override { return "Valid moves are " +
            GameVocabulary::ASK + ", " +
            GameVocabulary::SHOW + ", " +
            GameVocabulary::SCORE + ", " +
            GameVocabulary::RESIGN + "."; }

        // Override Game::SetSync()
        void SetSync(bool b)  { CardGame::SetSync(b); m_bSyncBooks = b; }

        // Draw a card from the stock
        bool GoFish(int nPlayer);

    private:
        // Books used to hold books for each player
        std::unordered_multimap<int, Book> m_uommBooks;

        // Number of cards to make a book
        const int m_knBookNumber {4};

        // Return all ranks in the Books
        std::string BooksRanks() const;
        std::string BooksUniqueRanks() const;
        bool        BookMade(const std::string&sRank) const;

        // Serialize and deserialize Books
        Json::Value BooksJsonSerialization() const;
        bool        BooksJsonDeserialization(const std::string &sJsonPlayingCards, std::string &sErrorMessage);

        // Sync flags
        bool m_bSyncBooks { false };

        // Stats: successful asks for cards
        int m_aiSuccessfulAsks[2] {0};
};

#endif // CARDGAMEGOFISH_H
