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
{
}

/**
  * Return information to be synchronized between networked players.
  *
  * This function is a NOP and should be overridden in derived classes.
  *
  * \param sGameInformation String representing game information to be synchronized
  * between players.
  *
  * \return True if information received to synchronize, false otherwise.
  */

bool CardGameGoFish::GetSyncInfo(std::string &sGameInformation)
{
    std::string sLogInfo {};
    sGameInformation.clear();

    if (m_bSyncBooks)
    {
        m_cLogger.LogInfo("Gathering synchronization on books", 2);
        sLogInfo = "Books Ranks: " + BooksRanks();
        m_cLogger.LogInfo(sLogInfo, 3);
        sGameInformation = BooksJsonSerialization().toStyledString();
        m_bSyncBooks = false;
        return true;
    }

    return CardGame::GetSyncInfo(sGameInformation);
}

/**
  * Receive information to be synchronized from a networked opponent
  *
  * This function is a NOP and should be overridden in derived classes.
  *
  * \return True if information is available to be sent, false otherwise.
  */

bool CardGameGoFish::ApplySyncInfo(const std::string &sGameInformation, std::string &sErrorMessage)
{
    std::string sLogInfo {};

    if (m_bSyncBooks)
    {
        m_cLogger.LogInfo("Applying synchronization on books", 2);
        if (BooksJsonDeserialization(sGameInformation, sErrorMessage))
        {
            m_bSyncBooks = false;
            sLogInfo = "Books Ranks: " + BooksRanks();
            m_cLogger.LogInfo(sLogInfo, 3);
            return true;
        }
        else
        {
            return false;
        }
    }

    return CardGame::ApplySyncInfo(sGameInformation, sErrorMessage);
}

/**
  * Return a string of valid moves.
  *
  * \param nPlayer The player whose turn it is.
  *
  * \return A string of valid moves.
  */

std::string CardGameGoFish::ValidMoves(int nPlayer) const
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

std::vector<GameMove> CardGameGoFish::GenerateMoves(int nPlayer) const
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
                cGameMove.UpdateCard(cCard);
                cGameMove.SetArgument(sRank);
                vGameMoves.push_back(cGameMove);

                sRank.clear();
            }
        }
    }
/*
    // Else if the deck has cards, create a DRAW game move
    else if (m_cDeck.HasCards())
    {
        cGameMove.SetDraw(true);
        vGameMoves.push_back(cGameMove);
    }
*/

    return vGameMoves;
}

bool CardGameGoFish::GoFish(int nPlayer)
{
    if (m_cDeck.HasCards())
    {
        std::string sMessage = "Player " + std::to_string(nPlayer) + " draws from the stock";
        m_cLogger.LogInfo(sMessage,1);

        Card cCard = m_cDeck.DrawTopCard();
        m_vHands[nPlayer - 1].AddCard(cCard);

        return true;
    }

    m_cLogger.LogInfo("No cards left in the stock to draw from" ,1);

    return false;
}

