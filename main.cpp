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

#include <cassert>
#include <cctype>
#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <memory>
#include <vector>

#include "GameAIException.h"
#include "GameAIVersion.h"
#include "Player.h"
#include "Game.h"

//using namespace std;

/**
  * ShowUsage
  *
  * Print command line arguments
  *
  * \param sName Name of the executing program.
  */

static void ShowUsage(std::string sName)
{
    std::cerr << "Usage: " << sName << " OPTIONS\n"
              << "Run a Game AI program\n\n"
              << "Required Options:\n"
              << "    -1 TYPE,  --player1=TYPE  assign TYPE of player to Player 1\n"
              << "    -2 TYPE,  --player2=TYPE  assign TYPE of player to Player 2\n"
              << "    -p PLIES, --plies=PLIES   assign the number of PLIES that non-human players will use\n"
              << "              --plies1=PLIES  assign the number of PLIES to Player 1, if non-human\n"
              << "              --plies2=PLIES  assign the number of PLIES to Player 2, if non-human\n"
              << "    -g GAME,  --game=GAME     play GAME"
              << "\nNon Required Options:\n"
              << "    -h,       --help          display this help message and exit\n"
              << "    -v LEVEL, --verbose=LEVEL display game information\n"
              << "    -V,       --version       display version and exit\n"
              << "\n"
              << "TYPE is either human or minimax\n"
              << "PLIES are from 1 to 9.  The default is 4.\n"
              << "GAME is ttt, connectfour, or reversi.\n"
              << "LEVEL is an integer 0 to 2.  The default is 1.\n"
              << "    0 = display start and ending announcements\n"
              << "    1 = display game move-by-move\n"
              << "    2 = display AI scoring of moves\n"
              << std::endl;
}

/**
  * ShowVersion
  *
  * Print version
  *
  */

static void ShowVersion()
{
    std::cerr << "Game AI version " << GameAIVersion::SemanticVersion() << " " << GameAIVersion::DateVersion() << std::endl;
}

static bool SetPlayerType(char* pcType, std::vector<std::unique_ptr<Player>> &vPlayers)
{
    std::string sType(pcType);
    if (sType == "human")
        vPlayers.emplace_back(Player::MakePlayer(PlayerType::TYPE_HUMAN));
    else if (sType == "minimax")
        vPlayers.emplace_back(Player::MakePlayer(PlayerType::TYPE_MINIMAX));
    else
        return false;

    return true;
}

static std::unique_ptr<Game> SetGame(char* pcGame)
{
    std::string sGame(pcGame);
    if (sGame == "connectfour")
        return Game::MakeGame(GameType::TYPE_CONNECT_FOUR);
    else if (sGame == "ttt")
        return Game::MakeGame(GameType::TYPE_TTT);
    else if (sGame == "reversi")
        return Game::MakeGame(GameType::TYPE_REVERSI);
    else
        return nullptr;
}

static void SetPlayers(std::string sName, int nPlies1, int nPlies2, int nVerbosity, std::vector<std::unique_ptr<Player>> &vPlayers)
{
    if (nVerbosity >= 0 && nVerbosity <= 2)
    {
        vPlayers[0]->SetVerbosity(nVerbosity);
        vPlayers[1]->SetVerbosity(nVerbosity);
    }
    else
    {
        ShowUsage(sName);
        exit (EXIT_FAILURE);
    }

    if (nPlies1 > 0 && nPlies1 <= 9)
    {
        vPlayers[0]->SetPlies(nPlies1);
    }
    else
    {
        ShowUsage(sName);
        exit (EXIT_FAILURE);
    }

    if (nPlies2 > 0 && nPlies2 <= 9)
    {
        vPlayers[1]->SetPlies(nPlies2);
    }
    else
    {
        ShowUsage(sName);
        exit (EXIT_FAILURE);
    }
}

/**
  * Main
  *
  * TODO
  *
  */

