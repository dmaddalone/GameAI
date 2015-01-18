#ifndef CONNECTFOUR_H
#define CONNECTFOUR_H

#include <iostream>
#include "Game.h"

class ConnectFour : public Game
{
    public:
        ConnectFour(GameType ecGameType) : Game(ecGameType) {}
        ~ConnectFour() {}

        void Display(const bool bDisplayCoordinates = false) const;
        void DisplayValidMoves() const;
        //virtual void Reset() = 0;
        int  ApplyMove(const int nPlayer, const int nMove);
        bool RetractMove(const int nPlayer, const int nMove);
        int  PreferredMove(const int nMove) const;
        bool GameEnded();

        std::vector<int> GenerateMoves() const;
        int  EvaluateGameState(const int nPlayer) const;

        std::string Title() { return "ConnectFour"; }

    private:
        int  FindBottom(const int x) const;
        bool CheckOrthogonal(const int nPlayer) ;//const;
        int  CheckHorizontal(const int nPlayer, const int y, const int x) ;//const;
        int  CheckVertical(const int nPlayer, const int y, const int x) ;//const;
        bool CheckDiagonal(const int nPlayer) ;//const;
        int  CheckDiagonalUpperLeftLowerRight(const int nPlayer, const int y, const int x) ;//const;
        int  CheckDiagonalUpperRightLowerLeft(const int nPlayer, const int y, const int x) ;//const;
        bool ValidMove(const int y, const int x) const;

        static const int m_kY = 6;
        static const int m_kX = 7;
        static const int m_kClear = 0;
        static const int m_kPlayer1 = 1;
        static const int m_kPlayer2 = 2;

        int m_anGrid[m_kY][m_kX] {{m_kClear}};
};

#endif // CONNECTFOUR_H
