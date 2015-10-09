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

#include "CardGameGoFish.h"

/**
  * Display the cards.
  *
  * For Go Fish, this is NOP.
  *
  */

void CardGameGoFish::Display() const
{}

/**
  * Return information to be synchronized between networked players.
  *
  * \param sGameInformation String representing game information to be synchronized
  * between players.
  *
  * \return True if information received to synchronize, false otherwise.
  */

bool CardGameGoFish::GetSyncInfo(std::string &sGameInformation)
{
    sGameInformation.clear();

    //Sync books
    if (m_bSyncBooks)
    {
        m_cLogger.LogInfo("Gathering synchronization on books", 2);
        std::string sLogInfo = "Books Ranks: " + BooksRanks();
        m_cLogger.LogInfo(sLogInfo, 3);

        // Serialize books
        sGameInformation = BooksJsonSerialization().toStyledString();
        m_bSyncBooks = false;
        return true;
    }

    // Pass to base class and return
    return CardGame::GetSyncInfo(sGameInformation);
}

/**
  * Receive information to be synchronized from a networked opponent
  *
  * \param sGameInformation String representing game information to be
  * synchronized between players
  *
  * \param sErrorMessage    String representing the error message upon
  * unsuccessful deserialization
  *
  * \return True if information is available to be sent, false otherwise.
  */

bool CardGameGoFish::ApplySyncInfo(const std::string &sGameInformation, std::string &sErrorMessage)
{
    // Sync books
    if (m_bSyncBooks)
    {
        m_cLogger.LogInfo("Applying synchronization on books", 2);

        // Deserialize books from JSON
        if (BooksJsonDeserialization(sGameInformation, sErrorMessage))
        {
            m_bSyncBooks = false;
            std::string sLogInfo = "Books Ranks: " + BooksRanks();
            m_cLogger.LogInfo(sLogInfo, 3);
            return true;
        }
        else
        {
            return false;
        }
    }

    // Pass to base class and return
    return CardGame::ApplySyncInfo(sGameInformation, sErrorMessage);
}

/**
  * Return a string of valid moves.
  *
  * \param nPlayer The player whose turn it is.
  *
  * \return A string of valid moves.
  */

std::string CardGameGoFish::ValidMoves(int nPlayer)
{
    std::string sValidMoves = GameVocabulary::ASK + " " + m_vHands[nPlayer - 1].Ranks();

    return sValidMoves;
}

/**
  * Return a vector of valid game moves.
  *
  * For Go Fish, the only move is to ask for cards, if the player has the
  * card rank in their hand.
  *
  * \param nPlayer The player whose turn it is.
  *
  * \return A vector of valid moves.
  */

std::vector<GameMove> CardGameGoFish::GenerateMoves(int nPlayer)
{
    std::vector<GameMove> vGameMoves {};
    GameMove cGameMove;
    Card cCard;

    // If player has cards in their hand, create ASK game moves for each rank
    if (m_vHands[nPlayer - 1].HasCards() > 0)
    {
        cGameMove.SetAsk(true);

        std::string sRank {};
        for (char &chToken : m_vHands[nPlayer - 1].Ranks())
        {
            if (chToken != ' ')
            {
                sRank.append(1, chToken);
            }
            else
            {
                cCard.SetRank(sRank);
                //cGameMove.UpdateCard(cCard);
                cGameMove.AddCard(cCard);
                cGameMove.SetArgument(sRank);
                vGameMoves.push_back(cGameMove);

                sRank.clear();
            }
        }
    }

    return vGameMoves;
}

/**
  * Go Fish.
  *
  * Pull a card from the stock of cards and add it to the player's hand.
  *
  * \param nPlayer The player whose turn it is.
  *
  * \return True if there are cards to pull, false otherwise.
  */