/**
  * Apply a move to the game.
  *
  * Draw a card from the player's deck and insert it into the 'battle.'
  * Evaluate for highest value card.
  * If tied, set up War: draw a face-down card and a face-up card.
  * Face-up cards are evaluated for highest value.
  * The highest value wins all cards in play.
  *
  * \param nPlayer   The player whose turn it is.
  * \param cGameMove The player's move
  *
  * \return True, if valid.  False otherwise.
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

        std::cout << "Books made: ";

        return true;
    }

    // Check for score
    if (cGameMove.Score())
    {
        //cGameMove.SetScore(false);
        cGameMove.SetAnotherTurn(true);
        cGameMove.SetPlayerNumber(nPlayer);
        // Capture move for network play
        m_vGameMoves.push_back(cGameMove);
        return true;
    }

/*
    // Check for draw, used when no cards left in player's hand
    if (cGameMove.Draw())
    {
        if (GoFish(nPlayer))
        {
            cGameMove.SetAsk(true);
            cGameMove.SetDraw(false);
        }
    }
*/

    //// If not Resignation or Draw, then must be Ask
    // Must be Ask
    if (!cGameMove.Ask())
    {
        return false;
    }

    // Ensure that asked-for rank is in the players hand.  You can't ask for it
    // unless it's in your hand.
    if (!m_vHands[nPlayer - 1].HasRank(cGameMove.GetCard().Rank()))
    {
        return false;
    }

    //
    // Apply move to the game
    //

    // Find rank in opposing player's hand
    if (m_vHands[3 - nPlayer - 1].HasRank(cGameMove.GetCard().Rank()))
    {
        // Pass cards from asked player to asking player
        std::vector<Card> vCards = m_vHands[2 - nPlayer].RemoveCardsOfRank(cGameMove.GetCard().Rank());

        sMessage = "Player " + std::to_string(3 - nPlayer) + " hands over " + m_asNumbers[vCards.size()] + " " +  cGameMove.AnnounceCardRank();
        m_cLogger.LogInfo(sMessage,1);

        m_vHands[nPlayer - 1].AddCards(vCards);

        cGameMove.SetAnotherTurn(true);
        cGameMove.SetPlayerNumber(nPlayer);
        cGameMove.SetSuccess(true);
        cGameMove.SetCards(vCards.size());
    }
    else // Go Fish
    {
        sMessage = "Player " + std::to_string(3 - nPlayer) + " says Go Fish";
        m_cLogger.LogInfo(sMessage,1);

        if (GoFish(nPlayer))
        {
            Card cCard = m_vHands[nPlayer - 1].PeekAtBottomCard();

            if (cGameMove.GetCard().Rank() == cCard.Rank())
            {
                m_cLogger.LogInfo("Card drawn has the same rank as originally asked for" ,1);

                cGameMove.SetAnotherTurn(true);
                cGameMove.SetPlayerNumber(nPlayer);
                cGameMove.SetCards(1);
            }
        }
    }

    // Check for books
    do
    {
        Hand cHand = m_vHands[nPlayer - 1].RemoveBookByRank(m_knBookNumber);
        if (cHand.HasCards())
        {
            m_uommBooks.insert(std::make_pair(m_vHands[nPlayer - 1].ID(), cHand));
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
  * Count the cards for each player and the war chest.
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

// Initialize Blackboard
void CardGameGoFish::BlackboardInitialize(int nPlayer, Blackboard &cBlackboard) const
{
    // Remove cards matching my hand from ProbableDeck
    std::string       sRank {};
    std::vector<Card> vCards {};
    for (const char &chToken : m_vHands[nPlayer - 1].Ranks())
    {
        if (chToken != ' ')
        {
            sRank.append(1, chToken);
        }
        else
        {
            // Remove one card of sRank from ProbableDeck
            vCards = cBlackboard.m_cProbableDeck.RemoveCardsOfRank(sRank, 1);
            if (vCards.size() != 1)
            {
                std::string sError = "Expected to remove one card from probable deck, but removed " + std::to_string(vCards.size());
                throw GameAIException(sError);
            }
            sRank.clear();
        }
    }

    // "Copy" cards from ProbableDeck to ProbableOpponentHand
    std::string sErrorMessage {};
    Json::Value jValue = cBlackboard.m_cProbableDeck.JsonSerialization();
    if (!cBlackboard.m_cProbableOpponentHand.JsonDeserialization(jValue.toStyledString(), sErrorMessage))
    {
        std::string sError = "Error during JsonDeserialization: " + sErrorMessage;
        throw GameAIException(sError);
    }

    //
    // Set the number of cards the ProbableDeck and the ProbableOpponentHand should have
    //

    // ProbableDeck: Subtract size of player's hand multiplied by two from ProbableDeck
    cBlackboard.m_cProbableDeck.SetNumberOfCards(cBlackboard.m_cProbableDeck.HasCards() - (m_vHands[1 - nPlayer].HasCards() * 2));
    // ProbableOpponentHand: Set to number of cards in player's hand
    cBlackboard.m_cProbableOpponentHand.SetNumberOfCards(m_vHands[1 - nPlayer].HasCards());

    // Set initialized flag
    cBlackboard.SetInitialized();

    return;
}

// Generate a move from the Blackboard
GameMove CardGameGoFish::BlackboardMove(int nPlayer, Blackboard &cBlackboard) const
{
    // Generic GamevMove
    GameMove cGameMove;

    // Turn off 'Move' for card games
    cGameMove.SetMove(false);

    // Set as ASK
    cGameMove.SetAsk(true);

    //
    // Ask for cards that opponent probably has and I need
    //
    //TODO: cProbableOpponentHand.SortByProbability();
    for (auto &cProbableCard : cBlackboard.m_cProbableOpponentHand.Cards())
    {
        // Ask if probability of successfully pulling the card is 50%
        float fProbabilityOfPullingCard =
            cBlackboard.m_cProbableOpponentHand.HasRank(cProbableCard.Rank()) /
            cBlackboard.m_cProbableOpponentHand.HasCards() * cProbableCard.Probability();

        if (fProbabilityOfPullingCard >= .5)
        {
            for (Card &cCard : m_vHands[1 - nPlayer].Cards())
            {
                if (cProbableCard.Rank() == cCard.Rank())
                {
                    cGameMove.UpdateCard(cProbableCard);
                    cBlackboard.UpdateAsks(cCard.Rank());
                    return cGameMove;
                }
            }
        }
    }

    //
    // Ask for cards that the deck has, and I have a good chance of pulling
    //
    //TODO: cProbableDeck.SortByProbability();
    for (auto &cProbableCard : cBlackboard.m_cProbableDeck.Cards())
    {
        // Ask if probability of successfully pulling the card is 15%
        float fProbabilityOfPullingCard =
            cBlackboard.m_cProbableDeck.HasRank(cProbableCard.Rank()) /
            cBlackboard.m_cProbableDeck.HasCards() * cProbableCard.Probability();

        if (fProbabilityOfPullingCard >= .15)
        {
            for (Card &cCard : m_vHands[1 - nPlayer].Cards())
            {
                if (cProbableCard.Rank() == cCard.Rank())
                {
                    cGameMove.UpdateCard(cProbableCard);
                    cBlackboard.UpdateAsks(cCard.Rank());
                    return cGameMove;
                }
            }
        }
    }

    //
    // Ask for cards that I have many of and I have not asked for recently
    //
    Card cLastCard;
    Card cBestCard;
    int nLeastNumberOfAsks {INT_MAX};

    // Loop through my hands in order of number of Ranks in hand (sorted by ApplyMove)
    for (Card &cCard : m_vHands[1 - nPlayer].Cards())
    {
        // If last card rank is not the same as this card rank, evaluate
        if (cLastCard.Rank() != cCard.Rank())
        {
            // Update last card
            cLastCard = cCard;

            // Evaluate best card on basis of least asks
            if (cBlackboard.Asks(cCard.Rank()) < nLeastNumberOfAsks)
            {
                nLeastNumberOfAsks = cBlackboard.Asks(cCard.Rank());
                cBestCard = cCard;
            }

            // If least asks at zero, break
            if (nLeastNumberOfAsks == 0)
            {
                break;
            }
        }
    }

    cGameMove.UpdateCard(cBestCard);
    cBlackboard.UpdateAsks(cBestCard.Rank());
    return cGameMove;
}

// Update Blackboard
void CardGameGoFish::BlackboardUpdate(int nPlayer, Blackboard &cBlackboard) const
{
    // If not initialized, initialize
    if (!cBlackboard.Initialized())
        BlackboardInitialize(nPlayer, cBlackboard);

    //
    // Check last move
    //
    GameMove cLastMove = LastMove();

    // If not an ASK, return
    if (!cLastMove.Ask())
    {
        return;
    }

    std::string sRank = cLastMove.Argument();
    bool        bSuccess = cLastMove.Success();
    int         nCards = cLastMove.Cards();

    // Player turn
    if (cLastMove.PlayerNumber() == nPlayer)
    {
        // If Successful ASK
        if (bSuccess)
        {
            //
            // Update ProbableOpponentHand
            //
            // Remove cards of rank from ProbableOpponentHand
            cBlackboard.m_cProbableOpponentHand.RemoveCardsOfRank(sRank);
            // Reduce number of cards in ProbableOpponentHand
            cBlackboard.m_cProbableOpponentHand.ReduceNumberOfCards(nCards);

            //
            // Update ProbableDeck
            //
            int nNumberOfCardsOfRankInMyHand = m_vHands[nPlayer - 1].HasCardsOfRank(sRank);

            // Remove any cards of rank from ProbableDeck
            cBlackboard.m_cProbableDeck.RemoveCardsOfRank(sRank);

            // Add cards of rank back to ProbableDeck with certainty
            Card cCard;
            cCard.SetRank(sRank);
            cCard.SetProbability(1.0);
            for (int iii = 0; iii < m_knBookNumber - nNumberOfCardsOfRankInMyHand; ++iii)
            {
                cBlackboard.m_cProbableDeck.AddCard(cCard);
            }
        }
        // Else if ASK not Successful, but Go-Fish was
        else if (nCards > 0) //
        {
            //
            // Update ProbableOpponentHand
            //
            // Remove cards of rank from ProbableOpponentHand
            cBlackboard.m_cProbableOpponentHand.RemoveCardsOfRank(sRank);

            //
            // Update ProbableDeck
            //
            int nNumberOfCardsOfRankInMyHand = m_vHands[nPlayer - 1].HasCardsOfRank(sRank);

            // Remove any cards of rank from ProbableDeck
            cBlackboard.m_cProbableDeck.RemoveCardsOfRank(sRank);

            // Add cards of rank back to ProbableDeck with certainty
            Card cCard;
            cCard.SetRank(sRank);
            cCard.SetProbability(1.0);
            for (int iii = 0; iii < m_knBookNumber - nNumberOfCardsOfRankInMyHand; ++iii)
            {
                cBlackboard.m_cProbableDeck.AddCard(cCard);
            }

            // Reduce number of cards in ProbableDeck
            cBlackboard.m_cProbableDeck.ReduceNumberOfCards(nCards);
        }
        // Else Ask and Go-Fish not Successful
        else
        {
            //
            // Update ProbableOpponentHand
            //
            // Remove cards of rank from ProbableOpponentHand
            cBlackboard.m_cProbableOpponentHand.RemoveCardsOfRank(sRank);

            //
            // Update ProbableDeck
            //
            // Reduce number of cards in ProbableDeck
            cBlackboard.m_cProbableDeck.ReduceNumberOfCards(nCards);
        }
    }
    // Opponent turn
    else
    {
        if (bSuccess)
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
                cBlackboard.m_cProbableOpponentHand.AddCard(cCard);
            }
            // Add cards of rank back to ProbableOpponentHand with less than certainty
            cCard.SetProbability(0.0);
            for (int iii = nCards + 1; iii < m_knBookNumber; ++iii)
            {
                cBlackboard.m_cProbableOpponentHand.AddCard(cCard);
            }

            // Add number of cards in ProbableOpponentHand
            cBlackboard.m_cProbableOpponentHand.AddNumberOfCards(nCards);

            //
            // Update ProbableDeck
            //
            // Remove any cards of rank from ProbableDeck
            cBlackboard.m_cProbableDeck.RemoveCardsOfRank(sRank);

            // Add cards of rank back to ProbableDeck
            //Card cCard;
            cCard.SetRank(sRank);
            cCard.SetProbability(0.0);
            for (int iii = nCards + 1; iii < m_knBookNumber; ++iii)
            {
                cBlackboard.m_cProbableDeck.AddCard(cCard);
            }
        }
        // Else if ASK not Successful, but Go-Fish was
        else if (nCards > 0) //
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
                cBlackboard.m_cProbableOpponentHand.AddCard(cCard);
            }

            // Add cards of rank back to ProbableOpponentHand with less than certainty
            cCard.SetProbability(0.0);
            for (int iii = nCards + 1; iii < m_knBookNumber; ++iii)
            {
                cBlackboard.m_cProbableOpponentHand.AddCard(cCard);
            }

            // Add number of cards in ProbableOpponentHand
            cBlackboard.m_cProbableOpponentHand.AddNumberOfCards(nCards);

            //
            // Update ProbableDeck
            //
            // Remove any cards of rank from ProbableDeck
            cBlackboard.m_cProbableDeck.RemoveCardsOfRank(sRank);

            // Add cards of rank back to ProbableDeck
            //Card cCard;
            cCard.SetRank(sRank);
            cCard.SetProbability(0.0);
            for (int iii = nCards + 1; iii < m_knBookNumber; ++iii)
            {
                cBlackboard.m_cProbableDeck.AddCard(cCard);
            }

            // Reduce number of cards in ProbableDeck
            cBlackboard.m_cProbableDeck.ReduceNumberOfCards(nCards);
        }
        // Else Ask and Go-Fish not Successful
        else
        {
            //
            // Update ProbableOpponentHand
            //
            // Remove cards of rank from ProbableOpponentHand
            cBlackboard.m_cProbableOpponentHand.RemoveCardsOfRank(sRank);

            // Add number of cards in ProbableOpponentHand
            cBlackboard.m_cProbableOpponentHand.AddNumberOfCards(nCards);

            //
            // Update ProbableDeck
            //
            // Reduce number of cards in ProbableDeck
            cBlackboard.m_cProbableDeck.ReduceNumberOfCards(nCards);
        }
    }

    //
    // TODO: Update based on books
    //

    // Update rank probabilities
    cBlackboard.m_cProbableDeck.UpdateRankProbabilities(cBlackboard.m_cProbableOpponentHand);

    return;
}

