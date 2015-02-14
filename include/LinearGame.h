#ifndef LINEARGAME_H
#define LINEARGAME_H

#include <iostream>

#include "Game.h"

class LinearGame : public Game
{
    public:
        LinearGame(GameType ecGameType, int nX, int nY, char cClear, char cToken1, char cToken2, bool bDisplayGrid) :
            Game(ecGameType),
            m_knX(nX),
            m_knY(nY),
            m_kcClear(cClear),
            m_kcToken1(cToken1),
            m_kcToken2(cToken2),
            m_kbDisplayGrid(bDisplayGrid)
            {  SetTokens(); ClearBoard(); }

        ~LinearGame() {}

        virtual void Display(const bool bDisplayCoordinates = false) const override;
        virtual void DisplayValidMoves() const override;
        virtual void AnnounceMove(const int nPlayer, const GameMove &cGameMove) override;
        virtual bool RetractMove(const int nPlayer, const GameMove &cGameMove) override;
        virtual int  EvaluateGameState(const int nPlayer) override;

    protected:
        void SetTokens() { m_acTokens[0] = m_kcClear; m_acTokens[1] = m_kcToken1; m_acTokens[2] = m_kcToken2; }
        void ClearBoard();
        void SetBoard() { return; };
        bool CheckOrthogonal(const int nPlayer, int nConnect) ;//const;
        int  CheckHorizontal(const int nPlayer, const int y, const int x) ;//const;
        int  CheckVertical(const int nPlayer, const int y, const int x) ;//const;
        bool CheckDiagonal(const int nPlayer, int nConnect) ;//const;
        int  CheckDiagonalUpperLeftLowerRight(const int nPlayer, const int y, const int x) ;//const;
        int  CheckDiagonalUpperRightLowerLeft(const int nPlayer, const int y, const int x) ;//const;
        bool ValidMove(const int y, const int x) const;

        const int  m_knX;
        const int  m_knY;
        const char m_kcClear;
        const char m_kcToken1;
        const char m_kcToken2;
        const bool m_kbDisplayGrid;

        static const int m_kMaxX {8};
        static const int m_kMaxY {8};
        int m_acGrid[m_kMaxY][m_kMaxX] {{}};

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
