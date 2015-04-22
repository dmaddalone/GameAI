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
 * \brief Provides game vocbulary between server and client.
 *
 */

#ifndef GAMEVOCABULARY_H
#define GAMEVOCABULARY_H

#include <string>

namespace GameVocabulary
{
    static const std::string DELIMETER                      {" "};

    static const std::string ESTABLISH_GAME                 {"ESTABLISH_GAME"};
    static const std::string REQUEST_CLIENT_PLAYER_NUMBER   {"REQUEST_CLIENT_PLAYER_NUMBER"};
    static const std::string ESTABLISH_CLIENT_PLAYER_NUMBER {"ESTABLISH_CLIENT_PLAYER_NUMBER"};
    static const std::string CONFIRM                        {"CONFIRM"};
    static const std::string UNCONFIRM                      {"UNCONFIRM"};

    static const std::string FATAL_EXIT                     {"FATAL_EXIT"};

    std::string ParseCommand(std::string sText);
    std::string ParseArgument(std::string sText);
};

#endif // GAMEVOCABULARY_H