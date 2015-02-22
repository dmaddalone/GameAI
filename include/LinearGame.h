#ifndef LINEARGAME_H
#define LINEARGAME_H

#include <iostream>

#include "Game.h"

class LinearGame : public Game
{
    public:
        LinearGame(GameType ecGameType, int nX, int nY, char cClear, char cToken1, char cToken2, int nWin, bool bDisplayGrid, bool bDisplayXCoordinates, bool bDisplayYCoordinates) :
            Game(ecGameType),
            m_knX(nX),
            m_knY(nY),
            m_kcClear(cClear),
            m_kcToken1(cToken1),
            m_kcToken2(cToken2),
            m_knWin(nWin),
            m_kbDisplayGrid(bDisplayGrid),
            m_kbDisplayXCoordinates(bDisplayXCoordinates),
            m_kbDisplayYCoordinates(bDisplayYCoordinates)
            {  SetTokens(); ClearBoard(); }

        ~LinearGame() {}

        virtual void Display() const override;
        //virtual void DisplayValidMoves(int nPlayer) const override;
        virtual std::string ValidMoves(int nPlayer) const override;
        virtual GameMove GetMove(int nPlayer) const override;
        virtual int  PreferredMove(const GameMove &cGameMove) const override;
        virtual std::string GameScore() const override;
        virtual bool ApplyMove(int nPlayer, GameMove &cGameMove) override;
        //virtual void AnnounceMove(int nPlayer, const GameMove &cGameMove) override;
        virtual std::string AnnounceMove(int nPlayer, const GameMove &cGameMove) override;
        //virtual bool RetractMove(int nPlayer, const GameMove &cGameMove) override;
        virtual int  EvaluateGameState(int nPlayer) override;
        virtual bool GameEnded(int nPlayer) override;

        virtual std::unique_ptr<Game> Clone() const = 0;

    protected:
        void SetTokens() { m_acTokens[0] = m_kcClear; m_acTokens[1] = m_kcToken1; m_acTokens[2] = m_kcToken2; }
        void ClearBoard();
        void SetBoard() { return; };
        bool CheckOrthogonal(int nPlayer, int nConnect) ;//const;
        int  CheckHorizontal(int nPlayer, int y, int x) ;//const;
        int  CheckVertical(int nPlayer, int y, int x) ;//const;
        bool CheckDiagonal(int nPlayer, int nConnect) ;//const;
        int  CheckDiagonalUpperLeftLowerRight(int nPlayer, int y, int x) ;//const;
        int  CheckDiagonalUpperRightLowerLeft(int nPlayer, int y, int x) ;//const;
        bool ValidMove(int y, int x) const;

        const int  m_knX;
        const int  m_knY;
        const char m_kcClear;
        const char m_kcToken1;
        const char m_kcToken2;
        const int m_knWin;
        const bool m_kbDisplayGrid;
        const bool m_kbDisplayXCoordinates;
        const bool m_kbDisplayYCoordinates;
        const char m_kcXCoordinate {'a'};

        static const int m_knMaxX {8};
        static const int m_knMaxY {8};
        int m_acGrid[m_knMaxY][m_knMaxX] {{}};

        const char m_kDelimeter {','}; // TTT specific?

        static const int m_knTokens {3};
        char m_acTokens[m_knTokens];

        struct SequenceCounts {
            int nCount1 {0};
            int nCount2 {0};
            int nCount3 {0};
        } m_stMyCounts, m_stOpponentCounts;

        void CountSequence(int nSequence, SequenceCounts &stSequenceCounts);
};

#endif // LINEARGAME_H
