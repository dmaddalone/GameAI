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
        // Minimax(PlayerType ecPlayerType) : Player(ecPlayerType), m_acSpin{"\\|-/-"} {};
        ~Minimax() {};

        bool Move(Game &cGame);

        std::string TypeName() { return "Minimax"; }

    private:
        int MinimaxMove(int nPlayer, Game &cGame, int nDepth);
        //int MinMove(int nPlayer, Game &cGame, int nDepth);
        //int MaxMove(int nPlayer, Game &cGame, int nDepth);
        int MinMove(int nPlayer, Game &cGame, int nDepth, int nAlpha, int nBeta);
        int MaxMove(int nPlayer, Game &cGame, int nDepth, int nAlpha, int nBeta);

        //char m_acSpin[4] = {'|', '/', '--', '\\'};
        //char m_acSpin[] = "\\|-/-";
        //char m_acSpin[6];
        //int  m_nSpinIndex {0};
        //char Spin() { ++m_nSpinIndex; if (m_nSpinIndex > 5) m_nSpinIndex = 0; return m_acSpin[m_nSpinIndex]; }

        //int m_nDepth = 2;
};

#endif // MINIMAX_H
