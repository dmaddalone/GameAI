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
  * Return information to be synchronized between networked players.
  *
  * This function is a NOP and should be overridden in derived classes.
  *
  * \param sGameInformation String representing game information to be synchronized
  * between players.
  *
  * \return True if information received to synchronize, false otherwise.
  */

bool CardGameWar::GetSyncInfo(std::string &sGameInformation)
{
    std::string sLogInfo {};
    sGameInformation.clear();

    if (m_bSyncBattle)
    {
        m_cLogger.LogInfo("Gathering synchronization on battle deck", 2);
        sLogInfo = "Battle Ranks: " + BattleRanks();
        //m_cLogger.LogInfo("Battle Ranks:", 3);
        //m_cLogger.LogInfo(BattleRanks(), 3);
        m_cLogger.LogInfo(sLogInfo, 3);
        sGameInformation = BattleJsonSerialization().toStyledString();
        m_bSyncBattle = false;
        return true;
    }
    else if (m_bSyncWarCards)
    {
        m_cLogger.LogInfo("Gathering synchronization on war cards", 2);
        sLogInfo = "War Card Ranks: " + WarCardsRanks();
        //m_cLogger.LogInfo("War Card Ranks:", 3);
        //m_cLogger.LogInfo(WarCardsRanks(), 3);
        m_cLogger.LogInfo(sLogInfo, 3);
        sGameInformation = WarCardsJsonSerialization().toStyledString();
        m_bSyncWarCards = false;
        return true;
    }
    else if (m_bSyncWar)
    {
        m_cLogger.LogInfo("Gathering synchronization on war flag", 2);
        //std::string sLogInfo = "War Flag:" + std::string(m_bWar ? "true" : "false");
        sLogInfo = "War Flag:" + std::string(m_bWar ? "true" : "false");
        m_cLogger.LogInfo(sLogInfo, 3);
        Json::Value jValue;
        jValue["War"] = m_bWar;
        sGameInformation = jValue.toStyledString();
        m_bSyncWar = false;
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

bool CardGameWar::ApplySyncInfo(const std::string &sGameInformation, std::string &sErrorMessage)
{
    std::string sLogInfo {};

    if (m_bSyncBattle)
    {
        m_cLogger.LogInfo("Applying synchronization on battle deck", 2);
        if (BattleJsonDeserialization(sGameInformation, sErrorMessage))
        {
            m_bSyncBattle = false;
            sLogInfo = "Battle Ranks: " + BattleRanks();
            //m_cLogger.LogInfo("Battle Ranks:", 3);
            //m_cLogger.LogInfo(BattleRanks(), 3);
            m_cLogger.LogInfo(sLogInfo, 3);
            return true;
        }
        else
        {
            return false;
        }

    }
    else if (m_bSyncWarCards)
    {
        m_cLogger.LogInfo("Applying synchronization on war cards", 2);
        if (WarCardsJsonDeserialization(sGameInformation, sErrorMessage))
        {
            m_bSyncWarCards = false;
            sLogInfo = "War Card Ranks: " + WarCardsRanks();
            //m_cLogger.LogInfo("War Card Ranks:", 3);
            //m_cLogger.LogInfo(WarCardsRanks(), 3);
            m_cLogger.LogInfo(sLogInfo, 3);
            return true;
        }
        else
        {
            return false;
        }

    }
    else if (m_bSyncWar)
    {
        m_cLogger.LogInfo("Applying synchronization on war flag", 2);
        Json::Reader jReader;
        Json::Value jValue;
        if (jReader.parse(sGameInformation, jValue, false))
        {
            m_bWar = jValue["War"].asBool();
            m_bSyncWar = false;
            //std::string sLogInfo = "War Flag:" + std::string(m_bWar ? "true" : "false");
            sLogInfo = "War Flag:" + std::string(m_bWar ? "true" : "false");
            m_cLogger.LogInfo(sLogInfo, 3);
            return true;
        }
        else
        {
            sErrorMessage = jReader.getFormattedErrorMessages();
            return false;
        }
    }

    return CardGame::ApplySyncInfo(sGameInformation, sErrorMessage);
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

    // Draw card for battle
    Card cCard = m_vHands[nPlayer - 1].DrawTopCard();
    cCard.TurnUp(true);

    // Add card to GameMove
    cGameMove.UpdateCard(cCard);

    sMessage = "Player " + std::to_string(nPlayer) + " draws " + cCard.DisplayRank();
    m_cLogger.LogInfo(sMessage,1);

    // Insert players's up card into battle
    bool bInserted = m_uomBattle.insert(std::make_pair(nPlayer, cCard)).second;

    // If not inserted successfully, return the card to hand
    if (!bInserted)
    {
        // Since DrawTopCard is destructive, add the card back
        cCard.TurnUp(false);
        m_vHands[nPlayer - 1].AddCardToTop(cCard);

        // Replace the card in th GameMove with a blank card
        Card cBlankCard;
        cGameMove.UpdateCard(cBlankCard);

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
            sMessage = "Player " + std::to_string(nBestPlayer) + " Wins " + std::to_string(m_vWarCards.size()) + " cards.";
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

std::string CardGameWar::BattleRanks() const
{
    std::string sRanks {};
    Card cCard;

    for (const auto &PlayerCard : m_uomBattle)
    {
        cCard = PlayerCard.second;
        sRanks += cCard.Rank() + " ";
    }

    if (sRanks.empty())
        sRanks = "empty";

    return sRanks;
}

Json::Value CardGameWar::BattleJsonSerialization() const
{
    Json::Value jValue;
    Json::Value jBattle;
    Card cCard;

    for (const auto &PlayerCard : m_uomBattle)
    {
        jValue["Player"] = PlayerCard.first;
        cCard            = PlayerCard.second;
        jValue["Card"]   = cCard.JsonSerialization();
        jBattle[std::to_string(cCard.ID())] = jValue;
    }

    return jBattle;
}

bool CardGameWar::BattleJsonDeserialization(const std::string &sJsonBattle, std::string &sErrorMessage)
{
    Json::Reader jReader;
    Json::Value jBattle;
    int nPlayer;
    Card cCard;

    if (jReader.parse(sJsonBattle, jBattle, false))
    {
        m_uomBattle.clear();

        for (const Json::Value &jValue : jBattle)
        {
            nPlayer = jValue["Player"].asInt();

            if (cCard.JsonDeserialization(jValue["Card"].toStyledString(), sErrorMessage))
            {
                // Insert players's up card into battle
                bool bInserted = m_uomBattle.insert(std::make_pair(nPlayer, cCard)).second;

                // If not inserted successfully, return the card to hand
                if (!bInserted)
                {
                    sErrorMessage = "Could not insert pair <Player, Card> into Battle";
                    return false;
                }
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

std::string CardGameWar::WarCardsRanks() const
{
    std::string sRanks {};

    for (const Card &cCard : m_vWarCards)
    {
        sRanks += cCard.Rank() + " ";
    }

    if (sRanks.empty())
        sRanks = "empty";

    return sRanks;
}

Json::Value CardGameWar::WarCardsJsonSerialization() const
{
    Json::Value jValue;
    Json::Value jWarCards;
    Card cCard;

    for (const Card &cCard : m_vWarCards)
    {
        jValue = cCard.JsonSerialization();
        jWarCards[std::to_string(cCard.ID())] = jValue;
    }

    return jWarCards;
}

bool CardGameWar::WarCardsJsonDeserialization(const std::string &sJsonWarCards, std::string &sErrorMessage)
{
    Json::Reader jReader;
    Json::Value  jWarCards;
    Card cCard;

    if (jReader.parse(sJsonWarCards, jWarCards, false))
    {
        m_vWarCards.clear();

        for (const Json::Value &jCard : jWarCards)
        {
            if (cCard.JsonDeserialization(jCard.toStyledString(), sErrorMessage))
            {
                m_vWarCards.push_back(cCard);
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
