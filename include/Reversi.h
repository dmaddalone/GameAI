#ifndef REVERSI_H
#define REVERSI_H

#include <iostream>
#include "Game.h"
#include "LinearGame.h"

class Reversi : public LinearGame
{
    public:
        Reversi(GameType ecGameType) : LinearGame(ecGameType, 8, 8, ' ', 'D', 'L', true) { SetBoard(); }
        ~Reversi() {}

        virtual GameMove GetMove() const override;

        virtual int  ApplyMove(const int nPlayer, GameMove &cGameMove) override;

        virtual int  PreferredMove(const GameMove &cGameMove) const override;
        virtual bool GameEnded() override;

        virtual std::vector<GameMove> GenerateMoves() const override;

        virtual std::string Title() override { return "Reversi"; }

    private:
        void SetBoard();
        const int m_kWin {0};
};

#endif // REVERSI_H