bool CardGameGoFish::GoFish(int nPlayer)
{
    if (m_cDeck.HasCards())
    {
        Card cCard = m_cDeck.DrawTopCard();
        m_vHands[nPlayer - 1].AddCard(cCard);

        std::string sMessage = "Player " + std::to_string(nPlayer) + " draws from the stock";
        m_cLogger.LogInfo(sMessage,1);

        return true;
    }

    m_cLogger.LogInfo("No cards left in the stock to draw from" ,1);

    return false;
}

/**
  * Apply a move to the game.
  *
  * Check for commands other than ASK
  * For an ASK command:
  *   Ensure rank being asked for is in player's hand
  *   Ask opponent for rank
  *     If Opponent has rank, she hands it over to player
  *     If not, player is to go-fish
  *   Evaluate for any books held by player
  *   Sort cards
  *   If player is out of cards, go fish
  *
  * \param nPlayer   The player whose turn it is.
  * \param cGameMove The player's move
  *
  * \return True, if valid turn.  False otherwise.
  */

bool CardGameGoFish::ApplyMove(int nPlayer, GameMove &cGameMove)
{
    // For logging
    std::string sMessage {};

    // Check player number
    if ((nPlayer != m_knPlayer1) && (nPlayer != m_knPlayer2))
        return false;

    // Check for resignation
    if (cGameMove.Resignation())
    {
        // Capture move for network play
        m_vGameMoves.push_back(cGameMove);
        return true;
    }

    // Check for show
    if (cGameMove.Show())
    {
        cGameMove.SetAnotherTurn(true);
        cGameMove.SetPlayerNumber(nPlayer);
        // Capture move for network play
        m_vGameMoves.push_back(cGameMove);

        // Display game stats
        for (Hand &cHand : m_vHands)
        {
            std::cout << "Player " << cHand.ID() << " has " << cHand.HasCards() << " cards." << std::endl;
        }
        std::cout << "The deck has " << m_cDeck.HasCards() << " cards." << std::endl;

        std::cout << "Books made: " << BooksUniqueRanks() << std::endl;

        return true;
    }

    // Check for score
    if (cGameMove.Score())
    {
        cGameMove.SetAnotherTurn(true);
        cGameMove.SetPlayerNumber(nPlayer);
        // Capture move for network play
        m_vGameMoves.push_back(cGameMove);
        return true;
    }

    // Must be Ask
    if (!cGameMove.Ask())
    {
        return false;
    }

    // Ensure that asked-for rank is in the players hand.  You can't ask for it
    // unless it's in your hand.
    std::vector<Card> vCards = cGameMove.GetCards();
    //if (!m_vHands[nPlayer - 1].HasRank(cGameMove.GetCard().Rank()))
    if (!m_vHands[nPlayer - 1].HasRank(vCards[0].Rank()))
    {
        return false;
    }

    //
    // Apply move to the game
    //

    // Find rank in opposing player's hand
    if (m_vHands[3 - nPlayer - 1].HasRank(vCards[0].Rank()))
    {
        // Pass cards from asked player to asking player
        std::vector<Card> vPassedCards = m_vHands[2 - nPlayer].RemoveCardsOfRank(vCards[0].Rank());

        cGameMove.SetAnotherTurn(true);                  // Player takes another turn
        cGameMove.SetPlayerNumber(nPlayer);
        cGameMove.SetNominalCards(vPassedCards.size());  // Number of cards taken

        sMessage = "Player " + std::to_string(3 - nPlayer) + " hands over " + m_asNumbers[vPassedCards.size()] + " " +  cGameMove.AnnounceCardRank();
        m_cLogger.LogInfo(sMessage,1);

        m_vHands[nPlayer - 1].AddCards(vPassedCards);

        // Update success stats
        ++m_aiSuccessfulAsks[nPlayer - 1];

    }
    else // Go Fish
    {
        sMessage = "Player " + std::to_string(3 - nPlayer) + " says Go Fish";
        m_cLogger.LogInfo(sMessage,1);

        cGameMove.SetAnotherTurn(false);    // Player does not take another turn
        cGameMove.SetPlayerNumber(nPlayer);
        cGameMove.SetNominalCards(0);       // Assume card taken from deck is not a match

        if (GoFish(nPlayer))
        {
            // Evaluate rank of Go Fish card against asked for rank
            Card cCard = m_vHands[nPlayer - 1].PeekAtBottomCard();
            std::vector<Card> vCards = cGameMove.GetCards();
            if (vCards[0].Rank() == cCard.Rank())
            {
                m_cLogger.LogInfo("Card drawn has the same rank as originally asked for" ,1);
                cGameMove.SetAnotherTurn(true); // Player takes another turn
                cGameMove.SetNominalCards(1);   // Number of cards taken that match rank
            }
        }
    }

    // Check for books
    do
    {
        Book cBook = m_vHands[nPlayer - 1].RemoveBookByRank(m_knBookNumber);
        if (cBook.HasCards())
        {
            m_uommBooks.insert(std::make_pair(m_vHands[nPlayer - 1].ID(), cBook));
            continue;
        }
        else
        {
            break;
        }
    } while(true);

    // If players has cards, sort them
    if (m_vHands[nPlayer - 1].HasCards())
    {
        m_vHands[nPlayer - 1].SortByRank();
    }
    // If no cards in hand and player has another turn, draw from stock
    else if (cGameMove.AnotherTurn())
    {
        // Draw a card from the stock
        // If no more stock (and no cards in hand), the turn is over
        if (!GoFish(nPlayer))
        {
            cGameMove.SetAnotherTurn(false);
        }
    }

    // Increment move counter
    ++m_nNumberOfMoves;

    // Capture move for network play
    m_vGameMoves.push_back(cGameMove);

    return true;
}

