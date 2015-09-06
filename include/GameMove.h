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

/** \file
 *
 * \brief The GameMove class represents a game move.
 *
 */

#ifndef GAMEMOVE_H
#define GAMEMOVE_H

#include <string>

#include "Card.h"
#include "GameVocabulary.h"

#include "json/json.h"

class GameMove
{
    public:
        // Construct a GameMove
        GameMove() {}

        // Construct using integer values and flags
        GameMove(int nFromX, int nFromY, int nToX, int nToY, bool bUseY, bool bUseFrom) :
            m_nFromX(nFromX),
            m_nFromY(nFromY),
            m_nToX(nToX),
            m_nToY(nToY),
            m_bUseY(bUseY),
            m_bUseFrom(bUseFrom)
        {
            m_cFromX = m_nFromX + m_kcXOffset;
            m_cFromY = m_nFromY + m_kcYOffset;
            m_cToX   = m_nToX + m_kcXOffset;
            m_cToY   = m_nToY + m_kcYOffset;
        }

        // Construct from character values
        GameMove(char cFromX, char cFromY, char cToX, char cToY)
        {
            SetFromX(cFromX);
            SetFromY(cFromY);
            SetToX(cToX);
            SetToY(cToY);
        }

        // Destructor
        virtual ~GameMove() {};

        // Set and return player number
        void SetPlayerNumber(int n)       { m_nPlayerNumber = n; }
        int PlayerNumber() const          { return m_nPlayerNumber; }

        // Return move command and argument
        std::string Command() const { return m_sCommand; }

        // Set and return move command and argument
        void SetArgument(std::string sArg) { m_sArgument.assign(sArg); }
        std::string Argument() const;

        // Modify the coordinates of a move
        void SetFromX(char cX)           { m_cFromX = tolower(cX); m_nFromX = m_cFromX - m_kcXOffset; }
        void SetFromX(int nX)            { m_nFromX = nX; m_cFromX = m_nFromX + m_kcXOffset; }
        void SetToX(char cX)             { m_cToX = tolower(cX); m_nToX = m_cToX - m_kcXOffset; }
        void SetToX(char cX, bool bUseY) { SetToX(cX); m_bUseY = bUseY; }
        void SetToX(int nX)              { m_nToX = nX; m_cToX = m_nToX + m_kcXOffset; }
        void SetFromY(char cY)           { m_cFromY = tolower(cY); m_nFromY = m_cFromY - m_kcYOffset; }
        void SetFromY(int nY)            { m_nFromY = nY; m_cFromY = m_nFromY + m_kcYOffset; }
        void SetToY(char cY)             { m_cToY = tolower(cY); m_nToY = m_cToY - m_kcYOffset; }
        void SetToY(int nY)              { m_nToY = nY; m_cToY = m_nToY + m_kcYOffset; }

        // Modify several coordinates in one call
        void Set(int nFromX, int nFromY, int nToX, int nToY, bool bUseY, bool bUseFrom)
        {
            SetFromX(nFromX);
            SetFromY(nFromY);
            SetToX(nToX);
            SetToY(nToY);
            SetUseY(bUseY);
            SetUseFrom(bUseFrom);
        }

        // Return the coordinates of a move
        int  FromX() const      { return m_nFromX; }
        int  FromY() const      { return m_nFromY; }
        int  ToX()   const      { return m_nToX; }
        int  ToY()   const      { return m_nToY; }
        char FromInputX() const { return m_cFromX; }
        char FromInputY() const { return m_cFromY; }
        char ToInputX() const   { return m_cToX; }
        char ToInputY() const   { return m_cToY; }

        // Set and return whether the Y-coordinate is used
        void SetUseY(bool b)   { m_bUseY = b; }
        bool UseY() const      { return m_bUseY; }

        // Set and return whether the From coordinates are used
        void SetUseFrom(bool b)   { m_bUseFrom = b; }
        bool UseFrom() const      { return m_bUseFrom; }

        //  Set and return whether this is a Move - the default is set to true
        void SetMove(bool b) { m_bMove = b; if (b) m_sCommand = GameVocabulary::MOVE; else m_sCommand.clear(); }
        bool IsMove() const    { return m_bMove; }

        // Set and return whether a No-move has been made
        void SetNoMove(bool b) { m_bNoMove = b; if (b) m_sCommand = GameVocabulary::NO_MOVE; else m_sCommand.clear(); }
        bool NoMove() const    { return m_bNoMove; }

