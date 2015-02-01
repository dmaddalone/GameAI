#ifndef TTT_H
#define TTT_H

#include <iostream>
#include "Game.h"

class TTT : public Game
{
    public:
        TTT(GameType ecGameType) : Game(ecGameType) {}
        ~TTT() {}

        void Display(const bool bDisplayLines = false) const;
        void DisplayValidMoves() const;
        //virtual void Reset() = 0;
        int  ApplyMove(const int nPlayer, const int nMove);
        bool RetractMove(const int nPlayer, const int nMove);
        int  PreferredMove(const int nMove) const;
        bool GameEnded();

        std::vector<int> GenerateMoves() const;
        int  EvaluateGameState(const int nPlayer);

        std::string Title() { return "Tic-Tac-Toe"; }

    private:

        bool CheckOrthogonal(const int nPlayer, int nConnect) ;//const;
        int  CheckHorizontal(const int nPlayer, const int y, const int x) ;//const;
        int  CheckVertical(const int nPlayer, const int y, const int x) ;//const;
        bool CheckDiagonal(const int nPlayer, int nConnect) ;//const;
        int  CheckDiagonalUpperLeftLowerRight(const int nPlayer, const int y, const int x) ;//const;
        int  CheckDiagonalUpperRightLowerLeft(const int nPlayer, const int y, const int x) ;//const;
        bool ValidMove(const int y, const int x) const;


        static const int m_kY = 3;
        static const int m_kX = 3;
        static const int m_kClear = 0;
        static const int m_kPlayer1 = 1;
        static const int m_kPlayer2 = 2;

        int m_anGrid[m_kY][m_kX] {{m_kClear}};

        struct SequenceCounts {
            int nCount1 {0};
            int nCount2 {0};
            int nCount3 {0};
        } m_stMyCounts, m_stOpponentCounts;

        void CountSequence(int nSequence, SequenceCounts &stSequenceCounts);
};

#endif // TTT_H
