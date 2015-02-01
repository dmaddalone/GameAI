#ifndef GAME_H
#define GAME_H

#include <memory>
#include <vector>

#include "Move.h"

enum class GameType
{
    TYPE_NONE,
    TYPE_CONNECT_FOUR,
    TYPE_TTT
};

class Game
{
    public:
        Game(GameType ecGameType) { m_ecGameType = ecGameType; }
        virtual ~Game() {};

        virtual void Display(const bool bDisplayCoordinates = false) const  = 0;
        virtual void DisplayValidMoves() const = 0;;
        //virtual void Reset() = 0;
        virtual int  ApplyMove(const int nPlayer, const int nMoveX, const int nMoveY) = 0; //YES
        virtual bool RetractMove(const int nPlayer, const int nMove) = 0; //YES
        virtual int  PreferredMove(const int nMove) const = 0;
        virtual bool GameEnded() = 0; //YES
        //virtual bool ValidMove(const int y, const int x) = 0;
        virtual std::vector<int> GenerateMoves() const = 0; //YES
        virtual int  EvaluateGameState(const int nPlayer) = 0; //YES

        static std::unique_ptr<Game> MakeGame(GameType ecGameType);

        virtual std::string Title() = 0;
        int  Winner() { return m_nWinner; };
        int  NumberOfMoves() { return m_nNumberOfMoves; }
        std::string WinBy() { return m_sWinBy; }

    protected:
        GameType m_ecGameType {GameType::TYPE_NONE};
        int m_nWinner {0};
        int m_nNumberOfMoves {0};
        std::string m_sWinBy {};
        //int m_nWinBy {0};
};

#endif // GAME_H
