#include "Human.h"

bool Human::Move(Game &cGame)
{
    // TODO: Move cMove;
    int nMove;

    cGame.Display(true);

    std::cout << "Valid moves: ";
    cGame.DisplayValidMoves();

    do
    {
        // TODO: prompt player for move?  cGame.PromptPlayerForMove(m_nPlayerNumber);  OR not ?
        // TODO: capture move: cMove = cGame.GetMove(); OR cGame.GetMove(cMove); ??
        std::cout << "Player " << m_nPlayerNumber << ", enter move: ";
        std::cin >> nMove;

        // TODO: cGame.ApplyMove(m_nPlayerNumber, cMove)
        if ( cGame.ApplyMove(m_nPlayerNumber, nMove) == -1 )
        {
            std::cout << "Invalid move" << std::endl;
        }
        else
        {
            break;
        }

    } while(true);

    return true;
}
