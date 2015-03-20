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

/**
  * Clear the board.
  *
  * Run through every space on the board and assign a clear token to it.
  */

#include "LinearGame.h"

void LinearGame::ClearBoard()
{
    for (int xxx = 0; xxx < m_knX; ++xxx)
    {
        for (int yyy = 0; yyy < m_knY; ++yyy)
        {
            m_acGrid[yyy][xxx] = m_kcClear;
        }
    }

    SetBoard();
}

/**
  * Display the game board.
  *
  * Run through every space on the board and display it's token.  Also display
  * coordinates and grid lines, depending on settings.
  */

void LinearGame::Display() const
{
    if (m_kbDisplayXCoordinates)
    {
        char c = m_kcXCoordinate;

        std::cout << "   ";
        for (int xxx = 0; xxx < m_knX; ++xxx)

            std::cout << c << "   ";
            ++c;
        }
        std::cout << std::endl << std::endl;;
    }

    for (int yyy = 0; yyy < m_knY; ++yyy)
    {
        if (m_kbDisplayYCoordinates)
        {
            std::cout << yyy + 1 << "  ";
        }
        else
        {
            std::cout << "   ";
        }

        for (int xxx = 0; xxx < m_knX; ++xxx)
        {
            if (m_kbDisplayGrid && xxx > 0)
            {
                std::cout << " | ";
            }

            std::cout << static_cast<char>(m_acGrid[yyy][xxx]);

            if (!m_kbDisplayGrid)
            {
                std::cout << "   ";
            }
        }

        if (m_kbDisplayGrid && yyy < m_knY - 1)
        {
            std::string sHyphen("  ");
            sHyphen.insert(sHyphen.length() - 1, 4 * m_knX, '-');
            std::cout << std::endl << sHyphen << std::endl;
        }
        else
            std::cout << std::endl << std::endl;
    }
}

/**
  * Return a string of valid moves.
  *
  * Call GenerateMoves and collect information into a string.
  *
  * \param nPlayer The player whose turn it is.
  *
  * \return A string of valid moves.
  */

std::string LinearGame::ValidMoves(int nPlayer) const
{
    std::string sValidMoves {};

    std::vector<GameMove> vGameMoves = GenerateMoves(nPlayer);
    for (GameMove cGameMove : vGameMoves)
    {
        sValidMoves += cGameMove.AnnounceToMove() + " ";
    }

    return sValidMoves;
}

GameMove LinearGame::GetMove(int nPlayer) const
{
    (void)nPlayer;

    std::string sMove {};
    GameMove cGameMove;

    std::cin >> sMove;

    int xxx = tolower(sMove[0]) - m_kcXCoordinate;

    sMove.erase(0,1);
    int yyy = std::stoi(sMove, nullptr);

    cGameMove.SetToX(xxx);
    cGameMove.SetToY(yyy - 1);

    return cGameMove;
}

bool LinearGame::ApplyMove(int nPlayer, GameMove &cGameMove)
{
    if ((nPlayer != m_knPlayer1) && (nPlayer != m_knPlayer2))
        return false;

    if ((cGameMove.ToX() > m_knX - 1) || (cGameMove.ToX() < 0))
        return false;


    if ((cGameMove.ToY() > m_knY - 1) || (cGameMove.ToY() < 0))
        return false;

    if (m_acGrid[cGameMove.ToY()][cGameMove.ToX()] != m_kcClear)
        return false;

    m_acGrid[cGameMove.ToY()][cGameMove.ToX()] = m_acTokens[nPlayer];
    ++m_nNumberOfMoves;

    return true;
}

std::string LinearGame::AnnounceMove(int nPlayer, const GameMove &cGameMove)
{
    std::string sMessage = "Move number: " + std::to_string(m_nNumberOfMoves + 1) + "  Player: " + std::to_string(nPlayer) + "  Moves To: " + cGameMove.AnnounceToMove();

    return sMessage;
}

/*
bool LinearGame::RetractMove(int nPlayer, const GameMove &cGameMove)
{
    (void)nPlayer;

    m_acGrid[cGameMove.ToY()][cGameMove.ToX()] = m_kcClear;
    --m_nNumberOfMoves;

    return true;
}
*/

int LinearGame::EvaluateGameState(int nPlayer)
{
    if (m_nWinner == nPlayer)
        return 1000000;

    if (m_nWinner == (1 - nPlayer + 2))
        return -1000000;

    m_stMyCounts = {};
    m_stOpponentCounts = {};

    if (m_nWinner == 0)
    {
        for (int yyy = 0; yyy < m_knY; ++yyy)
        {
            for (int xxx = 0; xxx < m_knX; ++xxx)
            {
                CountSequence(CheckHorizontal(nPlayer, yyy, xxx), m_stMyCounts);
                CountSequence(CheckHorizontal(1 - nPlayer + 2, yyy, xxx), m_stOpponentCounts);
                CountSequence(CheckVertical(nPlayer, yyy, xxx), m_stMyCounts);
                CountSequence(CheckVertical(1 - nPlayer + 2, yyy, xxx), m_stOpponentCounts);
                CountSequence(CheckDiagonalUpperLeftLowerRight(nPlayer, yyy, xxx), m_stMyCounts);
                CountSequence(CheckDiagonalUpperLeftLowerRight(1 - nPlayer + 2, yyy, xxx), m_stOpponentCounts);
                CountSequence(CheckDiagonalUpperRightLowerLeft(nPlayer, yyy, xxx), m_stMyCounts);
                CountSequence(CheckDiagonalUpperRightLowerLeft(1 - nPlayer + 2, yyy, xxx), m_stOpponentCounts);
            }
        }

        return ( (m_stMyCounts.nCount2 * 10) - (m_stOpponentCounts.nCount2 * 100) +
                 (m_stMyCounts.nCount3 * 1000) - (m_stOpponentCounts.nCount3 * 10000) );
    }

    return 0;
}