/**
  * Announce game move.
  *
  * Construct and return a string containing the move.
  *
  * \param nPlayer   The player whose turn it is.
  * \param cGameMove The player's move
  *
  * \return A string containing the move.
  */

std::string CardGameGoFish::AnnounceMove(int nPlayer, const GameMove &cGameMove) const
{
    return "\rPlayer " + std::to_string(nPlayer) + " asks for " + cGameMove.AnnounceCardRank();
}

/**
  * Return a string providing a current score of the game.
  *
  * Count the number of books for each player.
  *
  * \return A string containing the game score.
  */

std::string CardGameGoFish::GameScore() const
{
    // Flag to prohibit showing score multiple times
    static bool bDisplayedScoreLastTime {false};

    // Do not display game score if logging turned off
    if (m_cLogger.Level() < 1)
        return "";

    // Do not display game score if last move is another turn and not explicitly a call for Score
    GameMove cGameMove = LastMove();
    if ((cGameMove.AnotherTurn()))
    {
        if (!cGameMove.Score())
        {
            return "";
        }
        else if (bDisplayedScoreLastTime)
        {
            bDisplayedScoreLastTime = false;
            return "";
        }
        else
        {
            bDisplayedScoreLastTime = true;
        }
    }

    // Display book counts for the game score
    std::string sScore {"\nBook Count:"};

    for (const Hand &cHand : m_vHands)
    {
        sScore += " Player " + std::to_string(cHand.ID()) + "=" + std::to_string(m_uommBooks.count(cHand.ID()));
    }

    sScore += "\n";

    return sScore;
}

/**
  * Return a string providing a current statistics of the game.
  *
  * Count the number of successful asks per player.
  *
  * \return A string containing the game stats.
  */

std::string CardGameGoFish::GameStatistics() const
{
    std::string sGameStats = "Successful Asks\n";
    sGameStats += "Player 1 = " + std::to_string(m_aiSuccessfulAsks[0]) + " (" + std::to_string(static_cast<float>(m_aiSuccessfulAsks[0]) / static_cast<float>(m_nNumberOfMoves) / 2.0 * 100.0) + "%)\n";
    sGameStats += "Player 2 = " + std::to_string(m_aiSuccessfulAsks[1]) + " (" + std::to_string(static_cast<float>(m_aiSuccessfulAsks[1]) / static_cast<float>(m_nNumberOfMoves) / 2.0 * 100.0) + "%)\n";

    return sGameStats;
}

