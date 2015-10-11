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

    m_cAllowedMoves.NextMoveInSequence(vGameMoves);
    EvaluatePossibleMoves(nPlayer, vGameMoves);

    for (const GameMove &cGameMove : vGameMoves)
    {
        sMoves += "\n" + cGameMove.Command() + GameVocabulary::DELIMETER + cGameMove.Argument();
    }

    sMoves += "\nThe top of the discard pile is ";
    if (m_cDiscardPile.HasCards() > 0)
    {
        sMoves += m_cDiscardPile.PeekAtTopCard().DisplayShortName();
    }
    else
    {
        sMoves += "empty";
    }

    m_vHands[nPlayer -1].SortByRank();
    sMoves += "\nYour hand is " + m_vHands[nPlayer - 1].DisplayCards(true);

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

bool CardGameBasicRummy::DrawCard(int nPlayer, GameMove &cGameMove)
{
    std::string sMessage {};

    if (cGameMove.IsArgument(GameVocabulary::ARG_STOCK))
    {
        if (m_cDeck.HasCards())
        {
            Card cCard = m_cDeck.DrawTopCard();

            sMessage = "Player " + std::to_string(nPlayer) + " draws " +
                cCard.DisplayShortName(true) + " from the stock";
            m_cLogger.LogInfo(sMessage,1);

            m_vHands[nPlayer - 1].AddCard(cCard);
            m_vHands[nPlayer - 1].SortByRank();

            // Add card to Game Move for Blackboard updates
            cGameMove.AddCard(cCard);

            ++m_aiNumberOfDrawsFromStock[nPlayer - 1];

            // If stock has been depleted, the top card of the discard pile
            // is set aside and the remainder of the discard pile is turned
            // over to form a new stock.
            if (m_cDeck.HasCards() == 0)
            {
                m_cLogger.LogInfo("Stock has been depleted.  Creating new stock from discard pile.",1);

                // Grab top card from discard pile
                Card cNewDiscardPileCard = m_cDiscardPile.DrawTopCard();

                // Remove all cards from the discard pile and add them to the stock
                std::vector<Card> vNewStockCards = m_cDiscardPile.RemoveAllCards();
                for (Card &cCard : vNewStockCards)
                {
                    cCard.TurnUp(false);
                    m_cDeck.AddCard(cCard);
                }

                // Add top card from discard pile back to discard pile
                m_cDiscardPile.AddCard(cNewDiscardPileCard);
            }

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
            m_vHands[nPlayer - 1].SortByRank();

            ++m_aiNumberOfDrawsFromDiscard[nPlayer - 1];

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

bool CardGameBasicRummy::MeldCards(int nPlayer, GameMove &cGameMove)
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

    // Grab cards
    std::vector<Card> vCards = cGameMove.GetCards();

    // Ensure cards in the player's hand
    for (const Card &cCard : vCards)
    {
        if (!m_vHands[nPlayer - 1].HasCard(cCard))
        {
            return false;
        }
    }

    // Generate a match from the hand
    Match cMatch = m_vHands[nPlayer - 1].RemoveMatch(vCards, m_knMatchNumber);
    if (cMatch.HasCards() >= m_knMatchNumber)
    {
        // Insert into matches
        m_uommMatches.insert(std::make_pair(m_vHands[nPlayer - 1].ID(), cMatch));

        ++m_aiNumberOfMelds[nPlayer - 1];

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

bool CardGameBasicRummy::LayoffCard(int nPlayer, GameMove &cGameMove)
{
    std::vector<Card> vCards = cGameMove.GetCards();

    // Ensure the cards in the player's hand
    if (!m_vHands[nPlayer - 1].HasCard(vCards[0]))
    {
        return false;
    }

    if (m_vHands[nPlayer - 1].RemoveLayoffs(m_uommMatches, vCards[0]))
    {
        ++m_aiNumberOfLayoffs[nPlayer - 1];
        return true;
    }

    return false;
}

/**
  * Discard card from hand.
  *
  * Discard card from hand onto the discard pile.
  *
  * \param nPlayer   The player whose turn it is.
  * \param cGameMove The player's game move object
  *
  * \return True if there is a card to discard, false otherwise.
  */

bool CardGameBasicRummy::Discard(int nPlayer, GameMove &cGameMove)
{
    std::vector<Card> vCards = cGameMove.GetCards();

    // Ensure the cards in the player's hand
    if (!m_vHands[nPlayer - 1].HasCard(vCards[0]))
    {
        return false;
    }

    vCards[0].TurnUp(true);
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

        // Display game
        for (Hand &cHand : m_vHands)
        {
            std::cout << "Player " << cHand.ID() << " has " << cHand.HasCards() << " cards." << std::endl;
        }
        std::cout << "The stock has " << m_cDeck.HasCards() << " cards." << std::endl;
        std::cout << "The discard pile has " << m_cDiscardPile.HasCards() << " cards." << std::endl;
        std::cout << "Rank Matches:     " << MatchesTypes(MatchType::TYPE_SAME_RANK) << std::endl;
        std::cout << "Sequence Matches: " << MatchesTypes(MatchType::TYPE_SEQUENCE) << std::endl;

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
        cGameMove.SetPlayerNumber(nPlayer);
        //vGameMoves.push_back(cGameMove);
        if (!DrawCard(nPlayer, cGameMove))
            return false;

        m_cAllowedMoves.ProgressSequence();
    }

    // Check for meld
    if (cGameMove.Meld())
    {
        cGameMove.SetAnotherTurn(true);
        cGameMove.SetPlayerNumber(nPlayer);
        //vGameMoves.push_back(cGameMove);

        if (!MeldCards(nPlayer, cGameMove))
            return false;
    }

    // Check for layoff
    if (cGameMove.Layoff())
    {
        cGameMove.SetAnotherTurn(true);
        cGameMove.SetPlayerNumber(nPlayer);
        //vGameMoves.push_back(cGameMove);

        if (!LayoffCard(nPlayer, cGameMove))
            return false;
    }

    // Check for discard
    if (cGameMove.Discard())
    {
        //vGameMoves.push_back(cGameMove);

        if (!Discard(nPlayer, cGameMove))
            return false;

        m_cAllowedMoves.ProgressSequence();
    }

    // If players has cards, sort them and turn off Rummy possibility
    if (m_vHands[nPlayer - 1].HasCards())
    {
        m_vHands[nPlayer - 1].SortByRank();
        SetRummyOff(nPlayer); //TODO - This turns off Rummy too soon; should be set after a series of melds and layoffs and a discard
                              //TODO - Make sure this gets re-initialized after the go out or Rummy
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
    return "\rPlayer " + std::to_string(nPlayer) + ": " + cGameMove.Command() + " " + cGameMove.Argument();
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
    std::string sScore {"\nCurrent Meld Count and Points:"};

    for (const Hand &cHand : m_vHands)
    {
        sScore += "\nPlayer " + std::to_string(cHand.ID()) + " Meld Count=" + std::to_string(m_uommMatches.count(cHand.ID())) +
            " | Points=" + std::to_string(Score(cHand.ID()));
    }

    sScore += "\nTarget Points=" + std::to_string(TargetScore()) + "\n";

    return sScore;
}

/**
  * Return a string providing a current statistics of the game.
  *
  * Per player:
  * The number of melds
  * The number of layoffs
  * The number of draws from the discard
  * The number of draws from the stock
  * The number of hands won and lost
  * The average and median points per hand gained from the opponent
  *
  * \return A string containing the game stats.
  */

std::string CardGameBasicRummy::GameStatistics() const
{
    std::string sGameStats = "Statistic            Player 1       Player 2\n";
    sGameStats            += "Num. of Melds          " + std::to_string(m_aiNumberOfMelds[0])            +
                             "              " + std::to_string(m_aiNumberOfMelds[1])                     + "\n";
    sGameStats            += "Num. of Layoffs        " + std::to_string(m_aiNumberOfLayoffs[0])          +
                             "              " + std::to_string(m_aiNumberOfLayoffs[1])                   + "\n";
    sGameStats            += "Num. of Draws Stock    " + std::to_string(m_aiNumberOfDrawsFromStock[0])   +
                             "              " + std::to_string(m_aiNumberOfDrawsFromStock[1])            + "\n";
    sGameStats            += "Num. of Draws Discard  " + std::to_string(m_aiNumberOfDrawsFromDiscard[0]) +
                             "              " + std::to_string(m_aiNumberOfDrawsFromDiscard[1])          + "\n";
    sGameStats            += "Num. of Hands Won      " + std::to_string(m_aiNumberOfHandsWon[0])         +
                             "              " + std::to_string(m_aiNumberOfHandsWon[1])                  + "\n";
    sGameStats            += "Avg. Points Won / Hand "                                                   +
        std::to_string(static_cast<float>(Score(1)) / static_cast<float>(m_aiNumberOfHandsWon[0]))       +
                                                                                        "              " +
        std::to_string(static_cast<float>(Score(2)) / static_cast<float>(m_aiNumberOfHandsWon[1]))       + "\n";

    return sGameStats;
}

int CardGameBasicRummy::ScoreHand(int nPlayer, int nRummyMultiplier)
{
    int nScore {};
    for (Card &cCard : m_vHands[nPlayer - 1].Cards())
    {
        nScore += cCard.RankValue() * nRummyMultiplier;
    }

    return nScore;
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

    // nPlayer represents the player to move next (the opponent)
    // Change to player that just moved
    int nThisPlayer = 3 - nPlayer;

    // Evaluate whether the player has any cards in their hand
    if (m_vHands[nThisPlayer - 1].HasCards() == 0)
    {
        int nRummyMultiplier {0};
        std::string sMessage {};

        if (Rummy(nThisPlayer))
        {
            sMessage = "Player " + std::to_string(nThisPlayer) + " has gone Rummy!";
            nRummyMultiplier = 2;
        }
        else
        {
            sMessage = "Player " + std::to_string(nThisPlayer) + " has gone out.";
            nRummyMultiplier = 1;
        }

        m_cLogger.LogInfo(sMessage, 1);

        // Find total of cards in opponent's hand
        int nScore = ScoreHand(nPlayer, nRummyMultiplier);
        sMessage = "Player " + std::to_string(nPlayer) + " has a hand worth " +
            std::to_string(nScore) + "; added to Player " +
            std::to_string(nThisPlayer) + "'s cumulative score";
        m_cLogger.LogInfo(sMessage, 1);
        AddToScore(nThisPlayer, nScore);

        std::cout << GameScore() << std::endl;

        ++m_aiNumberOfHandsWon[nThisPlayer - 1];

        // If this player's total score is greater than or equal to the goal score, this player wins
        if (Score(nThisPlayer) >= TargetScore())
        {
            m_nWinner = nThisPlayer;
            m_sWinBy = "having " + std::to_string(Score(nThisPlayer)) + " points.";
            m_bGameOver = true;
            return true;
        }
        // Else start a new hand of Rummy
        else
        {
            m_cLogger.LogInfo("New hand being dealt", 1);
            BeginHand();
        }
    }

    return false;
}

/**
  * Begin a hand of Basic Rummy.
  *
  * Shuffle and deal cards to the player's hands and start the
  * discard pile.
  *
  */

void CardGameBasicRummy::BeginHand()
{
    // Initialize deck
    m_cDeck.Initialize();
    m_cDeck.SetAcesLow();

    // Shuffle and deal cards
    m_cDeck.Shuffle();
    m_cDeck.Deal(10, m_vHands);

    // Deal next card up to start the discard pile
    m_cDiscardPile.RemoveAllCards();
    Card cCard = m_cDeck.DrawTopCard();
    cCard.TurnUp(true);
    m_cDiscardPile.AddCard(cCard);

    // Clear matches
    m_uommMatches.clear();

    // Sort hands
    for (Hand &cHand : m_vHands)
    {
        cHand.SortByRank();
    }

    // Set allowed moves sequence
    m_cAllowedMoves.ResetSequence();
}

/**
  * Initialize the blackboard
  *
  * Initialize the Probable Deck and the Probable Opponent Hand.
  *
  * TODO: Move to CardGame
  */

void CardGameBasicRummy::BlackboardInitialize(int nPlayer, Blackboard &cBlackboard) const
{
    // Remove the cards on the discard pile from the deck
    std::vector<Card> vCards = m_cDiscardPile.Cards();
    cBlackboard.m_cProbableDeck.RemoveCards(vCards);

    // Call parent class to finish BB init
    CardGame::BlackboardInitialize(nPlayer, cBlackboard);
}

/**
  * Generate a move from the Blackboard
  *
  * Evaluate the probability of successfully pulling needed cards from the
  * probable deck (aka, stock).
  *
  * \param nPlayer      The player
  * \param cBlackboatrd The blackboard for this player
  *
  * \return A game move.
  */

GameMove CardGameBasicRummy::BlackboardMove(int nPlayer, Blackboard &cBlackboard, int nProbability)
{
    int nOpportunities {};
    std::string sCards {};
    std::vector<Card> vDoNotDiscard {};

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

    // Generate a vector of all possible valid moves for the player
    std::vector<GameMove> vGameMoves = GenerateMoves(nPlayer);

    // Evaluate all possible moves
    for (const GameMove &cPossibleGameMove : vGameMoves)
    {
        //
        // Evaluate DRAW
        //
        if (cPossibleGameMove.Draw())
        {
            // If possible to draw from stock and Game Move is not already set, set it
            if (cPossibleGameMove.IsArgument(GameVocabulary::ARG_STOCK))
            {
                m_cLogger.LogInfo("Evaluating DRAW STOCK move", 3);

                if (!cGameMove.Draw())
                {
                    cGameMove.SetDraw(true);
                    cGameMove.SetArgument(GameVocabulary::ARG_STOCK);
                }
            }

            // Evaluate possibly drawing from the discard pile
            if (cPossibleGameMove.IsArgument(GameVocabulary::ARG_DISCARD))
            {
                m_cLogger.LogInfo("Evaluating DRAW DISCARD move", 3);

                // Add top card from discard pile to theoretical hand
                Hand cTheoreticalHand = m_vHands[nPlayer - 1];
                cTheoreticalHand.AddCard(m_cDiscardPile.PeekAtTopCard());

                // Evaluate for Layoff
                if (cTheoreticalHand.LayoffOpportunities(m_uommMatches))
                {
                    nOpportunities = 0;
                    for (const Card &cCard : cTheoreticalHand.Cards())
                    {
                        // Count the number of cards in the hand that are eligible
                        // for a layoff
                        if (cCard.Eligible())
                            ++nOpportunities;
                    }

                    // If number of layoffs with the discard card are greater than
                    // the number of layoffs without it, set the game move to draw
                    // from the discard pile
                    if (nOpportunities > cBlackboard.LayoffOpportunities())
                    {
                        cGameMove.SetDraw(true);
                        cGameMove.SetArgument(GameVocabulary::ARG_DISCARD);
                    }
                }
                // Evaluate for Matches
                else if (cTheoreticalHand.MatchOpportunities(m_knMatchNumber))
                {
                    nOpportunities = 0;
                    for (const Card &cCard : cTheoreticalHand.Cards())
                    {
                        // Count the number of cards in the hand that are eligible
                        // for a match
                        if (cCard.Eligible())
                            ++nOpportunities;
                    }

                    // If number of matches with the discard card are greater than
                    // the number of matches without it, set the game move to draw
                    // from the discard pile
                    if (nOpportunities > cBlackboard.MatchOpportunities())
                    {
                        cGameMove.SetDraw(true);
                        cGameMove.SetArgument(GameVocabulary::ARG_DISCARD);
                    }
                }
                // Evaluate for Near Matches
                else if (cTheoreticalHand.MatchOpportunities(m_knMatchNumber - 1))
                {
                    nOpportunities = 0;
                    for (const Card &cCard : cTheoreticalHand.Cards())
                    {
                        // Count the number of cards in the hand that are eligible
                        // for a match
                        if (cCard.Eligible())
                            ++nOpportunities;
                    }

                    // If number of matches with the discard card are greater than
                    // the number of matches without it, set the game move to draw
                    // from the discard pile
                    if (nOpportunities > cBlackboard.NearMatchOpportunities())
                    {
                        cGameMove.SetDraw(true);
                        cGameMove.SetArgument(GameVocabulary::ARG_DISCARD);
                    }
                }
            } //  if (cPossibleGameMove.IsArgument(GameVocabulary::ARG_DISCARD))
        } //  if (cPossibleGameMove.Draw())

        //
        // Evaluate MELD
        //
        if (cPossibleGameMove.Meld())
        {
            m_cLogger.LogInfo("Evaluating MELD move", 3);

            cGameMove.SetMeld(true);

            std::string sArg {};

            // Find cards to meld - Sequence first, then Book (same rank)
            if (!m_vHands[nPlayer -1].MatchOpportunities(m_knMatchNumber, true, false))
            {
                m_vHands[nPlayer -1].MatchOpportunities(m_knMatchNumber, false, true);
            }

            for (const Card &cCard : m_vHands[nPlayer - 1].Cards())
            {
                // Add cards to GameMove
                if (cCard.Eligible())
                {
                    cGameMove.AddCard(cCard);
                    sArg += cCard.DisplayShortName(true) + " ";
                }
            }

            cGameMove.SetArgument(sArg);
        }

        //
        // Evaluate a LAYOFF
        //
        // If not already set to a Meld
        if (cPossibleGameMove.Layoff() && !cGameMove.Meld())
        {
            m_cLogger.LogInfo("Evaluating LAYOFF move", 3);

            cGameMove.SetLayoff(true);

            // Find cards to layoff
            m_vHands[nPlayer -1].LayoffOpportunities(m_uommMatches);
            for (const Card &cCard : m_vHands[nPlayer - 1].Cards())
            {
                // Add cards to GameMove
                if (cCard.Eligible())
                {
                    cGameMove.AddCard(cCard);
                    cGameMove.SetArgument(cCard.DisplayShortName(true));
                    break;
                }
            }
        }

        //
        // Evaluate DISCARD
        //
        // TODO: don't discard card that has multiple opportunities
        // If not already a Meld or Layoff
        if (cPossibleGameMove.Discard() && !cGameMove.Meld() && !cGameMove.Layoff())
        {
            m_cLogger.LogInfo("Evaluating DISCARD move", 3);

            cGameMove.SetDiscard(true);

            // Evaluate hand to decide which card to discard
            Hand cTheoreticalHand = m_vHands[nPlayer - 1];

            // Evaluate for near Match opportunities in player's hand
            if (cTheoreticalHand.MatchOpportunities(m_knMatchNumber - 1))
            {
                for (const Card &cCard : cTheoreticalHand.Cards())
                {
                    // Remove cards that are near matching
                    if (cCard.Eligible())
                    {
                        cTheoreticalHand.RemoveCard(cCard);
                    }
                }
            }

            // If cards still exist in the hand, sort by rank and select the highest to discard
            if (cTheoreticalHand.HasCards() > 0)
            {
                cTheoreticalHand.SortByRank();
                cGameMove.AddCard(cTheoreticalHand.PeekAtBottomCard());
                cGameMove.SetArgument(cTheoreticalHand.PeekAtBottomCard().DisplayShortName(true));
            }
            // Else no cards exist in theoretical hand; select highest ranking from real hand
            else
            {
                m_vHands[nPlayer - 1].SortByRank();
                m_vHands[nPlayer - 1].SortBySuit();
                cGameMove.AddCard(m_vHands[nPlayer - 1].PeekAtBottomCard());
                cGameMove.SetArgument(m_vHands[nPlayer - 1].PeekAtBottomCard().DisplayShortName(true));
            }
        }
    } // for (const GameMove &cPossibleGameMove : vGameMoves)

    return cGameMove;
}

/**
  * Update the blackboard
  *
  * Update the probability that cards exist in the Probable Deck and the
  * Probable Opponent's Hand based on the last move.
  *
  * \param nPlayer     The player
  * \param cBlackboard The blackboard for this player
  *
  */

void CardGameBasicRummy::BlackboardUpdate(int nPlayer, Blackboard &cBlackboard)
{
    Hand cHandToEvaluate {};
    std::string sLogMessage {};

    // If not initialized, initialize
    if (!cBlackboard.Initialized()) // TODO: Need to re-initialize upon a new hand
    {
        BlackboardInitialize(nPlayer, cBlackboard);
    }

    //
    // Check last move
    //
    GameMove cLastMove = LastMove();
    std::vector<Card> vCards = cLastMove.GetCards();

    // Player turn
    if (cLastMove.PlayerNumber() == nPlayer)
    {
        // Evaluate a DRAW move from the stock
        if (cLastMove.Draw() && cLastMove.IsArgument(GameVocabulary::ARG_STOCK))
        {
            // Update ProbableDeck - remove drawn card from ProbableDeck
            cBlackboard.m_cProbableDeck.RemoveCard(vCards[0]);
        }
    }
    // Opponent turn
    else
    {
        // Evaluate a MELD, LAYOFF, or DISCARD
        if (cLastMove.Meld() || cLastMove.Layoff() || cLastMove.Discard())
        {
            // Update ProbableDeck
            cBlackboard.m_cProbableDeck.RemoveCards(vCards);

            // Update ProbableHand
            cBlackboard.m_cProbableOpponentHand.RemoveCards(vCards);
        }
    }

    // Update number of cards in Probable Deck and Probable Opponent's Hand
    cBlackboard.m_cProbableDeck.SetNumberOfCards(m_cDeck.HasCards());
    cBlackboard.m_cProbableOpponentHand.SetNumberOfCards(m_vHands[2 - nPlayer].HasCards());

    // Update rank probabilities
    cBlackboard.m_cProbableDeck.UpdateRankProbabilities(cBlackboard.m_cProbableOpponentHand.NumberOfCards());
    cBlackboard.m_cProbableOpponentHand.UpdateRankProbabilities(cBlackboard.m_cProbableDeck.NumberOfCards());

    // Evaluate for Layoff opportunities in player's hand
    int Opportunities {0};
    if (m_vHands[nPlayer - 1].LayoffOpportunities(m_uommMatches))
    {
        for (const Card &cCard : m_vHands[nPlayer - 1].Cards())
        {
            // Count the number of cards in the hand that are eligible
            // for a layoff
            if (cCard.Eligible())
            {
                ++Opportunities;
            }

        }
    }
    cBlackboard.SetLayoffOpportunities(Opportunities);

    // Evaluate for Match opportunities in player's hand
    Opportunities = 0;
    if (m_vHands[nPlayer - 1].MatchOpportunities(m_knMatchNumber))
    {
        for (const Card &cCard : m_vHands[nPlayer - 1].Cards())
        {
            // Count the number of cards in the hand that are eligible
            // for a match
            if (cCard.Eligible())
            {
                ++Opportunities;
            }

        }
    }
    cBlackboard.SetMatchOpportunities(Opportunities);

    // Evaluate for Near Match opportunities in player's hand
    Opportunities = 0;
    if (m_vHands[nPlayer - 1].MatchOpportunities(m_knMatchNumber - 1))
    {
        for (const Card &cCard : m_vHands[nPlayer - 1].Cards())
        {
            // Count the number of cards in the hand that are eligible
            // for a match
            if (cCard.Eligible())
            {
                ++Opportunities;
            }

        }
    }
    cBlackboard.SetNearMatchOpportunities(Opportunities);
}

/**
  * Status of current matches.
  *
  * Return a string representing the current matches
  *
  * \return A string containing the matches.
  *
  */

std::string CardGameBasicRummy::MatchesTypes(const MatchType ecMatchType) const
{
    std::string sMatches {};

    for (const auto &PlayerMatch : m_uommMatches)
    {
        if (ecMatchType == PlayerMatch.second.Type())
        {
            sMatches += PlayerMatch.second.RanksAndSuits() + " | ";
        }
        else if (ecMatchType == MatchType::TYPE_NONE)
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
