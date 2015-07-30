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

#include "Hand.h"

// Initial static int to zero; used to generate unique identifier numbers for hands
int Hand::m_nHandCount {0};

void Hand::AddCard(Card cCard)
{
    m_vCards.push_back(cCard);
}

std::string Hand::DisplayCards() const
{
    std::string sCards {};

    for (const Card &cCard : m_vCards)
    {
        sCards.append(cCard.ShortName());
        sCards.append(" ");
    }

    return sCards;
}

int Hand::HasCards() const
{
    return m_vCards.size();
}




