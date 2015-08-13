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
  * For War, this is NOP.
  */

void CardGameGoFish::Display() const
{
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

    if (m_vHands[nPlayer - 1].HasCards() > 0)
    {
        GameMove cGameMove;
        cGameMove.SetAsk(true);
        vGameMoves.push_back(cGameMove);
    }

    return vGameMoves;
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
        // Capture move for later playback or analysis
        m_vGameMoves.push_back(cGameMove);
        return true;
    }

    // If not Resignation, then must be Ask
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

    // Announce move
    m_cLogger.LogInfo(AnnounceMove(nPlayer + 1, cGameMove),1);

    // Find rank in opposing player's hand
    if (m_vHands[nPlayer].HasRank(cGameMove.GetCard().Rank()))
    {
        // Pass cards from asked player to asking player
        std::vector<Card> vCards = m_vHands[nPlayer].RemoveCardsOfRank(cGameMove.GetCard().Rank());

        sMessage = "Player " + std::to_string(2 - nPlayer + 1) + " hands over " + std::to_string(vCards.size());
        m_cLogger.LogInfo(sMessage,1);

        m_vHands[nPlayer - 1].AddCards(vCards);

        cGameMove.SetAnotherTurn(true);
    }
    else // Go Fish
    {
        sMessage = "Player " + std::to_string(2 - nPlayer + 1) + "  says Go Fish";
        m_cLogger.LogInfo(sMessage,1);

        if (m_cDeck.HasCards())
        {
            sMessage = "Player " + std::to_string(2 - nPlayer + 1) + "  draws from the stock";
            m_cLogger.LogInfo(sMessage,1);

            Card cCard = m_cDeck.DrawTopCard();

            if (cGameMove.GetCard().Rank() == cCard.Rank())
            {
                m_cLogger.LogInfo("Card drawn has the same rank as originally asked for" ,1);

                cGameMove.SetAnotherTurn(true);
            }
        }
        else
        {
            m_cLogger.LogInfo("No cards left in the stock to draw from" ,1);
        }
    }

    // Check for books
    /*
    do
    {
        Hand cHand = m_vHands[nPlayer - 1].FindBookByRank(4);
        if (cHand.HasCards())
        {
            m_uomBooks.insert(nPlayer, cHand);
            // check insert
        }
        else
        {
            break
        }
    } while(true);
    */

    // Increment move counter
    ++m_nNumberOfMoves;

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
    return "\rPlayer " + std::to_string(nPlayer) + "  asks for: " + cGameMove.AnnounceCard();;
}

/**
  * Evaluate the game state.
  *
  * From a player's perspective, return a value cooresponding to the player's
  * standing in the game.  If the player has won the game, return a large,
  * positive integer.  If lost return a large negative integer.  Else return zero.
  *
  * \param nPlayer   The player whose turn it is.
  *
  * \return An integer representing game state for the player.
  */

int CardGameGoFish::EvaluateGameState(int nPlayer)
{
    // If won, return largest positive integer // TODO: make these constants
    if (m_nWinner == nPlayer)
        return INT_MAX;

    // If lost, return largest negative integer // TODO: make these constants
    if (m_nWinner == (1 - nPlayer + 2))
        return INT_MIN;

    return 0;
}

/**
  * Return a string providing a current score of the game.
  *
  * Count the cards for each player and the war chest.
  *
  * \return ""
  */

std::string CardGameGoFish::GameScore() const
{
    // Display the score after the second player has played
    static bool bDisplayScore = true;

    // Do not display game score if logging turned off
    if (m_cLogger.Level() < 1)
        return "";

    // Flip flag
    bDisplayScore = !bDisplayScore;
    if (!bDisplayScore)
        return "";

    // Display card counts for the game score
    std::string sScore {"Card Count"};

    for (const Hand &cHand : m_vHands)
    {
        sScore += " Player " + std::to_string(cHand.ID()) + ": " + std::to_string(cHand.HasCards());
    }

    if (m_vWarCards.size() > 0)
    {
        sScore += " War Chest: " + std::to_string(m_vWarCards.size());
    }

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
    m_nWinner = 0;
    m_sWinBy.assign("nothing");

    if (CardGame::GameEnded(nPlayer))
        return true;

    // Evaluate whether the player has any valid moves to make
    std::vector<GameMove> vGameMoves = GenerateMoves(nPlayer);
    if (vGameMoves.empty())
    {
        // Evaluate whether the player has a card in the battle
        std::unordered_map<int, Card>::const_iterator kit = m_uomBattle.find(nPlayer);
        if (kit == m_uomBattle.end())
        {
            m_nWinner = 1 - nPlayer + 2;
            m_sWinBy.assign("attrition");
            return true;
        }
    }

    return false;
}



