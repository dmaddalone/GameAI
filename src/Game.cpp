#include "Game.h"
#include "ConnectFour.h"

std::unique_ptr<Game> Game::MakeGame(GameType ecGameType)
{
    switch (ecGameType)
    {
        case GameType::TYPE_CONNECT_FOUR:
            return std::unique_ptr<Game>(new ConnectFour(ecGameType));
        default:
            return nullptr;
    }
}
