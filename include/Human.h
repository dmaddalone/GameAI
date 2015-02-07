#ifndef HUMAN_H
#define HUMAN_H

#include <iostream>
#include "Player.h"
#include "Game.h"
#include "GameMove.h"

class Human: public Player
{
    public:
        Human(PlayerType ecPlayerType) : Player(ecPlayerType) {};
        ~Human() {};

        bool Move(Game &cGame);

        std::string TypeName() { return "Human"; }

    private:
};

#endif // HUMAN_H
