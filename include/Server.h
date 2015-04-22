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
 * \brief The Server class represents a server game player.
 *
 */

#ifndef SERVER_H
#define SERVER_H

#include "Player.h"
#include "Socket.h"
#include "SocketException.h"
#include "Game.h"
#include "GameMove.h"
#include "Logger.h"
#include "GameVocabulary.h"
#include "GameAIException.h"

class Server: private Socket, public Player
{
    public:
        // Construct a server player
        //Server(PlayerType ecPlayerType, int nPort) : Player(ecPlayerType) {};
        Server(PlayerType ecPlayerType, int nPort);

        // Destructor
        ~Server() {};

        // Initializer
        virtual void Initialize(bool &bSwap) override;

        void Accept(Server&);

        // Generate the next game move
        virtual bool Move(Game &cGame) override;

        // Announce the type pf player
        std::string TypeName() { return "Server"; }
};

#endif // SERVER_H
