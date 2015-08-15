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

#include "CardGameWar.h"

/**
  * Display the cards.
  *
  * For War, this is NOP.
  *
  */

void CardGameWar::Display() const
{
}

/**
  * Return a vector of valid game moves.
  *
  * For War, the only move is to draw, if the player has cards in their hand.
  *
  * \param nPlayer The player whose turn it is.
  *
  * \return A vector of valid moves.
  */

std::vector<GameMove> CardGameWar::GenerateMoves(int nPlayer) const
{
    std::vector<GameMove> vGameMoves {};

    if (m_vHands[nPlayer - 1].HasCards() > 0)
    {
        GameMove cGameMove;
        cGameMove.SetDraw(true);
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

bool CardGameWar::ApplyMove(int nPlayer, GameMove &cGameMove)
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

    // If not Resignation, then must be Draw
    if (!cGameMove.Draw())
    {
        return false;
    }

    //
    // Apply move to the game
    //

    // If War, draw two cards: one down and one up for play
    if (m_bWar)
    {
        if (m_vHands[nPlayer -1].HasCards() >= 2)
        {
            sMessage = "Player " + std::to_string(nPlayer) + " draws a down-facing card";
            m_cLogger.LogInfo(sMessage,1);

            Card cCard = m_vHands[nPlayer - 1].DrawTopCard();
            m_vWarCards.push_back(cCard);
        }
    }

    // Insert players's up card into battle
    Card cCard = m_vHands[nPlayer - 1].DrawTopCard();
    cCard.TurnUp(true);

    sMessage = "Player " + std::to_string(nPlayer) + " draws " + cCard.DisplayRank();
    m_cLogger.LogInfo(sMessage,1);

    bool bInserted = m_uomBattle.insert(std::make_pair(nPlayer, cCard)).second;
    if (!bInserted)
    {
        // Since DrawTopCard is destructive, add the card back
        cCard.TurnUp(false);
        m_vHands[nPlayer - 1].AddCardToTop(cCard);

        return false;
    }

    // If all players have added their cards to the battle, perform the battle
    if (m_uomBattle.size() == m_vHands.size())
    {
        int  nCurrentCardValue       {m_knUnknownValue};
        int  nBestPlayer             {m_knUnknownValue};
        int  nBestCardValue          {m_knUnknownValue};

        m_bWar = false;

        m_cLogger.LogInfo("Battle! ",1);

        // Gather cards and card values
        for (auto &paPlayerCard : m_uomBattle)
        {
            // Add card to war cards vector for winning player
            m_vWarCards.push_back(paPlayerCard.second);

            //
            // Evaluate values and select winning card
            //
            nCurrentCardValue = paPlayerCard.second.Value();

            // If current value equals best value, we have a potential war
            if (nCurrentCardValue == nBestCardValue)
            {
                // Set flag to true
                m_bWar = true;

                // Clear best player
                nBestPlayer = m_knUnknownValue;
            }
            // If current value greater,
            else if (nCurrentCardValue > nBestCardValue)
            {
                // Clear war flag
                m_bWar = false;

                // Update best values
                nBestCardValue = nCurrentCardValue;
                nBestPlayer    = paPlayerCard.first;
            }
        }

        // Clear battle
        m_uomBattle.clear();

        // War?
        if (m_bWar)
        {
            m_cLogger.LogInfo("WAR! Previous battle cards placed in war chest.",1);
        }
        // No War
        else
        {
            // Winning player gets all cards in the war cards vector
            sMessage = "Player " + std::to_string(nBestPlayer) + " Wins";
            m_cLogger.LogInfo(sMessage,1);

            for (Card &cCard : m_vWarCards)
            {
                cCard.TurnUp(false);
                m_vHands[nBestPlayer - 1].AddCard(cCard);
            }

            // Clear war cards vector
            m_vWarCards.clear();
        }
    }

    // Increment move counter after second player draws
    if (nPlayer == m_knPlayer2)
        ++m_nNumberOfMoves;

    // Capture move for network play
    m_vGameMoves.push_back(cGameMove);

    return true;
}

/**
  * Return a string providing a current score of the game.
  *
  * Count the cards for each player and the war chest.
  *
  * \return ""
  */

std::string CardGameWar::GameScore() const
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

bool CardGameWar::GameEnded(int nPlayer)
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
