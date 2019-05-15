#include "game.h"
#include "cards.h"
#include <iostream>
#include <algorithm>

game::game()
{
    comCheck = false;
    menu2string = "2 - Check if my phase is done";
    gameState = false;
}

void game::gameStart()
{
    if(gameState == false)
    {
        gameState = true;
        std::cout << "A new game started! Try to collect cards to match your phases requirements!\n\n";
        std::cout << "The phases are defined in the following way: \n";
        std::cout << "run: a run is made of two or more cards numbered in order, in any color.\n";
        std::cout << "set: a set is made of two or more cards with the same number and any color.\n";
        std::cout << "\nThe cards will be displayed with their position in brackets.\n";
        std::cout << "The third card in your hand will look like this: \n";
        std::cout << "(3) 2 blue, you have a 2 with the color blue at the third position in your hand\n";
        std::cout << "\nAfter each turn you discard one of your cards\n";
        std::cout << "Every turn you get to draw a card, either the top card from\nthe visible discard stack or the top card from the normal hidden stack\n";
        std::cout << "The first card you'll draw will be from the normal stack, since no cards have been discarded yet\n";
        std::cout << '\n';
        cardStack.getCurrentPhaseDescr();

        std::cout << "\nReady to begin your first turn? Press enter to begin\n";

        std::cin.get();
        cardStack.giveInitialCards();
        playerTurn();
    }
    else
    {
        cardStack.createStack();
        cardStack.giveInitialCards();
        playerTurn();
    }
}

void game::playerTurn()
{
    int c;
    cardStack.setCurrentPlayer(0);
    cardStack.drawCard();

    do {
        std::cout << "\nWhat do you want to do?\n";
        std::cout << "1 - Show my cards\n";
        std::cout << menu2string << '\n';
        std::cout << "3 - Choose one card to discard and end the turn\n";
        std::cout << "4 - Check my current phases' requirements\n";
        std::cout << "5 - Add a card to your laid out phase\n";
        std::cout << "6 - Exit Game\n";
        if(!(std::cin >> c))
        {
            std::cout << "Please enter a correct menu choice\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        switch(c) {

            case 1:
            {
                std::cout << "These are your current cards: \n";
                cardStack.getPlayerCards(0);
                std::cout << '\n';
                break;
            }
            case 2:
            {
                if(cardStack.checkPhase() == true)
                   if(cardStack.getMenuStringCheck() == true)
                        menu2string = "2 - Show my laid out phase";
                break;
            }
            case 3:
            {
                if(cardStack.getPlayerVector().size() != 1)
                {
                    int c;
                    std::cout << "Which card do you want to discard?\nType in the card's position displayed in the brackets (i.e. (5) for the fifth card in your hand\n";
                    cardStack.getPlayerCards(0);
                    std::cout << "\n\nCard position: ";
                    while(!(std::cin >> c))
                    {
                        std::cout << "Please enter one of the cards positions.\n";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                    cardStack.discardCard(c);
                    std::cout << "\n\nNext turn is the computer\n";
                    computerTurn();
                }
                else
                {
                  cardStack.discardCard(c);
                  menu2string = "2 - Check if my phase is done";
                  congrats();
                }
                break;
            }
            case 4:
            {
                cardStack.getCurrentPhaseDescr();
                break;
            }
            case 5:
            {
                if(cardStack.getPlayerVector().size() != 1)
                    cardStack.addPhaseCards();
                else
                    if(cardStack.addPhaseCards() == true)
                    {
                        menu2string = "2 - Check if my phase is done";
                        congrats();
                    }
                break;
            }
            case 6:
            {
                exit(0);
            }
        }
    } while(c != 3);
}

void game::computerTurn()
{
    cardStack.setCurrentPlayer(1);
    if(comCheck == false)
    {
        cardStack.giveInitialCards();
        comCheck = true;
    }
    cardStack.drawCard();

    int i = rand() % 11 +1;
    cardStack.discardCard(i);
    playerTurn();
}

void game::congrats()
{
    std::cout << "  #####                                                                                             \n";
    std::cout << " #     #  ####  #    #  ####  #####    ##   ##### #    # #        ##   ##### #  ####  #    #  ####  \n";
    std::cout << " #       #    # ##   # #    # #    #  #  #    #   #    # #       #  #    #   # #    # ##   # #      \n";
    std::cout << " #       #    # # #  # #      #    # #    #   #   #    # #      #    #   #   # #    # # #  #  ####  \n";
    std::cout << " #       #    # #  # # #  ### #####  ######   #   #    # #      ######   #   # #    # #  # #      # \n";
    std::cout << " #     # #    # #   ## #    # #   #  #    #   #   #    # #      #    #   #   # #    # #   ## #    # \n";
    std::cout << "  #####   ####  #    #  ####  #    # #    #   #    ####  ###### #    #   #   #  ####  #    #  ####  \n";
    cardStack.initiateNextRound();
    comCheck = false;
    gameStart();
}
