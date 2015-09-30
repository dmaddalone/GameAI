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

#include "CardGameBasicRummy.h"

/**
  * Display the cards.
  *
  * For Go Fish, this is NOP.
  *
  */

void CardGameBasicRummy::Display() const
{}

/**
  * Return information to be synchronized between networked players.
  *
  * \param sGameInformation String representing game information to be synchronized
  * between players.
  *
  * \return True if information received to synchronize, false otherwise.
  */

bool CardGameBasicRummy::GetSyncInfo(std::string &sGameInformation)
{
    sGameInformation.clear();

    //Sync matches
    if (m_bSyncMatches)
    {
        m_cLogger.LogInfo("Gathering synchronization on matches", 2);
        std::string sLogInfo = "Matches: " + MatchesTypes();
        m_cLogger.LogInfo(sLogInfo, 3);

        // Serialize matches
        sGameInformation = MatchesJsonSerialization().toStyledString();
        m_bSyncMatches = false;
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

bool CardGameBasicRummy::ApplySyncInfo(const std::string &sGameInformation, std::string &sErrorMessage)
{
    // Sync matches
    if (m_bSyncMatches)
    {
        m_cLogger.LogInfo("Applying synchronization on matches", 2);

        // Deserialize matches from JSON
        if (MatchesJsonDeserialization(sGameInformation, sErrorMessage))
        {
            m_bSyncMatches = false;
            std::string sLogInfo = "Matches: " + MatchesTypes();
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

void CardGameBasicRummy::EvaluatePossibleMoves(int nPlayer, std::vector<GameMove> &vGameMoves)
{
    bool bIteratorIncremented {false};

    std::vector<GameMove>::iterator it = vGameMoves.begin();
    while (it != vGameMoves.end())
    {
        if (it->IsCommand(GameVocabulary::DRAW))
        {
            it->SetDraw(true);

            if (it->IsArgument(GameVocabulary::ARG_STOCK))
            {
                // If stock is out of cards, remove this as a valid move
                if (m_cDeck.HasCards() == 0)
                {
                    it = vGameMoves.erase(it);
                    bIteratorIncremented = true;
                }
            }
            else if (it->IsArgument(GameVocabulary::ARG_DISCARD))
            {
                // If discard pile is out of cards, remove this as a valid move
                if (m_cDiscardPile.HasCards() == 0)
                {
                    it = vGameMoves.erase(it);
                    bIteratorIncremented = true;
                }
            }
        }

        if (it->IsCommand(GameVocabulary::MELD))
        {
            it->SetMeld(true);

            // If no opportunities for meld in hand, remove this as a valid move
            if (!m_vHands[nPlayer - 1].MatchOpportunities(m_knMatchNumber))
            {
                it = vGameMoves.erase(it);
                bIteratorIncremented = true;
            }
        }

        if (it->IsCommand(GameVocabulary::LAYOFF))
        {
            it->SetLayoff(true);

            // If no opportunities for meld in hand, remove this as a valid move
            if (!m_vHands[nPlayer - 1].LayoffOpportunities(m_uommMatches))
            {
                it = vGameMoves.erase(it);
                bIteratorIncremented = true;
            }
        }

        if (it->IsCommand(GameVocabulary::DISCARD))
        {
            it->SetDiscard(true);

            // If player has no cards in hand, remove this as a valid move
            if (m_vHands[nPlayer - 1].HasCards() == 0)
            {
                it = vGameMoves.erase(it);
                bIteratorIncremented = true;
            }
        }

        if (bIteratorIncremented)
        {
            bIteratorIncremented = false;
        }
        else
        {
            ++it;
        }
    }
}

/**
  * Return a string of valid moves.
  *
  * \param nPlayer The player whose turn it is.
  *
  * \return A string of valid moves.
  */

std::string CardGameBasicRummy::ValidMoves(int nPlayer)
{
    std::string sMoves {};
    std::vector<GameMove> vGameMoves {};

    m_cAllowedMoves.NextMoveInSequence(sMoves, false);
    EvaluatePossibleMoves(nPlayer, vGameMoves);

    for (const GameMove &cGameMove : vGameMoves)
    {
        sMoves += cGameMove.Command() + GameVocabulary::DELIMETER + cGameMove.Argument() + "\n";
    }

    return sMoves;
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

std::vector<GameMove> CardGameBasicRummy::GenerateMoves(int nPlayer)
{
    std::vector<GameMove> vGameMoves {};

    m_cAllowedMoves.NextMoveInSequence(vGameMoves);
    EvaluatePossibleMoves(nPlayer, vGameMoves);

    return vGameMoves;
}

/**
  * Draw a card from either the stock or the discard pile.
  *
  * \param nPlayer    The player whose turn it is.
  * \param cGamdeMove The player's game move object
  *
  * \return True if there are cards to pull, false otherwise.
  */

bool CardGameBasicRummy::DrawCard(int nPlayer, const GameMove &cGameMove)
{
    std::string sMessage {};

    if (cGameMove.IsArgument(GameVocabulary::ARG_STOCK))
    {
        if (m_cDeck.HasCards())
        {
            sMessage = "Player " + std::to_string(nPlayer) + " draws from the stock";
            m_cLogger.LogInfo(sMessage,1);

            Card cCard = m_cDeck.DrawTopCard();
            m_vHands[nPlayer - 1].AddCard(cCard);

            return true;
        }
        else
        {
            m_cLogger.LogInfo("No cards left in the stock to draw from" ,1);
            return false;
        }
    }

    if (cGameMove.IsArgument(GameVocabulary::ARG_DISCARD))
    {
        if (m_cDiscardPile.HasCards())
        {
            sMessage = "Player " + std::to_string(nPlayer) + " draws from the discard pile";
            m_cLogger.LogInfo(sMessage,1);

            Card cCard = m_cDiscardPile.DrawTopCard();
            m_vHands[nPlayer - 1].AddCard(cCard);

            return true;
        }
        else
        {
            m_cLogger.LogInfo("No cards left in the discard pile to draw from" ,1);
            return false;
        }
    }

    if (cGameMove.Argument().empty())
    {
        sMessage = "Player " + std::to_string(nPlayer) + " does not know where to draw from";
    }
    else
    {
        sMessage = "Player " + std::to_string(nPlayer) + " cannot draw from " + cGameMove.Argument();
    }

    return false;
}

/**
  * Meld cards from hand.
  *
  * \param nPlayer   The player whose turn it is.
  * \param cGameMove The player's game move object
  *
  * \return True if there are cards to meld, false otherwise.
  */

bool CardGameBasicRummy::MeldCards(int nPlayer, const GameMove &cGameMove)
{
    std::string sMessage {};

    // Number of cards in game move must be at least the match number
    if (cGameMove.NumberOfCards() < m_knMatchNumber)
    {
        sMessage = "Player " + std::to_string(nPlayer) + " tries to meld " +
            std::to_string(cGameMove.NumberOfCards()) + " cards.  Must be at least " +
            std::to_string(m_knMatchNumber) + ".";
        m_cLogger.LogInfo(sMessage,1);
        return false;
    }

    // Generate a match from the hand
    std::vector<Card> vCards = cGameMove.GetCards();
    Match cMatch = m_vHands[nPlayer - 1].RemoveMatch(vCards, m_knMatchNumber);
    if (cMatch.HasCards() >= m_knMatchNumber)
    {
        // Insert into matches
        m_uommMatches.insert(std::make_pair(m_vHands[nPlayer - 1].ID(), cMatch));
        return true;
    }

    return false;
}

/**
  * Layoff cards from hand.
  *
  * Layoff cards from hand onto other melds.
  *
  * \param nPlayer   The player whose turn it is.
  * \param cGameMove The player's game move object
  *
  * \return True if there are cards to layoff, false otherwise.
  */

bool CardGameBasicRummy::LayoffCard(int nPlayer, const GameMove &cGameMove)
{
    std::vector<Card> vCards = cGameMove.GetCards();
    return m_vHands[nPlayer - 1].RemoveLayoffs(m_uommMatches, vCards[0]);
}

/**
  * Discard card from hand.
  *
  * Discard card from hand onto the discard pile.
  *
  * \param nPlayer   The player whose turn it is.
  * \param cGameMove The player's game move object
  *
  * \return True if there is acard to discard, false otherwise.
  */

bool CardGameBasicRummy::Discard(int nPlayer, const GameMove &cGameMove)
{
    std::vector<Card> vCards = cGameMove.GetCards();
    m_vHands[nPlayer - 1].Discard(m_cDiscardPile, vCards[0]);

    return true;
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

bool CardGameBasicRummy::ApplyMove(int nPlayer, GameMove &cGameMove)
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
        std::cout << "The discard pile has " << m_cDiscardPile.HasCards() << " cards." << std::endl;
        std::cout << "Rank Matches:     " << MatchesTypes("R") << std::endl;
        std::cout << "Sequence Matches: " << MatchesTypes("S") << std::endl;

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

    // Generate a vector of all possible valid moves for the player
    std::vector<GameMove> vGameMoves = GenerateMoves(nPlayer);

    // Compare passed GameMove to generated game moves.  If one is found to be
    // the same, make the move on the board.
    bool bValidMove = false;
    for (GameMove &cValidGameMove : vGameMoves)
    {
        if (cValidGameMove.SameCommand(cGameMove))
        {
            bValidMove = true;
            break;
        }
    }

    // If move is not valid, return false
    if (!bValidMove)
    {
        return false;
    }

    //
    // Apply move to the game
    //

    // Check for draw
    if (cGameMove.Draw())
    {
        cGameMove.SetAnotherTurn(true);
        vGameMoves.push_back(cGameMove);
        if (!DrawCard(nPlayer, cGameMove))
            return false;
    }

    // Check for meld
    if (cGameMove.Meld())
    {
        cGameMove.SetAnotherTurn(true);
        vGameMoves.push_back(cGameMove);
        if (!MeldCards(nPlayer, cGameMove))
            return false;
    }

    // Check for layoff
    if (cGameMove.Layoff())
    {
        cGameMove.SetAnotherTurn(true);
        vGameMoves.push_back(cGameMove);
        if (!LayoffCard(nPlayer, cGameMove))
            return false;
    }

    // Check for discard
    if (cGameMove.Discard())
    {
        vGameMoves.push_back(cGameMove);
        if (!Discard(nPlayer, cGameMove))
            return false;
    }

    // If players has cards, sort them
    if (m_vHands[nPlayer - 1].HasCards())
    {
        m_vHands[nPlayer - 1].SortByRank();
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

std::string CardGameBasicRummy::AnnounceMove(int nPlayer, const GameMove &cGameMove) const
{
    return "\rPlayer " + std::to_string(nPlayer) + ": " + cGameMove.Command() + " " + cGameMove.AnnounceCard();
}

/**
  * Return a string providing a current score of the game.
  *
  * Count the number of books for each player.
  *
  * \return A string containing the game score.
  */

std::string CardGameBasicRummy::GameScore() const
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

    // Display meld counts for the game score

    std::string sScore {"\nMeld Count:"};

    for (const Hand &cHand : m_vHands)
    {
        sScore += " Player " + std::to_string(cHand.ID()) + "=" + std::to_string(m_uommMatches.count(cHand.ID()));
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

std::string CardGameBasicRummy::GameStatistics() const
{
    std::string sGameStats = "Successful Asks\n";
    ////sGameStats += "Player 1 = " + std::to_string(m_aiSuccessfulAsks[0]) + " (" + std::to_string(static_cast<float>(m_aiSuccessfulAsks[0]) / static_cast<float>(m_nNumberOfMoves) * 100.0) + "%)\n";
    ////sGameStats += "Player 2 = " + std::to_string(m_aiSuccessfulAsks[1]) + " (" + std::to_string(static_cast<float>(m_aiSuccessfulAsks[1]) / static_cast<float>(m_nNumberOfMoves) * 100.0) + "%)\n";

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

bool CardGameBasicRummy::GameEnded(int nPlayer)
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
/*            if (static_cast<int>(m_uommBooks.count(cHand.ID())) > nNumberOfBooks)
            {
                nNumberOfBooks = m_uommBooks.count(cHand.ID());
                m_nWinner = cHand.ID();
            }
*/
        }

        m_sWinBy = "having " + std::to_string(nNumberOfBooks) + " books";
        m_bGameOver = true;
        return true;
    }

    return false;
}

/**
  * Initialize the blackboard
  *
  * Initialize the Probable Deck and the Probable Opponent Hand.
  *
  */

void CardGameBasicRummy::BlackboardInitialize(int nPlayer, Blackboard &cBlackboard) const
{
    //
    // Set the number of cards the ProbableDeck and the ProbableOpponentHand should have
    //

    // ProbableDeck: Subtract size of player's hand multiplied by two from ProbableDeck
    cBlackboard.m_cProbableDeck.SetNumberOfCards(cBlackboard.m_cProbableDeck.HasCards() - (m_vHands[1 - nPlayer].HasCards() * 2));

    // ProbableOpponentHand: Set to number of cards in player's hand
    cBlackboard.m_cProbableOpponentHand.SetNumberOfCards(m_vHands[1 - nPlayer].HasCards());

    // Remove cards matching the player's hand from ProbableDeck
    std::vector<Card> vCards {};
    for (const Card &cCard : m_vHands[nPlayer -1].Cards())
    {
        // Remove one card of sRank from ProbableDeck
        vCards = cBlackboard.m_cProbableDeck.RemoveCardsOfRank(cCard.Rank(), 1);
        if (vCards.size() != 1)
        {
            std::string sError = "Expected to remove one card from probable deck, but removed " + std::to_string(vCards.size());
            throw GameAIException(sError);
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

    // Set initialized flag
    cBlackboard.SetInitialized();

    return;
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

GameMove CardGameBasicRummy::BlackboardMove(int nPlayer, Blackboard &cBlackboard, int nProbability) const
{
    // Probability threshold
    float fProbabilityThreshold = static_cast<float>(nProbability) / 10;

    // Probability of pulling card
    float fProbabilityOfPullingCard {};

    // Logging messages
    std::string sLogMessage {};

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
            " * " + std::to_string(cProbableCard.Probability()) + "]";
        m_cLogger.LogInfo(sLogMessage, 3);

        if (fProbabilityOfPullingCard >= fProbabilityThreshold)
        {
            for (Card &cCard : m_vHands[nPlayer - 1].Cards())
            {
                if (cProbableCard.Rank() == cCard.Rank())
                {
                    cGameMove.UpdateCard(cProbableCard);
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
    //TODO: cProbableDeck.SortByProbability();
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
                    cGameMove.UpdateCard(cProbableCard);
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
    Card cLastCard;
    Card cBestCard;
    int nBestCardNumber {INT_MIN};
    int nThisCardNumber {100};

    // Loop through my hands in order of number of Ranks in hand (sorted by ApplyMove)
    for (Card &cCard : m_vHands[nPlayer - 1].Cards())
    {
        if (cLastCard.Rank() != cCard.Rank())
        {
            cLastCard = cCard;
            nThisCardNumber = 100 - cBlackboard.Asks(cCard.Rank());
        }
        else
        {
            ++nThisCardNumber;
        }

        if (nThisCardNumber > nBestCardNumber)
        {
            nBestCardNumber = nThisCardNumber;
            cBestCard = cCard;
        }
    }

    cGameMove.UpdateCard(cBestCard);
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
  * \param cBlackboatrd The blackboard for this player
  *
  */

void CardGameBasicRummy::BlackboardUpdate(int nPlayer, Blackboard &cBlackboard)
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

    std::string sRank = cLastMove.GetCard().Rank();
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
/*
            for (int iii = 0; iii < m_knBookNumber - nNumberOfCardsOfRankInMyHand; ++iii)
            {
                sLogMessage = "Update Prob Deck: P(" + sRank + ")=" + std::to_string(cCard.Probability());
                m_cLogger.LogInfo(sLogMessage,3);

                cBlackboard.m_cProbableDeck.AddCard(cCard);
            }
*/
            // Update success stats
            ////++m_aiSuccessfulAsks[cLastMove.PlayerNumber() - 1];
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
/*
            for (int iii = 0; iii < m_knBookNumber - nNumberOfCardsOfRankInMyHand; ++iii)
            {
                sLogMessage = "Update Prob Deck: P(" + sRank + ")=" + std::to_string(cCard.Probability());
                m_cLogger.LogInfo(sLogMessage,3);

                cBlackboard.m_cProbableDeck.AddCard(cCard);
            }
*/
        }
        // Else Ask and Go-Fish not Successful
        else
        {
            //
            // Update ProbableOpponentHand
            //
            // Remove cards of rank from ProbableOpponentHand
            cBlackboard.m_cProbableOpponentHand.RemoveCardsOfRank(sRank);
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
                sLogMessage = "Update Prob Hand: P(" + sRank + ")=" + std::to_string(cCard.Probability());
                m_cLogger.LogInfo(sLogMessage,3);

                cBlackboard.m_cProbableOpponentHand.AddCard(cCard);
            }
            // Add cards of rank back to ProbableOpponentHand with less than certainty
            cCard.SetProbability(0.0);
/*
            for (int iii = nCards + 1; iii < m_knBookNumber; ++iii)
            {
                sLogMessage = "Update Prob Hand: P(" + sRank + ")=" + std::to_string(cCard.Probability());
                m_cLogger.LogInfo(sLogMessage,3);

                cBlackboard.m_cProbableOpponentHand.AddCard(cCard);
            }
*/

            //
            // Update ProbableDeck
            //
            // Remove any cards of rank from ProbableDeck
            cBlackboard.m_cProbableDeck.RemoveCardsOfRank(sRank);

            // Add cards of rank back to ProbableDeck
            //Card cCard;
            cCard.SetRank(sRank);
            cCard.SetProbability(0.0);
/*
            for (int iii = nCards + 1; iii < m_knBookNumber; ++iii)
            {
                sLogMessage = "Update Prob Deck: P(" + sRank + ")=" + std::to_string(cCard.Probability());
                m_cLogger.LogInfo(sLogMessage,3);

                cBlackboard.m_cProbableDeck.AddCard(cCard);
            }
*/
            // Update success stats
            ////++m_aiSuccessfulAsks[cLastMove.PlayerNumber() - 1];
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
                sLogMessage = "Update Prob Hand: P(" + sRank + ")=" + std::to_string(cCard.Probability());
                m_cLogger.LogInfo(sLogMessage,3);

                cBlackboard.m_cProbableOpponentHand.AddCard(cCard);
            }

            // Add cards of rank back to ProbableOpponentHand with less than certainty
            cCard.SetProbability(0.0);
/*
            for (int iii = nCards + 1; iii < m_knBookNumber; ++iii)
            {
                sLogMessage = "Update Prob Hand: P(" + sRank + ")=" + std::to_string(cCard.Probability());
                m_cLogger.LogInfo(sLogMessage,3);

                cBlackboard.m_cProbableOpponentHand.AddCard(cCard);
            }
*/
            //
            // Update ProbableDeck
            //
            // Remove any cards of rank from ProbableDeck
            cBlackboard.m_cProbableDeck.RemoveCardsOfRank(sRank);

            // Add cards of rank back to ProbableDeck
            //Card cCard;
            cCard.SetRank(sRank);
            cCard.SetProbability(0.0);
/*
            for (int iii = nCards + 1; iii < m_knBookNumber; ++iii)
            {
                sLogMessage = "Update Prob Deck: P(" + sRank + ")=" + std::to_string(cCard.Probability());
                m_cLogger.LogInfo(sLogMessage,3);

                cBlackboard.m_cProbableDeck.AddCard(cCard);
            }
*/
        }
        // Else Ask and Go-Fish not Successful
        else
        {
            //
            // Update ProbableOpponentHand
            //

            // Remove cards of rank from ProbableOpponentHand
            cBlackboard.m_cProbableOpponentHand.RemoveCardsOfRank(sRank);
        }
    }

    //
    // Update based on books
    //
    sRank.clear();
/*
    for (const char &cToken : BooksUniqueRanks())
    {
        if (cToken != ' ')
        {
            sRank += cToken;
        }
        else
        {
            cBlackboard.m_cProbableDeck.RemoveCardsOfRank(sRank);
            cBlackboard.m_cProbableOpponentHand.RemoveCardsOfRank(sRank);
            sRank.clear();
        }
    }
*/
    //
    // Set the number of cards the ProbableDeck and the ProbableOpponentHand should have
    //

    // Update number of cards in Probable Deck and Probable Opponent's Hand
    cBlackboard.m_cProbableDeck.SetNumberOfCards(m_cDeck.HasCards());
    cBlackboard.m_cProbableOpponentHand.SetNumberOfCards(m_vHands[2 - nPlayer].HasCards());

    // Update rank probabilities
    cBlackboard.m_cProbableDeck.UpdateRankProbabilities(cBlackboard.m_cProbableOpponentHand.NumberOfCards());
    cBlackboard.m_cProbableOpponentHand.UpdateRankProbabilities(cBlackboard.m_cProbableDeck.NumberOfCards());

    return;
}

/**
  * Status of current matches.
  *
  * Return a string representing the current matches
  *
  * \return A string containing the matches.
  *
  */

std::string CardGameBasicRummy::MatchesTypes(const std::string&sTypes) const
{
    std::string sMatches {};

    for (const auto &PlayerMatch : m_uommMatches)
    {
        if (sTypes.compare("R") && PlayerMatch.second.TypeSameRank())
        {
            sMatches += PlayerMatch.second.RanksAndSuits() + " | ";
        }
        else if (sTypes.compare("S") && PlayerMatch.second.TypeSequence())
        {
            sMatches += PlayerMatch.second.RanksAndSuits() + " | ";
        }
        else
        {
            sMatches += PlayerMatch.second.RanksAndSuits() + " | ";
        }
    }

    if (sMatches.empty())
        sMatches = "none";

    return sMatches;
}

/**
  * Serialize the matches into a Json::Value object.
  *
  * \return The Json::Value object representing the matches.
  */

Json::Value CardGameBasicRummy::MatchesJsonSerialization() const
{
    Json::Value jValue;
    Json::Value jMatches;
    Match cMatch;

    // The following are used to create a index that will retain uniqueness when
    // multimap items serialized to JSON.  May be used for up to 260 unique
    // values: a0 to z9.
    int nCounter {0};
    char chLetter = 'a';
    std::string sIndex {};

    for (const auto &PlayerMatch : m_uommMatches)
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
        jValue["Player"] = PlayerMatch.first;
        cMatch           = PlayerMatch.second;
        jValue["Match"]  = cMatch.JsonSerialization();

        // Add the jValue to jBooks
        jMatches[sIndex] = jValue;
    }

    return jMatches;
}

/**
  * Deserialize matches from a JSON string.
  *
  * \param sJsonMatches  A JSON string representing matches.
  * \param sErrorMessage A string to return an error message if needed
  *
  * \return True if deserialization is successful, false otherwise
  */

bool CardGameBasicRummy::MatchesJsonDeserialization(const std::string &sJsonMatches, std::string &sErrorMessage)
{
    Json::Reader jReader;
    Json::Value  jMatches;
    int nPlayer;
    Match cMatch;

    // Parse the JSON string into a Json object,
    if (jReader.parse(sJsonMatches, jMatches, false))
    {
        m_uommMatches.clear();

        for (const Json::Value &jValue : jMatches)
        {
            nPlayer = jValue["Player"].asInt();

            if (cMatch.JsonDeserialization(jValue["Match"], sErrorMessage))
            {
                // Insert players's book into Books
                m_uommMatches.insert(std::make_pair(nPlayer, cMatch));
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