/**
  * Check to see if a player has won the game.
  *
  * For a each player, evaluate whether they have cards left to play.
  *
  * \param nPlayer The player
  *
  * \return True, if any player has won the game.  False otherwise.
  */

bool CardGameGoFish::GameEnded(int nPlayer)
{
    // Clear win variables
    m_nWinner = -1;
    m_sWinBy.assign("nothing");

    if (CardGame::GameEnded(nPlayer))
        return true;

    // Evaluate whether the player has any valid moves to make
    std::vector<GameMove> vGameMoves = GenerateMoves(nPlayer);
    if (vGameMoves.empty())
    {
        int nNumberOfBooks {-1};
        for (const Hand &cHand : m_vHands)
        {
            if (static_cast<int>(m_uommBooks.count(cHand.ID())) > nNumberOfBooks)
            {
                nNumberOfBooks = m_uommBooks.count(cHand.ID());
                m_nWinner = cHand.ID();
            }
        }

        m_sWinBy = "having " + std::to_string(nNumberOfBooks) + " books";
        m_bGameOver = true;
        return true;
    }

    return false;
}

/**
  * Generate a move from the Blackboard
  *
  * Evaluate the probability of successfully pulling needed cards from opponent's
  * probable hand, the probable deck, or cards that have not been recently asked for.
  *
  * \param nPlayer      The player
  * \param cBlackboatrd The blackboard for this player
  *
  * \return A game move.
  */

