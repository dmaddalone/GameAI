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
 * \brief Provides a custom exception class derived from std::runtime_error.
 *
 */

#ifndef SOCKETEXCEPTION_H
#define SOCKETEXCEPTION_H

#include <stdexcept>
#include <string>

class SocketException : public std::runtime_error
{
    public:
        explicit SocketException(const std::string &s) : std::runtime_error(s) {}
};

#endif // SOCKETEXCEPTION_H
