#ifndef TTT_H
#define TTT_H

#include <iostream>
#include "Game.h"
#include "LinearGame.h"

class TTT : public LinearGame
{
    public:
        TTT(GameType ecGameType) : LinearGame(ecGameType, 3, 3, ' ', 'X', 'O', 3, true, true, true) {}
        ~TTT() {}

        virtual std::vector<GameMove> GenerateMoves(int nPlayer) const override;

        virtual std::unique_ptr<Game> Clone() const override { return std::unique_ptr<Game>(new TTT(*this)); }

        virtual std::string Title() override { return "Tic-Tac-Toe"; }

    private:
        //const int m_kWin {3};
};

#endif // TTT_H