GameMove CardGameGoFish::BlackboardMove(int nPlayer, Blackboard &cBlackboard, int nProbability)
{
    // Probability threshold
    float fProbabilityThreshold = static_cast<float>(nProbability) / 10;

    // Probability of pulling card
    float fProbabilityOfPullingCard {};

    // Logging messages
    std::string sLogMessage {};

    // Generic GameMove
    GameMove cGameMove;

    // Turn off 'Move' for card games
    cGameMove.SetMove(false);

    // Set as ASK
    cGameMove.SetAsk(true);

    //
    // Ask for cards that opponent probably has and I need
    //
    for (auto &cProbableCard : cBlackboard.m_cProbableOpponentHand.Cards())
    {
        // Calculate probability of successfully pulling the card
        if (cProbableCard.Probability() < 1.0)
        {
            fProbabilityOfPullingCard =
                static_cast<float>(cBlackboard.m_cProbableOpponentHand.HasCardsOfRank(cProbableCard.Rank())) /
                static_cast<float>(cBlackboard.m_cProbableOpponentHand.NumberOfCards()) * cProbableCard.Probability();
        }
        else
        {
            fProbabilityOfPullingCard = cProbableCard.Probability();
        }

        sLogMessage = "P(pull " + cProbableCard.Rank() + ") from Player" +
            std::to_string(3 - nPlayer) + " = " +
            std::to_string(fProbabilityOfPullingCard) + "[" +
            std::to_string(cBlackboard.m_cProbableOpponentHand.HasCardsOfRank(cProbableCard.Rank())) +
            " / " + std::to_string(cBlackboard.m_cProbableOpponentHand.NumberOfCards()) +
            " * " + std::to_string(fProbabilityOfPullingCard) + "]";
        m_cLogger.LogInfo(sLogMessage, 3);

        if (fProbabilityOfPullingCard >= fProbabilityThreshold)
        {
            for (Card &cCard : m_vHands[nPlayer - 1].Cards())
            {
                if (cProbableCard.Rank() == cCard.Rank())
                {
                    //cGameMove.UpdateCard(cProbableCard);
                    cGameMove.AddCard(cProbableCard);
                    cBlackboard.UpdateAsks(cCard.Rank());
                    m_cLogger.LogInfo("Asking for a card that opponent probably has and I need", 2);
                    return cGameMove;
                }
            }
        }
    }

    //
    // Ask for cards that the deck has, and I have a good chance of pulling
    //
    for (auto &cProbableCard : cBlackboard.m_cProbableDeck.Cards())
    {
        // Calculate probability of successfully pulling the card
        float fProbabilityOfPullingCard =
            static_cast<float>(cBlackboard.m_cProbableDeck.HasCardsOfRank(cProbableCard.Rank())) /
            static_cast<float>(cBlackboard.m_cProbableDeck.NumberOfCards())  * cProbableCard.Probability();

        sLogMessage = "P(pull " + cProbableCard.Rank() + ") from Deck = " +
            std::to_string(fProbabilityOfPullingCard) + "[" +
            std::to_string(cBlackboard.m_cProbableDeck.HasCardsOfRank(cProbableCard.Rank())) +
            " / " + std::to_string(cBlackboard.m_cProbableDeck.NumberOfCards()) +
            " * " + std::to_string(cProbableCard.Probability()) + "]";
        m_cLogger.LogInfo(sLogMessage, 3);

        if (fProbabilityOfPullingCard >= fProbabilityThreshold)
        {
            for (Card &cCard : m_vHands[nPlayer - 1].Cards())
            {
                if (cProbableCard.Rank() == cCard.Rank())
                {
                    //cGameMove.UpdateCard(cProbableCard);
                    cGameMove.AddCard(cProbableCard);
                    cBlackboard.UpdateAsks(cCard.Rank());
                    m_cLogger.LogInfo("Asking for a card that the deck probably has and I need", 2);
                    return cGameMove;
                }
            }
        }
    }

    //
    // Ask for cards that I have many of and I have not asked for recently
    //
    Card cBestCard;
    int nLowestAsks {INT_MAX};
    int nAsks {};

    // Loop through my hands in order of number of Ranks in hand (sorted by ApplyMove)
    for (const Card &cCard : m_vHands[nPlayer - 1].Cards())
    {
        nAsks = cBlackboard.Asks(cCard.Rank());
        if (nAsks < nLowestAsks)
        {
            nLowestAsks = nAsks;
            cBestCard = cCard;
        }
    }

    cGameMove.AddCard(cBestCard);
    cBlackboard.UpdateAsks(cBestCard.Rank());
    m_cLogger.LogInfo("Asking for a card that I have and haven't asked for recently", 2);
    return cGameMove;
}

/**
  * Update the blackboard
  *
  * Update the probability that cards exist in the Probable Deck and the
  * Probable Opponent's Hand based on the last move.
  *
  * \param nPlayer      The player
  * \param cBlackboard The blackboard for this player
  *
  */

