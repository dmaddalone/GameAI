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
 * \brief The Blackboard class represents a knowledge base of game information
 * for an AIPlayer.
 *
 */

#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include "Card.h"
#include "Deck.h"
#include "Hand.h"

class Blackboard
{
    public:
        Blackboard() {};
        virtual ~Blackboard() {};

        // Manage Initialization
        bool Initialized()                { return m_bInitialized; }
        void SetInitialized(bool b=true)  { m_bInitialized = b; }

        // Manage the number of asks
        void UpdateAsks(const std::string &sRank);
        int  Asks(const std::string &sRank) const;

        // Manage Layoff and Match Opportunities
        void SetLayoffOpportunities(int n)    { m_nLayoffOpportunities = n; }
        int  LayoffOpportunities() const      { return m_nLayoffOpportunities; }
        void SetMatchOpportunities(int n)     { m_nMatchOpportunities = n; }
        int  MatchOpportunities() const       { return m_nMatchOpportunities; }
        void SetNearMatchOpportunities(int n) { m_nNearMatchOpportunities = n; }
        int  NearMatchOpportunities() const   { return m_nNearMatchOpportunities; }

        // Manage Card
        void SetCard(const Card &cCard)    { m_cCard = cCard; }
        Card GetCard() const               { return m_cCard; }

        //
        // Public members
        //
        ProbableDeck m_cProbableDeck;
        ProbableHand m_cProbableOpponentHand;

    private:
        // Initialization flag
        bool m_bInitialized {false};

        // Manage asks
        std::map<std::string, int> m_mAsks {};

        // Layoff and Match Opportunities
        int m_nLayoffOpportunities {};
        int m_nMatchOpportunities {};
        int m_nNearMatchOpportunities {};

        // Card
        Card m_cCard;
};

#endif // BLACKBOARD_H
