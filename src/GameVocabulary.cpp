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

#include "GameVocabulary.h"

/**
  * Parse the string for the command.
  *
  * Find and return the command portion of the string.
  *
  * \param sText The string to parse.
  *
  * \return A string containing the command.
  */

std::string GameVocabulary::ParseCommand(const std::string &sText)
{
    return sText.substr(0, sText.find(GameVocabulary::DELIMETER));
}

/**
  * Parse the string for the argument.
  *
  * Find and return the argument portion of the string.
  *
  * \param sText The string to parse.
  *
  * \return A string containing the argument.
  */

std::string GameVocabulary::ParseArgument(const std::string &sText)
{
    return sText.substr(sText.find(GameVocabulary::DELIMETER) + 1);
}

/**
  * Parse the string for arguments.
  *
  * Find and return the arguments within a string.
  *
  * \param sText The string to parse.
  *
  * \return A vector of strings.
  */

std::vector<std::string> GameVocabulary::ParseArguments(const std::string &sText)
{
    std::vector<std::string> vArgs {};
    size_t nStartPos = sText.find(GameVocabulary::DELIMETER);
    size_t nEndPos {};

    do
    {
        if (nStartPos != std::string::npos)
        {
            nEndPos = sText.find(GameVocabulary::DELIMETER, nStartPos + 1);
            if (nEndPos != std::string::npos)
            {
                vArgs.push_back(sText.substr(nStartPos + 1, nEndPos - nStartPos - 1));
            }
            else
            {
                vArgs.push_back(sText.substr(nStartPos + 1));
            }
            nStartPos = nEndPos;
        }
    } while (nStartPos != std::string::npos);

    return vArgs;
}
