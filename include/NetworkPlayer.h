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

#ifndef NETWORKPLAYER_H
#define NETWORKPLAYER_H

/** \file
 *
 * \brief The NetworkPlayer class represents a game player who plays over a network.
 *
 */

#include <string>

#include "Player.h"
#include "Socket.h"
#include "SocketException.h"
#include "Game.h"
#include "GameMove.h"
//#include "Logger.h"
#include "GameVocabulary.h"
#include "GameAIException.h"

class NetworkPlayer: public Socket, public Player
{
    public:
        // Construct a client player
        NetworkPlayer(PlayerType ecPlayerType);
        //NetworkPlayer() {}

        // Destructor
        virtual ~NetworkPlayer() {}

        // Initializer
        virtual void Initialize(bool &bSwap) = 0;

        // Generate the next game move
        virtual bool Move(Game &cGame) override;

        // Player sends last move to networked player
        bool SendLastMove(Game &cGame);

        // Player receives last move from networked player
        bool RecvLastMove(Game &cGame);

        //// Announce the type pf player
        //std::string TypeName() { return "Client"; }

        bool Sending()        { return m_bSetToSend; }
        bool Receiving()      { return !m_bSetToSend; }
        void SetToSending()   { m_bSetToSend = true; }
        void SetToReceiving() { m_bSetToSend = false; }

    private:
        bool m_bSetToSend = false;
};

#endif // NETWORKPLAYER_H