void LinearGame::CountSequence(int nSequence, SequenceCounts &stSequenceCounts)
{
    switch (nSequence)
    {
        case 0:
            break;
        case 1:
            ++stSequenceCounts.nCount1;
            break;
        case 2:
            ++stSequenceCounts.nCount2;
            break;
        case 3:
            ++stSequenceCounts.nCount3;
            break;
        default:
            break; // TODO - should not get here
    }
    return;
}

bool LinearGame::CheckOrthogonal(int nPlayer, int nConnect) //const
{
    bool bWin = false;

    for (int yyy = 0; yyy < m_knY; ++yyy)
    {
        for (int xxx = 0; xxx < m_knX; ++xxx)
        {
            if (CheckHorizontal(nPlayer, yyy, xxx) == nConnect)
            {
                bWin = true;
                //if (nConnect == 3)
                    m_sWinBy.assign("Horizontal");
                break;
            }
        }

        if (bWin)
            break;
    }

    if (!bWin)
    {
        for (int yyy = 0; yyy < m_knY; ++yyy)
        {
            for (int xxx = 0; xxx < m_knX; ++xxx)
            {
                if (CheckVertical(nPlayer, yyy, xxx) == nConnect)
                {
                    bWin = true;
                    //if (nConnect == 3)
                        m_sWinBy.assign("Vertical");
                    break;
                }
            }

            if (bWin)
                break;
        }
    }

    return bWin;
}

int LinearGame::CheckHorizontal(int nPlayer, int y, int x) //const
{
    if (!ValidMove(x, y)) return 0;

    if (m_acGrid[y][x] == m_acTokens[nPlayer])
        return (1 + CheckHorizontal(nPlayer, y, x+1));
    else
        return 0;
}

int LinearGame::CheckVertical(int nPlayer, int y, int x) //const
{
    if (!ValidMove(x, y)) return 0;

    if (m_acGrid[y][x] == m_acTokens[nPlayer])
        return (1 + CheckVertical(nPlayer, y+1, x));
    else
        return 0;
}

bool LinearGame::CheckDiagonal(int nPlayer, int nConnect) //const
{
    bool bWin = false;

    for (int yyy = 0; yyy < m_knY; ++yyy)
    {
        for (int xxx = 0; xxx < m_knX; ++xxx)
        {
            if (CheckDiagonalUpperLeftLowerRight(nPlayer, yyy, xxx) == nConnect)
            {
                bWin = true;
                //if (nConnect == 3)
                    m_sWinBy.assign("DiagonalULLR");
                break;
            }
        }

        if (bWin)
            break;
    }

    if (!bWin)
    {
        for (int yyy = 0; yyy < m_knY; ++yyy)
        {
            for (int xxx = m_knX - 1; xxx >= 0; --xxx)
            {
                if (CheckDiagonalUpperRightLowerLeft(nPlayer, yyy, xxx) == nConnect)
                {
                    bWin = true;
                    //if (bWin == 3)
                        m_sWinBy.assign("DiagonalURLL");
                    break;
                }
            }

            if (bWin)
                break;
        }
    }

    return bWin;
}

int LinearGame::CheckDiagonalUpperLeftLowerRight(int nPlayer, int y, int x) //const
{
    if (!ValidMove(x, y)) return 0;

    if (m_acGrid[y][x] == m_acTokens[nPlayer])
        return (1 + CheckDiagonalUpperLeftLowerRight(nPlayer, y+1, x+1));
    else
        return 0;
}

int LinearGame::CheckDiagonalUpperRightLowerLeft(int nPlayer, int y, int x) //const
{
    if (!ValidMove(x, y)) return 0;

    if (m_acGrid[y][x] == m_acTokens[nPlayer])
        return (1 + CheckDiagonalUpperRightLowerLeft(nPlayer, y+1, x-1));
    else
        return 0;
}

bool LinearGame::ValidMove(int x, int y) const
{
    if ((x >= m_knX) || (x < 0))
        return false;

    if ((y >= m_knY) || (y < 0))
        return false;

    return true;
}

int LinearGame::PreferredMove(const GameMove &cGameMove) const
{
    (void)cGameMove;

    return 0;
}

std::string LinearGame::GameScore() const
{
    return "";
}

bool LinearGame::GameEnded(int nPlayer)
{
    m_nWinner = 0;
    m_sWinBy.assign("nothing");

    if (CheckOrthogonal(m_knPlayer1, m_knWin))
    {
        m_nWinner = m_knPlayer1;
        return true;
    }

    if (CheckOrthogonal(m_knPlayer2, m_knWin))
    {
        m_nWinner = m_knPlayer2;
        return true;
    }

    if (CheckDiagonal(m_knPlayer1, m_knWin))
    {
        m_nWinner = m_knPlayer1;
        return true;
    }

    if (CheckDiagonal(m_knPlayer2, m_knWin))
    {
        m_nWinner = m_knPlayer2;
        return true;
    }

    std::vector<GameMove> vGameMoves = GenerateMoves(nPlayer);
    if (vGameMoves.empty())
    {
        return true;
    }

    return false;
}
