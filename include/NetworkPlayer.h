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

#include "Game.h"
#include "GameVocabulary.h"
#include "GameAIException.h"
#include "Player.h"
#include "Socket.h"
#include "SocketException.h"

class NetworkPlayer: public Socket, public Player
{
    public:
        // Construct a client player
        NetworkPlayer(PlayerType ecPlayerType);

        // Destructor
        virtual ~NetworkPlayer() {}

        // Initializer
        virtual void Initialize(std::string sHost, int nPort, bool &bSwap) override { (void)sHost; (void)nPort; (void)bSwap; }

        // Generate the next game move
        virtual bool Move(Game &cGame) override;

        // Player actions at end of game
        virtual bool Finish(Game &cGame) override { return Move(cGame); }

        // Player sends last move to networked opponent
        bool SendLastMove(Game &cGame);

        // Player receives last move from networked opponent
        bool RecvLastMove(Game &cGame);

        // Send message
        void Send(const std::string &sMessage);

        // Player receives confirmation from networked opponent of last send
        void RecvConfirmation();

        // Send game information to be synchronized
        void Synchronize(Game &cGame);

        // Receives game information to be synchronized
        void RecvSyncInfo(Game &cGame);

        // Manage state of sending and receiving messages
        bool Sending()        { return m_bSetToSend; }
        bool Receiving()      { return !m_bSetToSend; }
        void SetToSending()   { m_bSetToSend = true; }
        void SetToReceiving() { m_bSetToSend = false; }

    private:
        // State of sending or reciving messages
        bool m_bSetToSend = false;
};

#endif // NETWORKPLAYER_H