int main(int argc, char* argv[])
{
    std::vector<std::unique_ptr<Player>> vPlayers;
    std::unique_ptr<Game> upGame {};
    //int  nPlies     {4};
    int  nPlies1    {4};
    int  nPlies2    {4};
    int  nVerbosity {1};

    // Check for command line arguments
    if (argc < 2)
    {
        ShowUsage(argv[0]);
        exit (EXIT_FAILURE);
    }

    // Set up and execute getopt_long
    static struct option stLongOptions[] =
    {
        {"player1", required_argument, nullptr, '1'},
        {"player2", required_argument, nullptr, '2'},
        {"plies",   required_argument, nullptr, 'p'},
        {"plies1",  required_argument, nullptr, 'x'},
        {"plies2",  required_argument, nullptr, 'y'},
        {"game",    required_argument, nullptr, 'g'},
        {"help",    no_argument,       nullptr, 'h'},
        {"verbose", required_argument, nullptr, 'v'},
        {"version", no_argument,       nullptr, 'V'},
        {NULL,      0,                 nullptr,  0}
    };

    int nC = 0;
    int nOptionIndex = 0;
    while ((nC = getopt_long(argc, argv, "1:2:p:g:x:y:hv:V", stLongOptions, &nOptionIndex)) != -1)
    {
        switch (nC)
        {
            case 'h':
                ShowUsage(argv[0]);
                exit(EXIT_SUCCESS);
                break;
            case 'v':
                //bVerbose = true;
                if (optarg)
                {
                    if (isdigit(optarg[0]))
                    {
                        nVerbosity = atoi(optarg);
                    }
                    else
                    {
                        ShowUsage(argv[0]);
                        exit (EXIT_FAILURE);
                    }
                }
                break;
            case 'V':
                ShowVersion();
                exit(EXIT_SUCCESS);
                break;
            case '1':
                if (!SetPlayerType(optarg, vPlayers))
                {
                    std::cerr << "unknown player type " << optarg << std::endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case '2':
                if (!SetPlayerType(optarg, vPlayers))
                {
                    std::cerr << "unknown player type " << optarg << std::endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case 'p':
                //nPlies = atoi(optarg);
                nPlies1 = nPlies2 = atoi(optarg);
                //nPlies2 = nPlies;
                break;
            case 'x':
                nPlies1 = atoi(optarg);
                break;
            case 'y':
                nPlies2 = atoi(optarg);
                break;
            case 'g':
                upGame = SetGame(optarg);
                break;
            case '?':
                ShowUsage(argv[0]);
                exit(EXIT_SUCCESS);
                break;
            default:
                throw GameAIException("main getopt returned character code ", std::to_string(nC));
                break;
        }
    }

    if ((vPlayers.size() != 2) || (upGame == nullptr))
    {
        ShowUsage(argv[0]);
        exit(EXIT_FAILURE);
    }

    SetPlayers(argv[0], nPlies1, nPlies2, nVerbosity, vPlayers);

    std::cout << "Playing " << upGame->Title() << std::endl;

    for (int iii = 0; iii < 2; ++iii)
    {
        std::cout << "Player " << iii + 1 << ": " << vPlayers[iii]->TypeName();
        if (vPlayers[iii]->Type() != PlayerType::TYPE_HUMAN)
        {
            std::cout << " Plies: " << vPlayers[iii]->Plies();
        }
        std::cout << std::endl;
    }

    while(true)
    {
        if (!vPlayers[0]->Move(*upGame))
        {
            std::cerr << "Invalid move.  Exiting." << std::endl;
            exit(EXIT_FAILURE);
        }

        if (upGame->GameEnded())
            break;

        if (!vPlayers[1]->Move(*upGame))
        {
            std::cerr << "Invalid move.  Exiting." << std::endl;
            exit(EXIT_FAILURE);
        }

        if (upGame->GameEnded())
            break;
    }

    upGame->Display(true);

    if (upGame->Winner() == 0)
    {
        std::cout << "Game drawn" << std::endl;
    }
    else
    {
        std::cout << "Game won by Player " << upGame->Winner() << std::endl;
        std::cout << "Game won with " << upGame->WinBy() << std::endl;
    }

    std::cout << "Total number of moves: " << upGame->NumberOfMoves() << std::endl;
}
