/*
    Copyright 2015 Dom Maddalone

    This file is part of GameAI.

    GameAI is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GameAI is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GameAI.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Game.h"
#include "ConnectFour.h"
#include "TTT.h"
#include "Reversi.h"
#include "ChessGardner.h"
#include "ChessBaby.h"
#include "ChessJacobsMeirovitz.h"
#include "ChessPetty.h"
#include "ChessSpeed.h"
#include "ChessQuick.h"
#include "ChessElena.h"
#include "ChessAttack.h"
#include "ChessLosAlamos.h"
#include "Chess.h"

/**
  * Make a game.
  *
  * Based on the type of game, create the game and return a unique pointer to it.
  *
  * \param ecGameType The type of game.
  *
  * \return A unique pointer to a game object.  Default is nullptr.
  */

std::unique_ptr<Game> Game::Make(GameType ecGameType)
{
    switch (ecGameType)
    {
        case GameType::TYPE_CONNECT_FOUR:
            return std::unique_ptr<Game>(new ConnectFour(ecGameType));
        case GameType::TYPE_TTT:
            return std::unique_ptr<Game>(new TTT(ecGameType));
        case GameType::TYPE_REVERSI:
            return std::unique_ptr<Game>(new Reversi(ecGameType));
        case GameType::TYPE_CHESS_GARDNER:
            return std::unique_ptr<Game>(new ChessGardner(ecGameType));
        case GameType::TYPE_CHESS_BABY:
            return std::unique_ptr<Game>(new ChessBaby(ecGameType));
        case GameType::TYPE_CHESS_JACOBS_MEIROVITZ:
            return std::unique_ptr<Game>(new ChessJacobsMeirovitz(ecGameType));
        case GameType::TYPE_CHESS_PETTY:
            return std::unique_ptr<Game>(new ChessPetty(ecGameType));
        case GameType::TYPE_CHESS_SPEED:
            return std::unique_ptr<Game>(new ChessSpeed(ecGameType));
        case GameType::TYPE_CHESS_QUICK:
            return std::unique_ptr<Game>(new ChessQuick(ecGameType));
        case GameType::TYPE_CHESS_ELENA:
            return std::unique_ptr<Game>(new ChessElena(ecGameType));
        case GameType::TYPE_CHESS_ATTACK:
            return std::unique_ptr<Game>(new ChessAttack(ecGameType));
        case GameType::TYPE_CHESS_LOS_ALAMOS:
            return std::unique_ptr<Game>(new ChessLosAlamos(ecGameType));
        case GameType::TYPE_CHESS:
            return std::unique_ptr<Game>(new Chess(ecGameType));
        default:
            return nullptr;
    }
}

/**
  * Write moves to a file.
  *
  * \param sFileName Name of the output file
  *
  * \return True if successful, false otherwise.
  */

bool Game::WriteMoves(std::string sFileName)
{
    // Log the method entry
    std::string sMessage = "Writing moves to file " + sFileName;
    m_cLogger.LogInfo(sMessage,2);

    std::ofstream ofsFile(sFileName);
    if (!ofsFile)
    {
        std::cerr << "Could not open " << sFileName << std::endl;
        return false;
    }

    for (GameMove cGameMove : m_vGameMoves)
    {
        ofsFile << cGameMove.AnnounceFromMove() << cGameMove.AnnounceToMove() << std::endl;
    }

    ofsFile.close();

    return true;
}

/**
  * Read moves from a file.
  *
  * Read moves from file, generate a GameMove, and apply the move to the game.
  *
  * \param sFileName Name of the input file
  *
  * \return Number pf th eplayer to play next if successful, otherwise 0.
  */

int Game::ReadMoves(std::string sFileName)
{
    std::string sMove;
    GameMove cGameMove;
    int nPlayer {1};

    // Log the method entry
    std::string sMessage = "Reading moves from file " + sFileName;
    m_cLogger.LogInfo(sMessage,2);

    std::ifstream ifsFile(sFileName);
    if (!ifsFile)
    {
        std::cerr << "Could not open " << sFileName << std::endl;
        return 0;
    }

    int nMoveCounter = 0;

    while (getline(ifsFile, sMove))
    {
        cGameMove = GenerateMove(sMove);

        sMessage = "Read move " + std::to_string(++nMoveCounter) + ". " + sMove;
        m_cLogger.LogInfo(sMessage,3);

        if (!ApplyMove(nPlayer, cGameMove))
            return 0;
        nPlayer = 1 - nPlayer + 2;
    }

    ifsFile.close();

    return nPlayer;
}
