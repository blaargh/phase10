#ifndef CARDS_H
#define CARDS_H
#include <vector>
#include <deque>
#include <string>
#include <stack>


class cards
{

private:
    struct cardStruct
    {
        std::vector<int> cardValue = { 1,2,3,4,5,6,7,8,9,10,11,12 };
        std::vector<std::string> cardColor = { "blue", "green", "red", "yellow" };
    };
    std::deque<std::pair<int, std::string>> cardStack, discardStack;
    std::vector<std::pair<int, std::string>> playerCards, computerCards, playerPhaseCards, computerPhaseCards;
    std::vector<int> playerCardValues, playerPhaseCardValues, computerCardValues, computerPhaseCardValues;
    std::vector<std::string> playerCardColors, playerPhaseCardColors, computerCardColors, computerPhaseCardColors;
    int currentPlayer, playerPhase, computerPhase;
    bool playerPhaseOut = false;
    bool computerPhaseOut = false;
    bool menuStringCheck = false;

public:
    cards();
    void giveInitialCards();
    void drawCard();
    bool checkPhase();
    void discardCard(int pos);
    void createStack();

    void getPlayerCards(int i);

    void updateCardProperties();

    int getCurrentPlayer();
    void setCurrentPlayer(int);

    int getPlayerPhase();
    void getCurrentPhaseDescr();
    void layOutPhase();
    bool addPhaseCards();
    void setPlayerPhase(int n);

    bool getMenuStringCheck();

    std::vector<std::pair<int, std::string>> getPlayerVector();
    void initiateNextRound();
};


#endif // CARDS_H
