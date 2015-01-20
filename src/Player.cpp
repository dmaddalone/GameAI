#include "Player.h"
#include "Human.h"
#include "Minimax.h"

int Player::m_nPlayerCount {0};

std::unique_ptr<Player> Player::MakePlayer(PlayerType ecPlayerType)
{
    ++m_nPlayerCount;

    switch (ecPlayerType)
    {
        case PlayerType::TYPE_HUMAN:
            return std::unique_ptr<Player>(new Human(ecPlayerType));
        case PlayerType::TYPE_MINIMAX:
            return std::unique_ptr<Player>(new Minimax(ecPlayerType));
        default:
            return nullptr;
    }
}
