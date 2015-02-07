#include "Human.h"

bool Human::Move(Game &cGame)
{
    GameMove cGameMove;

    cGame.Display(true);

    std::cout << "Valid moves: ";
    cGame.DisplayValidMoves();

    do
    {
        // TODO: prompt player for move?  cGame.PromptPlayerForMove(m_nPlayerNumber);  OR not ?
        std::cout << "Player " << m_nPlayerNumber << ", enter move: ";

        cGameMove = cGame.GetMove();

        if ( cGame.ApplyMove(m_nPlayerNumber, cGameMove) == -1 )
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
