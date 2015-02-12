#ifndef LINEARGAME_H
#define LINEARGAME_H

#include "Game.h"

class LinearGame : public Game
{
    public:
        LinearGame(GameType ecGameType) : Game(ecGameType) {  ClearBoard(); }
        ~LinearGame() {}

        int  EvaluateGameState(const int nPlayer) override;

    protected:
        void ClearBoard();
        bool CheckOrthogonal(const int nPlayer, int nConnect) ;//const;
        int  CheckHorizontal(const int nPlayer, const int y, const int x) ;//const;
        int  CheckVertical(const int nPlayer, const int y, const int x) ;//const;
        bool CheckDiagonal(const int nPlayer, int nConnect) ;//const;
        int  CheckDiagonalUpperLeftLowerRight(const int nPlayer, const int y, const int x) ;//const;
        int  CheckDiagonalUpperRightLowerLeft(const int nPlayer, const int y, const int x) ;//const;
        bool ValidMove(const int y, const int x) const;

        static const int m_kY = 3;           // TTT specific
        //static const int m_kY;// = 3;           // TTT specific
        //static int m_kY;// = 3;           // TTT specific
        static const int m_kX  = 3;           // TTT specific
        const char m_kClear = ' ';    // TTT specific?
        static const int m_kPlayer1 = 1;
        static const int m_kPlayer2 = 2;

        int m_acGrid[m_kY][m_kX] {{}};

        static const char m_kDelimeter {','}; // TTT specific?
        char m_kTokens[3] {m_kClear,'X','O'}; // TTT specific?

        struct SequenceCounts {
            int nCount1 {0};
            int nCount2 {0};
            int nCount3 {0};
        } m_stMyCounts, m_stOpponentCounts;

        void CountSequence(int nSequence, SequenceCounts &stSequenceCounts);
};

#endif // LINEARGAME_H
