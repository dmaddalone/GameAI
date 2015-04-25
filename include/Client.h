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

#ifndef CLIENT_H
#define CLIENT_H

/** \file
 *
 * \brief The Client class represents a client game player.
 *
 */

#include "NetworkPlayer.h"

class Client: public NetworkPlayer
{
    public:
        // Construct a client player
        Client(PlayerType ecPlayerType);

        // Destructor
        ~Client() {}

        // Initializer
        //virtual void Initialize(bool &bSwap) override;
        virtual void Initialize(std::string sHost, int nPort, bool &bSwap) override;

        // Announce the type pf player
        std::string TypeName() { return "Client"; }
};

#endif // CLIENT_H
