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

#include "GameBoard.h"

void GameBoard::InitializeZobrist()
{
    //
    // Initialize Zobrist hash table
    //

    // Random number generator
#if defined(_WIN32)
    static unsigned seed  = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    ++seed;
    std::mt19937 RandomNumberGenerator{seed};
#else
    std::random_device RandomDevice{};
    std::mt19937_64 RandomNumberGenerator{RandomDevice()};
#endif // defined

    // Fill Zobrist hash table with random numbers
    // one for every piece on every square
    for (int iii = 0; iii < m_knNumberOfTypesOfPieces; ++iii)
    {
        for (int jjj = 0; jjj < m_knNumberOfSquares; ++jjj)
        {
            m_auiZobrist[iii][jjj] = RandomNumberGenerator();
        }
    }

    // Initialize the Zobrist key
    for (int yyy = 0; yyy < m_knY; ++yyy)
    {
        for (int xxx = 0; xxx < m_knX; ++xxx)
        {
            if (PositionOccupied(xxx, yyy))
            {
                //m_uiZobristKey ^= m_auiZobrist[PieceNumber(xxx, yyy)][(xxx + (yyy * m_knMaxY))];
                UpdateZobristKey(xxx, yyy, xxx, yyy);
            }
        }
    }
}

void GameBoard::UpdateZobristKey(int nPX, int nPY, int nSX, int nSY)
{
    m_uiZobristKey ^= m_auiZobrist[PieceNumber(nPX, nPY)][nSX + (nSY * m_knMaxY)];
}

/**
  * Clear the game board.
  *
  * Run through every space on the board and clear the game piece.
  */

void GameBoard::Clear()
{
    for (int xxx = 0; xxx < m_knX; ++xxx)
        for (int yyy = 0; yyy < m_knY; ++yyy)
            m_vBoard[yyy][xxx].Clear();
}

/**
  * Set a piece on the board.
  *
  * If a valid location, set the token and the player.
  *
  * \param nX X-Coordinate
  * \param nY Y-Coordinate
  * \param cGamePiece Game piece object
  *
  * \return True if piece set, false otherwise.
  */

bool GameBoard::SetPiece(int nX, int nY, const GamePiece &cGamePiece)
{
    if (!ValidLocation(nX, nY))
        return false;

    m_vBoard[nY][nX].Set(cGamePiece.Token(), cGamePiece.Player(), cGamePiece.Value(), cGamePiece.Number());

    return true;
}

/**
  * Move a piece on the board.
  *
  * Copy piece From location and paste it to the To location.
  *
  * \param GameMove The game move
  *
  * \return True if valid move, false otherwise.
  */

bool GameBoard::MovePiece(const GameMove &cGameMove)
{
    GamePiece cGamePiece = m_vBoard[cGameMove.FromY()][cGameMove.FromX()].Piece();

    if (SetPiece(cGameMove.ToX(), cGameMove.ToY(), cGamePiece))
    {
        m_vBoard[cGameMove.ToY()][cGameMove.ToX()].SetMoved();
        m_vBoard[cGameMove.FromY()][cGameMove.FromX()].Clear();
        return true;
    }
    else
    {
        return false;
    }
}

/**
  * Check validity of location on the board.
  *
  * If a valid location, return true.
  *
  * \param nX X-Coordinate
  * \param nY Y-Coordinate
  *
  * \return True if location is valid, false otherwise.
  */

bool GameBoard::ValidLocation(int x, int y) const
{
    if ((x >= m_knX) || (x < 0))
        return false;

    if ((y >= m_knY) || (y < 0))
        return false;

    return true;
}

/**
  * Evaluate if a position on the board is occupied.
  *
  * If occupied, return true.
  *
  * \param nX X-Coordinate
  * \param nY Y-Coordinate
  *
  * \return True if location is occupied, false otherwise.
  */

bool GameBoard::PositionOccupied(int nX, int nY) const
{
    if (m_vBoard[nY][nX].IsAGamePiece())
        return true;
    else
        return false;
}

/**
  * Return the player number of the game peice located on the board
  * at the coordinates.
  *
  * \param nX X-Coordinate
  * \param nY Y-Coordinate
  *
  * \return The player number of the located at the coordinates.
  */

int GameBoard::PositionOccupiedBy(int nX, int nY) const
{
    return m_vBoard[nY][nX].Player();
}

/**
  * Evaluate if a position on the board is occupied by a player's game piece.
  *
  * If occupied, return true.
  *
  * \param nX X-Coordinate
  * \param nY Y-Coordinate
  *
  * \return True if the player number parameter matches the player of the piece
  * located at the coordinates.  Otherwise false.
  */

bool GameBoard::PositionOccupiedByPlayer(int nX, int nY, int nPlayer) const
{
    if (m_vBoard[nY][nX].Player() == nPlayer)
        return true;
    else
        return false;
}


/**
  * Display the game board.
  *
  * Run through every space on the board and display it's token.  Also display
  * coordinates and grid lines, depending on settings.
  */

void GameBoard::Display() const
{
    std::string sColor;

#if defined(_WIN32)
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

    // Display X-coordinatres
    if (m_kbDisplayXCoordinates)
    {
        char c = m_kcXCoordinate;

        std::cout << "   ";
        for (int xxx = 0; xxx < m_knX; ++xxx)
        {

            std::cout << c << "   ";
            ++c;
        }
        std::cout << std::endl << std::endl;
    }

    for (int yyy = 0; yyy < m_knY; ++yyy)
    {
        int nDisplayY;

        if (m_bReverseY)
            nDisplayY = m_knY - yyy - 1;
        else
            nDisplayY = yyy;

        // Display Y-coordinates
        if (m_kbDisplayYCoordinates)
        {
            std::cout << nDisplayY + 1 << "  ";
        }
        else
        {
            std::cout << "   ";
        }

        for (int xxx = 0; xxx < m_knX; ++xxx)
        {
            // Display grid lines
            if (m_kbDisplayGrid && xxx > 0)
            {
                std::cout << " | ";
            }

            // Determine color of token
            if (m_vBoard[nDisplayY][xxx].Player() == 1)
            {
#if defined(_WIN32)
                SetConsoleTextAttribute(hConsole, m_nPlayer1TokenColor);
#else
                sColor = m_sPlayer1TokenColor;
#endif // defined

            }
            else if (m_vBoard[nDisplayY][xxx].Player() == 2)
            {
#if defined(_WIN32)
                SetConsoleTextAttribute(hConsole, m_nPlayer2TokenColor);
#else
                sColor = m_sPlayer2TokenColor;
#endif // defined
            }
            else // Clear color
            {
                sColor = "";
            }

#if defined(_WIN32)
            std::cout << m_vBoard[nDisplayY][xxx].Token();
            SetConsoleTextAttribute(hConsole, m_knResetTokenColor);
#else
            std::cout << sColor << m_vBoard[nDisplayY][xxx].Token() << m_ksResetTokenColor;
#endif // defined

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