        // Set and return whether a resignation has been made
        void SetResignation(bool b) { m_bResignation = b; if (b) m_sCommand = GameVocabulary::RESIGN; else m_sCommand.clear(); }
        bool Resignation() const    { return m_bResignation; }

        // Set and return whether a fold has been made
        void SetFold(bool b) { m_bFold = b; if (b) m_sCommand = GameVocabulary::FOLD; else m_sCommand.clear(); }
        bool Fold() const    { return m_bFold; }

        // Set and return whether a draw has been made
        void SetDraw(bool b)      { m_bDraw = b; m_nDraw = 1; if (b) m_sCommand = GameVocabulary::DRAW; else m_sCommand.clear(); }
        void SetDrawNumber(int n) { m_nDraw = n; }
        bool Draw() const         { return m_bDraw; }
        int  DrawNumber() const   { return m_nDraw; }

        // Set and return whether an ask has been made
        void SetAsk(bool b)       { m_bAsk = b; if (b) m_sCommand = GameVocabulary::ASK; else m_sCommand.clear(); }
        bool Ask() const          { return m_bAsk; }

        // Set and return whether a show has been made
        void SetShow(bool b)       { m_bShow = b; if (b) m_sCommand = GameVocabulary::SHOW; else m_sCommand.clear(); }
        bool Show() const          { return m_bShow; }

        // Set and return whether a score request has been made
        void SetScore(bool b)      { m_bScore = b; if (b) m_sCommand = GameVocabulary::SCORE; else m_sCommand.clear(); }
        bool Score() const         { return m_bScore; }

        // Whether another turn may be had
        void SetAnotherTurn(bool b) { m_bAnotherTurn = b; }
        bool AnotherTurn() const    { return m_bAnotherTurn; }

        // Set and return whether this is a test move
        void SetTestMove(bool b) { m_bTestMove = b; }
        bool TestMove() const    { return m_bTestMove; }

        // Compare two moves and whether their to-moves are the same
        bool SameTo(const GameMove &cGameMove) { if ((cGameMove.ToX() == m_nToX) && (cGameMove.ToY() == m_nToY)) return true; else return false;}

        // Add card to move
        void UpdateCard(Card &cCard) { m_cCard = cCard; }
        Card GetCard() const         { return m_cCard; }

        // Announce the moves
        std::string AnnounceMove() const;
        std::string AnnounceFromMove() const;
        std::string AnnounceToMove() const;
        std::string AnnounceCard() const;
        std::string AnnounceCardRank() const;

        // Json object serialization
        Json::Value JsonSerialization() const;
        bool        JsonDeserialization(const std::string &sJsonGameValue, std::string &sErrorMessage);

    private:
        // Used to convert int coordinates to char values for internal representation using ASCII representation
        static const char m_kcXOffset {'a'};
        static const char m_kcYOffset {'1'};

        // Player's number
        int m_nPlayerNumber {-1};

        // Command and Argument
        std::string m_sCommand  {GameVocabulary::MOVE};
        std::string m_sArgument {};

        // Game move coordinates - initialize to garbage
        int  m_nFromX {-1};   // Start = 0
        char m_cFromX {'?'};  // Start = 'a'

        int  m_nFromY {-1};   // Start = 0
        char m_cFromY {'?'};  // Start = '1'

        int  m_nToX {-1};     // Start = 0
        char m_cToX {'?'};    // Start = 'a'

        int  m_nToY {-1};     // Start = 0
        char m_cToY {'?'};    // Start  ='1'

        // Whether the Y-coordinates are used
        bool m_bUseY {true};

        // Whether the From coordinates are used
        bool m_bUseFrom {false};

        // Whether this is a Move
        bool m_bMove {true};

        // Whether this is a move or not; used if no move is possible
        bool m_bNoMove {false};

        // Whether this is a resignation
        bool m_bResignation {false};

        // Card
        Card m_cCard;

        // Whether this is a fold
        bool m_bFold {false};

        // Whether this is a draw and the number to draw
        bool m_bDraw {false};
        int  m_nDraw {0};

        // Whether this is an ask
        bool m_bAsk {false};

        // Whether this is a show
        bool m_bShow {false};

        // Whether this is a show
        bool m_bScore {false};

        // Whether another turn may be had
        bool m_bAnotherTurn {false};

        // Whether this is a test move
        bool m_bTestMove {false};
};

#endif // GAMEMOVE_H
