#ifndef LINEARGAME_H
#define LINEARGAME_H

#include "Game.h"

class LinearGame : public Game
{
    public:
        LinearGame(GameType ecGameType, int nX, int nY, char cClear, char cToken1, char cToken2) :
            Game(ecGameType),
            m_kX(nX),
            m_kY(nY),
            m_kClear(cClear),
            m_kToken1(cToken1),
            m_kToken2(cToken2)
            {  SetTokens(); ClearBoard(); }

        ~LinearGame() {}

        virtual int  EvaluateGameState(const int nPlayer) override;

    protected:
        void SetTokens() { m_acTokens[0] = m_kClear; m_acTokens[1] = m_kToken1; m_acTokens[2] = m_kToken2; }
        void ClearBoard();
        bool CheckOrthogonal(const int nPlayer, int nConnect) ;//const;
        int  CheckHorizontal(const int nPlayer, const int y, const int x) ;//const;
        int  CheckVertical(const int nPlayer, const int y, const int x) ;//const;
        bool CheckDiagonal(const int nPlayer, int nConnect) ;//const;
        int  CheckDiagonalUpperLeftLowerRight(const int nPlayer, const int y, const int x) ;//const;
        int  CheckDiagonalUpperRightLowerLeft(const int nPlayer, const int y, const int x) ;//const;
        bool ValidMove(const int y, const int x) const;

        const int m_kX;
        const int m_kY;
        const char m_kClear;
        const char m_kToken1;
        const char m_kToken2;
        const int m_kPlayer1 = 1;
        const int m_kPlayer2 = 2;

        int m_acGrid[100][100] {{}};

        const char m_kDelimeter {','}; // TTT specific?

        char m_acTokens[3];

        struct SequenceCounts {
            int nCount1 {0};
            int nCount2 {0};
            int nCount3 {0};
        } m_stMyCounts, m_stOpponentCounts;

        void CountSequence(int nSequence, SequenceCounts &stSequenceCounts);
};

#endif // LINEARGAME_H
