#ifndef ALPHABETA_H
#define ALPHABETA_H

#include <climits>
#include <iostream>
#include "Player.h"
#include "Game.h"

class AlphaBeta: public Player
{
    public:
        AlphaBeta(PlayerType ecPlayerType) : Player(ecPlayerType) {};
        ~AlphaBeta() {};

        bool Move(Game &cGame);

        std::string TypeName() { return "AlphaBeta"; }

    private:
        int AlphaBetaMove(int nPlayer, Game &cGame, int nDepth);
        int AlphaBetaMinMove(int nPlayer, Game &cGame, int nDepth, int nAlpha, int nBeta);
        int AlphaBetaMaxMove(int nPlayer, Game &cGame, int nDepth, int nAlpha, int nBeta);

        //int m_nDepth = 6;
};
#endif // ALPHABETA_H
