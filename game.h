#ifndef GAME_H
#define GAME_H
#include <cards.h>
#include <string>

class game
{

private:
    cards cardStack;
    bool comCheck, gameState;
    std::string menu2string;

public:
    game();
    void gameStart();
    void playerTurn();
    void computerTurn();
    void congrats();
};

#endif // GAME_H
