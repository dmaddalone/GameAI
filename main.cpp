#include <cassert>
#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <memory>
#include <vector>

#include "GameAIException.h"
#include "GameAIVersion.h"
#include "Player.h"
//#include "Human.h"
#include "Game.h"

using namespace std;

/**
  * ShowUsage
  *
  * Print command line arguments
  *
  * \param sName Name of the executing program.
  */

static void ShowUsage(std::string sName)
{
    std::cerr << "Usage: " << sName << " [OPTIONS] [FILE]\n"
              << "Run a Game AI program\n\n"
              << "Required Options:\n"
              << "    -1 TYPE,  --player1=TYPE  assign TYPE of player to Player 1\n"
              << "    -2 TYPE,  --player2=TYPE  assign TYPE of player to Player 2\n"
              << "    -p PLIES, --plies=PLIES   assign the number of PLIES that non-human players will use\n"
              << "              --plies1=PLIES  assign the number of PLIES to Player 1\n"
              << "              --plies2=PLIES  assign the number of PLIES to Player 2\n"
              << "    -g GAME,  --game=GAME     play GAME"
              << "\nNon Required Options:\n"
              << "    -h,       --help          display this help message and exit\n"
              << "    -v        --verbose       display non-human potential moves scores"
              << "    -V,       --version       display version and exit\n"
              << "\n"
              << "TYPE is either human or minimax\n"
              << "PLIES are from 1 to 9.  The default is 6."
              << "GAME is either ttt or connectfour"
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

static bool SetPlayerType(char* pcType, vector<std::unique_ptr<Player>> &vPlayers)
{
    std::string sType(pcType);
    if (sType == "human")
        vPlayers.emplace_back(Player::MakePlayer(PlayerType::TYPE_HUMAN));
    else if (sType == "minimax")
        vPlayers.emplace_back(Player::MakePlayer(PlayerType::TYPE_MINIMAX));
    else if (sType == "alphabeta")
        vPlayers.emplace_back(Player::MakePlayer(PlayerType::TYPE_ALPHA_BETA));
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
    else
        return nullptr;
}

static void SetPlayers(int nPlies, int nPlies1, int nPlies2, bool bVerbose, vector<std::unique_ptr<Player>> &vPlayers)
{
    if (bVerbose)
    {
        vPlayers[0]->SetVerbose();
        vPlayers[1]->SetVerbose();
    }

    if (nPlies > 0 && nPlies < 9)
    {
        vPlayers[0]->SetPlies(nPlies);
        vPlayers[1]->SetPlies(nPlies);
    }

    if (nPlies1 > 0 && nPlies1 < 9)
    {
        vPlayers[0]->SetPlies(nPlies1);
    }

    if (nPlies2 > 0 && nPlies2 < 9)
    {
        vPlayers[1]->SetPlies(nPlies2);
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
    vector<std::unique_ptr<Player>> vPlayers;
    std::unique_ptr<Game> upGame {};
    int  nPlies   {-1};
    int  nPlies1  {-1};
    int  nPlies2  {-1};
    bool bVerbose {false};

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
        {"verbose", no_argument,       nullptr, 'v'},
        {"version", no_argument,       nullptr, 'V'},
        {NULL,      0,                 nullptr,  0}
    };

    int nC = 0;
    int nOptionIndex = 0;
    while ((nC = getopt_long(argc, argv, "1:2:p:g:x:y:hvV", stLongOptions, &nOptionIndex)) != -1)
    {
        switch (nC)
        {
            case 'h':
                ShowUsage(argv[0]);
                exit(EXIT_SUCCESS);
                break;
            case 'v':
                bVerbose = true;
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
                nPlies = atoi(optarg);
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

    SetPlayers(nPlies, nPlies1, nPlies2, bVerbose, vPlayers);

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
