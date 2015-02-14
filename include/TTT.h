#ifndef TTT_H
#define TTT_H

#include <iostream>
#include "Game.h"
#include "LinearGame.h"

class TTT : public LinearGame
{
    public:
        TTT(GameType ecGameType) : LinearGame(ecGameType, 3, 3, ' ', 'X', 'O', true) {}
        ~TTT() {}

        virtual GameMove GetMove(int nPlayer, int nOpponent) const override;

        virtual int  ApplyMove(int nPlayer, GameMove &cGameMove) override;

        virtual int  PreferredMove(const GameMove &cGameMove) const override;
        virtual bool GameEnded(int nPlayer, int nOpponent) override;

        virtual std::vector<GameMove> GenerateMoves(int nPlayer, int nOpponent) const override;

        virtual std::string Title() override { return "Tic-Tac-Toe"; }

    private:
        const int m_kWin {3};
};

#endif // TTT_H
