#include "Human.h"

bool Human::Move(Game &cGame)
{
    int nMove;

    cGame.Display(true);

    std::cout << "Valid moves: ";
    cGame.DisplayValidMoves();

    do
    {
        std::cout << "Player " << m_nPlayerNumber << ", enter move: ";
        std::cin >> nMove;

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
