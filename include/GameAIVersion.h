/*
    Copyright 2014 Dom Maddalone

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

#ifndef GAMEAIVERSION_H
#define GAMEAIVERSION_H

#include <string>

namespace GameAIVersion
{
	//Date Version Types
	static const std::string DATE {"22"};
	static const std::string MONTH {"02"};
	static const std::string YEAR {"2015"};
	std::string DateVersion() {return (YEAR+MONTH+DATE); }

	//Software Status
	// Pre-alpha = The software is still under active development and not
	//   feature complete or ready for consumption by anyone other than
	//   software developers.
	// Alpha = The software is complete enough for internal testing.
	// Beta = The software is complete enough for external testing.
	// Release Candidate = The software is almost ready for final release.
	//   No feature development or enhancement of the software is undertaken;
	//   tightly scoped bug fixes are the only code you're allowed to write
	//   in this phase,
	// Release
	static const std::string STATUS {"Alpha"};

	//Semantic Versioning
	// MAJOR version when you make incompatible changes,
	// MINOR version when you add functionality in a backwards-compatible manner, and
	// PATCH version when you make backwards-compatible bug fixes.
	static const int MAJOR = 4;
	static const int MINOR = 0;
	static const int PATCH = 0;
	std::string SemanticVersion() { return (std::to_string(MAJOR) + "." + std::to_string(MINOR) + "." + std::to_string(PATCH)); }
};

#endif // GAMEAIVERSION_H