void CardGameGoFish::BlackboardUpdate(int nPlayer, Blackboard &cBlackboard)
{
    std::string sLogMessage {};

    // If not initialized, initialize
    if (!cBlackboard.Initialized())
    {
        BlackboardInitialize(nPlayer, cBlackboard);
    }

    //
    // Check last move
    //
    GameMove cLastMove = LastMove();

    // If not an ASK, return
    if (!cLastMove.Ask())
    {
        //// Update rank probabilities
        cBlackboard.m_cProbableDeck.UpdateRankProbabilities(cBlackboard.m_cProbableOpponentHand.NumberOfCards());
        cBlackboard.m_cProbableOpponentHand.UpdateRankProbabilities(cBlackboard.m_cProbableDeck.NumberOfCards());
        return;
    }

    std::vector<Card> vCards   = cLastMove.GetCards();
    std::string       sRank    = vCards[0].Rank();
    int               nCards   = cLastMove.NominalCards();  // Number of cards taken that match rank asked for

    // Player turn
    if (cLastMove.PlayerNumber() == nPlayer)
    {
        //
        // Update ProbableOpponentHand
        //

        // Remove cards of rank from ProbableOpponentHand
        cBlackboard.m_cProbableOpponentHand.RemoveCardsOfRank(sRank);

        //
        // Update ProbableDeck
        //

        // Remove any cards of rank from ProbableDeck
        cBlackboard.m_cProbableDeck.RemoveCardsOfRank(sRank);

        // Add cards of rank back to ProbableDeck with certainty
        Card cCard;
        cCard.SetRank(sRank);
        cCard.SetProbability(1.0);
        int nNumberOfCardsOfRankInMyHand = m_vHands[nPlayer - 1].HasCardsOfRank(sRank);

        for (int iii = 0; iii < m_knBookNumber - nNumberOfCardsOfRankInMyHand; ++iii)
        {
            sLogMessage = "Update Prob Deck: P(" + sRank + ")=" + std::to_string(cCard.Probability());
            m_cLogger.LogInfo(sLogMessage,3);

            cBlackboard.m_cProbableDeck.AddCard(cCard);
        }
    }
    // Opponent turn
    else
    {
        //
        // Update ProbableOpponentHand
        //

        // Remove cards of rank from ProbableOpponentHand
        cBlackboard.m_cProbableOpponentHand.RemoveCardsOfRank(sRank);

        // Add cards of rank back to ProbableOpponentHand with certainty
        Card cCard;
        cCard.SetRank(sRank);
        cCard.SetProbability(1.0);
        for (int iii = 0; iii < nCards + 1; ++iii)
        {
            sLogMessage = "Update Prob Hand: P(" + sRank + ")=" + std::to_string(cCard.Probability());
            m_cLogger.LogInfo(sLogMessage,3);

            cBlackboard.m_cProbableOpponentHand.AddCard(cCard);
        }

        // Add cards of rank back to ProbableOpponentHand with less than certainty
        cCard.SetProbability(0.0);
        for (int iii = nCards + 1; iii < m_knBookNumber; ++iii)
        {
            sLogMessage = "Update Prob Hand: P(" + sRank + ")=" + std::to_string(cCard.Probability());
            m_cLogger.LogInfo(sLogMessage,3);

            cBlackboard.m_cProbableOpponentHand.AddCard(cCard);
        }

        //
        // Update ProbableDeck
        //

        // Remove any cards of rank from ProbableDeck
        cBlackboard.m_cProbableDeck.RemoveCardsOfRank(sRank);

        // Add cards of rank back to ProbableDeck
        cCard.SetRank(sRank);
        cCard.SetProbability(0.0);
        for (int iii = nCards + 1; iii < m_knBookNumber; ++iii)
        {
            sLogMessage = "Update Prob Deck: P(" + sRank + ")=" + std::to_string(cCard.Probability());
            m_cLogger.LogInfo(sLogMessage,3);

            cBlackboard.m_cProbableDeck.AddCard(cCard);
        }
    }

    //
    // Update based on books
    //
    if (BookMade(sRank))
    {
        sLogMessage = "Update Prob Deck: Removing all " + sRank;
        m_cLogger.LogInfo(sLogMessage,3);
        cBlackboard.m_cProbableDeck.RemoveCardsOfRank(sRank);

        sLogMessage = "Update Prob Hand: Removing all " + sRank;
        m_cLogger.LogInfo(sLogMessage,3);
        cBlackboard.m_cProbableOpponentHand.RemoveCardsOfRank(sRank);
    }

    // Update number of cards in Probable Deck and Probable Opponent's Hand
    cBlackboard.m_cProbableDeck.SetNumberOfCards(m_cDeck.HasCards());
    cBlackboard.m_cProbableOpponentHand.SetNumberOfCards(m_vHands[2 - nPlayer].HasCards());

    // Update rank probabilities
    cBlackboard.m_cProbableDeck.UpdateRankProbabilities(cBlackboard.m_cProbableOpponentHand.NumberOfCards());
    cBlackboard.m_cProbableOpponentHand.UpdateRankProbabilities(cBlackboard.m_cProbableDeck.NumberOfCards());

    return;
}

