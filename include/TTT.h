#ifndef TTT_H
#define TTT_H

#include <iostream>
#include "Game.h"
#include "LinearGame.h"

class TTT : public LinearGame
{
    public:
        TTT(GameType ecGameType) : LinearGame(ecGameType, 3, 3, ' ', 'X', 'O') {}
        ~TTT() {}

        virtual void Display(const bool bDisplayCoordinates = false) const override;
        virtual void DisplayValidMoves() const override;

        virtual GameMove GetMove() const override;
        virtual void AnnounceMove(const int nPlayer, const GameMove &cGameMove) override;

        virtual int  ApplyMove(const int nPlayer, GameMove &cGameMove) override;

        virtual bool RetractMove(const int nPlayer, const GameMove &cGameMove) override;

        virtual int  PreferredMove(const GameMove &cGameMove) const override;
        virtual bool GameEnded() override;

        virtual std::vector<GameMove> GenerateMoves() const override;

        virtual std::string Title() override { return "Tic-Tac-Toe"; }

    private:
        const int m_kWin {3};
};

#endif // TTT_H
