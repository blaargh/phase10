#include <iostream>
//#include <stdlib.h>
#include "cards.h"
#include "game.h"



/* To_Do
 *
 * --ComPhase auslegen (erstmal nicht notwendig, weil der com aktuell nur mit extrem viel glück gewinnen kann)
 * --Beide Phasen anschaubar machen (siehe oben)
 *
 *
 *
 *
 *
 *
 * Die restlichen Phasen entwerfen
 */

int main()
{
    std::cout << " ##      ## ######## ##        ######   #######  ##     ## ########  " << '\n';
    std::cout << " ##  ##  ## ##       ##       ##    ## ##     ## ###   ### ##        " << '\n';
    std::cout << " ##  ##  ## ##       ##       ##       ##     ## #### #### ##        " << '\n';
    std::cout << " ##  ##  ## ######   ##       ##       ##     ## ## ### ## ######    " << '\n';
    std::cout << " ##  ##  ## ##       ##       ##       ##     ## ##     ## ##        " << '\n';
    std::cout << " ##  ##  ## ##       ##       ##    ## ##     ## ##     ## ##        " << '\n';
    std::cout << "  ###  ###  ######## ########  ######   #######  ##     ## ########  " << '\n';
    std::cout << "" << '\n';
    std::cout << "" << '\n';
    std::cout << " ########  #######                                                   " << '\n';
    std::cout << "    ##    ##     ##                                                  " << '\n';
    std::cout << "    ##    ##     ##                                                  " << '\n';
    std::cout << "    ##    ##     ##                                                  " << '\n';
    std::cout << "    ##    ##     ##                                                  " << '\n';
    std::cout << "    ##    ##     ##                                                  " << '\n';
    std::cout << "    ##     #######                                                   " << '\n';
    std::cout << "" << '\n';
    std::cout << "" << '\n';
    std::cout << " ########  ##     ##    ###     ######  ########       ##     #####  " << '\n';
    std::cout << " ##     ## ##     ##   ## ##   ##    ## ##           ####    ##   ## " << '\n';
    std::cout << " ##     ## ##     ##  ##   ##  ##       ##             ##   ##     ##" << '\n';
    std::cout << " ########  ######### ##     ##  ######  ######         ##   ##     ##" << '\n';
    std::cout << " ##        ##     ## #########       ## ##             ##   ##     ##" << '\n';
    std::cout << " ##        ##     ## ##     ## ##    ## ##             ##    ##   ## " << '\n';
    std::cout << " ##        ##     ## ##     ##  ######  ########     ######   #####  " << '\n';
    std::cout << "\nPress enter to start game";
    std::cin.get();
    game newGame;
    newGame.gameStart();
}