/**
  * Status of current books.
  *
  * Return a string representing the current books made by rank.
  *
  * \return A string containing the books made by rank.
  *
  */

std::string CardGameGoFish::BooksRanks() const
{
    std::string sRanks {};

    for (const auto &PlayerBook : m_uommBooks)
    {
        sRanks += PlayerBook.second.Ranks();
    }

    if (sRanks.empty())
        sRanks = "none";

    return sRanks;
}

/**
  * Status of current books.
  *
  * Return a string representing the current books made by unique rank.
  *
  * \return A string containing the books made by unique rank.
  *
  */

std::string CardGameGoFish::BooksUniqueRanks() const
{
    std::string sUniqueRanks {};
    std::string sRanks {};

    for (const auto &PlayerBook : m_uommBooks)
    {
        sRanks = PlayerBook.second.Ranks();
        sUniqueRanks = sUniqueRanks + sRanks[0] + " ";
    }

    if (sUniqueRanks.empty())
        sUniqueRanks = "none";

    return sUniqueRanks;
}

bool CardGameGoFish::BookMade(const std::string &sRank) const
{
    std::string sBookRanks {};

    for (const auto &PlayerBook : m_uommBooks)
    {
        sBookRanks = PlayerBook.second.Ranks();
        if (sRank[0] == sBookRanks[0])
            return true;
    }

    return false;
}

/**
  * Serialize the books into a Json object.
  *
  * \return The Json Value object representing the books.
  */

Json::Value CardGameGoFish::BooksJsonSerialization() const
{
    Json::Value jValue;
    Json::Value jBooks;
    Book cBook;

    // The following are used to create a index that will retain uniqueness when
    // multimap items serialized to JSON.  May be used for up to 260 unique
    // values: a0 to z9.
    int nCounter {0};
    char chLetter = 'a';
    std::string sIndex {};

    for (const auto &PlayerBook : m_uommBooks)
    {
        // Manage index
        if (nCounter >= 10)
        {
            nCounter = 0;
            ++chLetter;
        }
        sIndex.assign(1, chLetter);
        sIndex.append(std::to_string(nCounter));
        ++nCounter;

        // Capture player and serialized book (aka, hand) into jValue
        jValue["Player"] = PlayerBook.first;
        cBook            = PlayerBook.second;
        jValue["Book"]   = cBook.JsonSerialization();

        // Add the jValue to jBooks
        jBooks[sIndex]   = jValue;
    }

    return jBooks;
}

/**
  * Deserialize books from a Json object.
  *
  * \param sJsonBooks    A JSON string representing a books.
  * \param sErrorMessage A string to return an error message if needed
  *
  * \return True if deserialization is successful, false otherwise
  */

bool CardGameGoFish::BooksJsonDeserialization(const std::string &sJsonBooks, std::string &sErrorMessage)
{
    Json::Reader jReader;
    Json::Value  jBooks;
    int nPlayer;
    Book cBook;

    // Parse the JSON string into a Json object
    if (jReader.parse(sJsonBooks, jBooks, false))
    {
        m_uommBooks.clear();

        for (const Json::Value &jValue : jBooks)
        {
            nPlayer = jValue["Player"].asInt();

            if (cBook.JsonDeserialization(jValue["Book"].toStyledString(), sErrorMessage))
            {
                // Insert players's book into Books
                m_uommBooks.insert(std::make_pair(nPlayer, cBook));
            }
            else
            {
                return false;
            }
        }

        return true;
    }
    else
    {
        sErrorMessage = jReader.getFormattedErrorMessages();
        return false;
    }
}