std::string CardGameGoFish::BooksRanks() const
{
    std::string sRanks {};
    Hand cHand;

    for (const auto &PlayerHand : m_uommBooks)
    {
        cHand = PlayerHand.second;
        sRanks += cHand.Ranks();
    }

    if (sRanks.empty())
        sRanks = "empty";

    return sRanks;
}

Json::Value CardGameGoFish::BooksJsonSerialization() const
{
    Json::Value jValue;
    Json::Value jBooks;
    Hand cHand;

    // The following are used to create a index that will retain uniqueness when
    // multimap items serialized to JSON.  May be used for up to 260 unique
    // values: a0 to z9.
    int nCounter {0};
    char chLetter = 'a';
    std::string sIndex {};

    for (const auto &PlayerHand : m_uommBooks)
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

        jValue["Player"] = PlayerHand.first;
        cHand            = PlayerHand.second;
        jValue["Hand"]   = cHand.JsonSerialization();
        jBooks[sIndex]   = jValue;
    }

    return jBooks;
}

bool CardGameGoFish::BooksJsonDeserialization(const std::string &sJsonBooks, std::string &sErrorMessage)
{
    Json::Reader jReader;
    Json::Value  jBooks;
    int nPlayer;
    Hand cHand;

    if (jReader.parse(sJsonBooks, jBooks, false))
    {
        m_uommBooks.clear();

        for (const Json::Value &jValue : jBooks)
        {
            nPlayer = jValue["Player"].asInt();

            if (cHand.JsonDeserialization(jValue["Hand"].toStyledString(), sErrorMessage))
            {
                // Insert players's book into Books
                m_uommBooks.insert(std::make_pair(nPlayer, cHand));
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


