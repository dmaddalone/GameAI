#ifndef MINIMAX_H
#define MINIMAX_H

#include <climits>
#include <iostream>
#include "Player.h"
#include "Game.h"

class Minimax: public Player
{
    public:
        Minimax(PlayerType ecPlayerType) : Player(ecPlayerType) {};
        ~Minimax() {};

        bool Move(Game &cGame);

        std::string TypeName() { return "Minimax"; }

    private:
        int MinimaxMove(int nPlayer, Game &cGame, int nDepth);
        int MinMove(int nPlayer, Game &cGame, int nDepth);
        int MaxMove(int nPlayer, Game &cGame, int nDepth);

        //int m_nDepth = 2;
};

#endif // MINIMAX_H
