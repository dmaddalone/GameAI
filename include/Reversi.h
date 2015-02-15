#ifndef REVERSI_H
#define REVERSI_H

#include <iostream>
#include "Game.h"
#include "LinearGame.h"

class Reversi : public LinearGame
{
    public:
        Reversi(GameType ecGameType) : LinearGame(ecGameType, 8, 8, ' ', 'D', 'L', 0, true, true, true) { SetBoard(); }
        ~Reversi() {}

        virtual bool ApplyMove(int nPlayer, GameMove &cGameMove) override;
        virtual int  EvaluateGameState(int nPlayer) override;
        virtual bool GameEnded(int nPlayer) override;
        virtual std::vector<GameMove> GenerateMoves(int nPlayer) const override;

        virtual std::string Title() override { return "Reversi"; }

    private:
        void SetBoard();

        bool Contiguous(int nX, int nY, int nPlayer) const;
        bool ContiguousUp(int nX, int nY, int nPlayer) const;
        bool ContiguousUpRight(int nX, int nY, int nPlayer) const;
        bool ContiguousRight(int nX, int nY, int nPlayer) const;
        bool ContiguousDownRight(int nX, int nY, int nPlayer) const;
        bool ContiguousDown(int nX, int nY, int nPlayer) const;
        bool ContiguousDownLeft(int nX, int nY, int nPlayer) const;
        bool ContiguousLeft(int nX, int nY, int nPlayer) const;
        bool ContiguousUpLeft(int nX, int nY, int nPlayer) const;
        bool CheckContiguous(int nX, int nY, int nPlayer, bool &bOpponentPieceAdjacent) const;

        void UpdateTable(int nPlayer, const GameMove &cGameMove);
        void FlipUp(int nX, int nY, int nPlayer);
        void FlipUpRight(int nX, int nY, int nPlayer);
        void FlipRight(int nX, int nY, int nPlayer);
        void FlipDownRight(int nX, int nY, int nPlayer);
        void FlipDown(int nX, int nY, int nPlayer);
        void FlipDownLeft(int nX, int nY, int nPlayer);
        void FlipLeft(int nX, int nY, int nPlayer);
        void FlipUpLeft(int nX, int nY, int nPlayer);

        int  Count(int nPlayer) const;
};

#endif // REVERSI_H
