#ifndef PLAYER_H
#define PLAYER_H

#include <memory>

#include "Game.h"

enum class PlayerType
{
    TYPE_NONE,
    TYPE_HUMAN,
    TYPE_MINIMAX,
    TYPE_ALPHA_BETA
};

class Player
{
    public:
        Player(PlayerType ecPlayerType) { m_ecPlayerType = ecPlayerType; m_nPlayerNumber = m_nPlayerCount; }
        virtual ~Player() {};

        static std::unique_ptr<Player> MakePlayer(PlayerType ecPlayerType);

        virtual bool Move(Game &upGame) = 0;

        virtual std::string TypeName()  = 0;
        PlayerType Type() { return m_ecPlayerType; }

        void SetPlies(int nPlies)       { m_nDepth = nPlies; }

    protected:
        int m_nMove               {0};
        PlayerType m_ecPlayerType {PlayerType::TYPE_NONE};
        int m_nPlayerNumber       {0};
        int m_nDepth              {6};  // For AI

    private:
        static int m_nPlayerCount;
};

#endif // PLAYER_H
