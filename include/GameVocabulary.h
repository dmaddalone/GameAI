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
 * \brief Provides game vocabulary.
 *
 */

#ifndef GAMEVOCABULARY_H
#define GAMEVOCABULARY_H

#include <string>
#include <vector>

namespace GameVocabulary
{
    static const std::string DELIMETER                      {" "};

    // Game Setup Commands
    static const std::string ESTABLISH_GAME                 {"ESTABLISH_GAME"};
    static const std::string REQUEST_CLIENT_PLAYER_NUMBER   {"REQUEST_CLIENT_PLAYER_NUMBER"};
    static const std::string ESTABLISH_CLIENT_PLAYER_NUMBER {"ESTABLISH_CLIENT_PLAYER_NUMBER"};
    static const std::string CONFIRM                        {"CONFIRM"};
    static const std::string UNCONFIRM                      {"UNCONFIRM"};
    static const std::string SYNC                           {"SYNC"};
    static const std::string END_SYNC                       {"END_SYNC"};
    static const std::string FATAL_EXIT                     {"FATAL_EXIT"};

    // Game Commands
    static const std::string DECLARE_WIN                    {"DECLARE_WIN"};
    static const std::string RESIGN                         {"RESIGN"};
    static const std::string MOVE                           {"MOVE"};
    static const std::string NO_MOVE                        {"NO_MOVE"};

    static const std::string ASK                            {"ASK"};
    //static const std::string ASK_SHORT                      {"A"};
    static const std::string FOLD                           {"FOLD"};
    //static const std::string FOLD_SHORT                     {"F"};
    static const std::string DRAW                           {"DRAW"};
    //static const std::string DRAW_SHORT                     {"DR"};
    static const std::string MELD                           {"MELD"};
    //static const std::string MELD_SHORT                     {"M"};
    static const std::string LAYOFF                         {"LAYOFF"};
    //static const std::string LAYOFF_SHORT                   {"L"};
    static const std::string DISCARD                        {"DISCARD"};
    //static const std::string DISCARD_SHORT                  {"DI"};

    static const std::string SHOW                           {"SHOW"};
    //static const std::string SHOW_SHORT                     {"SH"};
    static const std::string SCORE                          {"SCORE"};
    //static const std::string SCORE_SHORT                    {"SC"};

    // Game Arguments
    static const std::string ARG_STOCK                      {"STOCK"};
    //static const std::string ARG_STOCK_SHORT                {"ST"};
    static const std::string ARG_DISCARD                    {"DISCARD"};
    //static const std::string ARG_DISCARD_SHORT              {"DI"};

    // Return command (first token)
    std::string ParseCommand(const std::string &sText);

    // Return argument (second token)
    std::string ParseArgument(const std::string &sText);

    // Return arguments (tokens second..n)
    std::vector<std::string> ParseArguments(const std::string &sText);
};

#endif // GAMEVOCABULARY_H
