#ifndef TTT_H
#define TTT_H

#include <iostream>
#include "Game.h"
#include "LinearGame.h"

class TTT : public LinearGame
{
    public:
        TTT(GameType ecGameType) : LinearGame(ecGameType) {}
        ~TTT() {}

        void Display(const bool bDisplayCoordinates = false) const override;
        void DisplayValidMoves() const override;

        GameMove GetMove() const override;
        void AnnounceMove(const int nPlayer, const GameMove &cGameMove) override;

        int  ApplyMove(const int nPlayer, GameMove &cGameMove) override;

        bool RetractMove(const int nPlayer, const GameMove &cGameMove) override;

        int  PreferredMove(const GameMove &cGameMove) const override;
        bool GameEnded() override;

        std::vector<GameMove> GenerateMoves() const override;

        std::string Title() override { return "Tic-Tac-Toe"; }
};

#endif // TTT_H
