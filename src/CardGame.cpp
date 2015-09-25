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

#include "CardGame.h"

/**
  * Display the cards.
  *
  * Display the cards in each player's hand.
  *
  */

void CardGame::Display() const
{
    for (const Hand &cHand : m_vHands)
    {
        std::cout << "P" << cHand.ID() << ": " << cHand.DisplayCards() << std::endl;
    }
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

bool CardGame::GetSyncInfo(std::string &sGameInformation)
{
    std::string sLogInfo {};
    sGameInformation.clear();

    if (m_bSyncDeck)
    {
        m_cLogger.LogInfo("Gathering synchronization on deck", 2);
        sLogInfo = "Deck Ranks: " + m_cDeck.Ranks();
        m_cLogger.LogInfo(sLogInfo, 3);
        sGameInformation = m_cDeck.JsonSerialization().toStyledString();
        m_bSyncDeck = false;
        return true;
    }
    else if (m_bSyncFirstHand)
    {
        m_cLogger.LogInfo("Gathering synchronization on first hand", 2);
        sLogInfo = "First Hand Ranks: " + m_vHands[0].Ranks();
        m_cLogger.LogInfo(sLogInfo, 3);
        sGameInformation = m_vHands[0].JsonSerialization().toStyledString();
        m_bSyncFirstHand = false;
        return true;
    }
    else if (m_bSyncSecondHand)
    {
        m_cLogger.LogInfo("Gathering synchronization on second hand", 2);
        sLogInfo = "Second Hand Ranks: " + m_vHands[1].Ranks();
        m_cLogger.LogInfo(sLogInfo, 3);
        sGameInformation = m_vHands[1].JsonSerialization().toStyledString();
        m_bSyncSecondHand = false;
        return true;
    }

    return false;
}

/**
  * Receive information to be synchronized from a networked opponent
  *
  * This function is a NOP and should be overridden in derived classes.
  *
  * \return True if information is available to be sent, false otherwise.
  */

bool CardGame::ApplySyncInfo(const std::string &sGameInformation, std::string &sErrorMessage)
{
    if (m_bSyncDeck)
    {
        m_cLogger.LogInfo("Applying synchronization on deck", 2);
        if (m_cDeck.JsonDeserialization(sGameInformation, sErrorMessage))
        {
            m_bSyncDeck = false;
            m_cLogger.LogInfo("Deck Ranks: ", 3);
            m_cLogger.LogInfo(m_cDeck.Ranks(), 3);
        }
        else
        {
            return false;
        }

    }
    else if (m_bSyncFirstHand)
    {
        m_cLogger.LogInfo("Applying synchronization on first hand", 2);
        if (m_vHands[0].JsonDeserialization(sGameInformation, sErrorMessage))
        {
            m_bSyncFirstHand = false;
            m_cLogger.LogInfo("First Hand Ranks: ", 3);
            m_cLogger.LogInfo(m_vHands[0].Ranks(), 3);
        }
        else
        {
            return false;
        }

    }
    else if (m_bSyncSecondHand)
    {
        m_cLogger.LogInfo("Applying synchronization on second hand", 2);
        if (m_vHands[1].JsonDeserialization(sGameInformation, sErrorMessage))
        {
            m_bSyncSecondHand = false;
            m_cLogger.LogInfo("Second Hand Ranks: ", 3);
            m_cLogger.LogInfo(m_vHands[1].Ranks(), 3);
        }
        else
        {
            return false;
        }
    }

    return true;
}

/**
  * Return a string of valid moves.
  *
  * This is NOP.  Dervied classes may override.
  *
  * \param nPlayer The player whose turn it is.
  *
  * \return A string of valid moves.
  */

std::string CardGame::ValidMoves(int nPlayer)
{
    (void) nPlayer;

    return "";
}

/**
  * Get the player's move.
  *
  * From std::cin, generate a GameMove object.  Provide a default move.
  *
  * \param nPlayer The player whose turn it is.
  *
  * \return A GameMove object.
  */

GameMove CardGame::GetMove(int nPlayer) const
{
    (void)nPlayer;

    static std::string sDefaultMove = DefaultMove();

    std::string sMove {};

    if (!sDefaultMove.empty())
    {
        std::cout << "[" << sDefaultMove << "] ";
        std::getline(std::cin, sMove);
        if (sMove.empty())
        {
            sMove = sDefaultMove;
        }
    }
    else
    {
        std::getline(std::cin, sMove); // (F)OLD, (D)RAW, (H)IT, (ST)AND, (DO)UBLE-DOWN, (SP)LIT, ...
    }

    return GenerateMove(sMove);;
}

/**
  * Return a GameMove object from a string representing a game move.
  *
  * Create a GameMove object from the passed string.  Check for a resignation.
  *
  * \param sMove The string representing a game move.
  *
  * \return A GameMove object.
  */

GameMove CardGame::GenerateMove(std::string sMove) const
{
    // Generic GamevMove
    GameMove cGameMove;

    // Turn off 'Move' for card games
    cGameMove.SetMove(false);

    // Upper case the move string
    std::locale loc;
    for (std::string::size_type iii = 0; iii < sMove.length(); ++iii)
        sMove[iii] = std::toupper(sMove[iii], loc);

    // Compare move against resignation
    if (sMove.compare(GameVocabulary::RESIGN) == 0)
    {
        cGameMove.SetResignation(true);
        return cGameMove;
    }

    // Compare move against folding
    if (FoldingAllowed())
    {
        if (sMove.compare(GameVocabulary::FOLD) == 0)
        {
            cGameMove.SetFold(true);
            return cGameMove;
        }
    }

    // Compare move against drawing
    if (DrawingAllowed())
    {
        if (sMove.compare(GameVocabulary::DRAW) == 0)
        {
            cGameMove.SetDraw(true);
            return cGameMove;
        }
    }

    // Compare move against asking for a card
    if (AskingAllowed())
    {
        // Evaluate against an ASK command
        std::string sToken = GameVocabulary::ParseCommand(sMove);
        if (sToken.compare(GameVocabulary::ASK) == 0)
        {
            // Grab argument, which should be card rank
            sToken = GameVocabulary::ParseArgument(sMove);

            // Create blank card
            Card cCard;

            // Evaluate for valid rank
            if (cCard.SetRank(sToken))
            {
                cGameMove.SetAsk(true);
                cGameMove.UpdateCard(cCard);
                return cGameMove;
            }
        }
    }

    // Compare move against showing
    if (ShowingAllowed())
    {
        if (sMove.compare(GameVocabulary::SHOW) == 0)
        {
            cGameMove.SetShow(true);
            return cGameMove;
        }
    }

    // Compare move against scoring
    if (ScoringAllowed())
    {
        if (sMove.compare(GameVocabulary::SCORE) == 0)
        {
            cGameMove.SetScore(true);
            return cGameMove;
        }
    }

    return cGameMove;
}

/**
  * Return a measure of the preference of a move.
  *
  * This function is a NOP and should be overridden in derived classes.
  *
  * \param cGameMove  The GameMove to be evaluated.
  *
  * \return 0
  */

int CardGame::PreferredMove(const GameMove &cGameMove) const
{
    (void)cGameMove;

    return 0;
}

/**
  * Announce game move.
  *
  * This function is a NOP and should be overridden in derived classes.
  *
  * \param nPlayer   The player whose turn it is.
  * \param cGameMove The player's move
  *
  * \return A string containing the move.
  */

std::string CardGame::AnnounceMove(int nPlayer, const GameMove &cGameMove) const
{
    (void) nPlayer;
    (void) cGameMove;

    return "";
}

/**
  * Evaluate the game state.
  *
  * From a player's perspective, return a value corresponding to the player's
  * standing in the game.  If the player has won the game, return a large,
  * positive integer.  If lost return a large negative integer.  Else return zero.
  *
  * \param nPlayer   The player whose turn it is.
  *
  * \return An integer representing game state for the player.
  */

int CardGame::EvaluateGameState(int nPlayer)
{
    // If won, return largest positive integer // TODO: make these constants
    if (m_nWinner == nPlayer)
        return INT_MAX;

    // If lost, return largest negative integer // TODO: make these constants
    if (m_nWinner == (3 - nPlayer))
        return INT_MIN;

    return 0;
}

/**
  * Return a  string providing a current score of the game.
  *
  * This function is a NOP and should be overridden in derived classes.
  *
  * \return ""
  */

std::string CardGame::GameScore() const
{
    return "";
}

/**
  * Check to see if a player has won the game.
  *
  * If the last move made was a resignation, return true.
  *
  * \param nPlayer The player
  *
  * \return True, if any player has won the game.  False otherwise.
  */

bool CardGame::GameEnded(int nPlayer)
{
    if (!m_vGameMoves.empty())
    {
        GameMove cGameMove = m_vGameMoves.back();
        if (cGameMove.Resignation())
        {
            m_nWinner = nPlayer;
            m_sWinBy.assign("resignation");
            m_bGameOver = true;
            return true;
        }
    }

    return false;
}
