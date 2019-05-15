#include "cards.h"
#include <iostream>
#include <ctime>
#include <algorithm>
#include <map>
#include <random>
#include <sstream>

cards::cards()
{
    srand(time(nullptr));
    createStack();
    currentPlayer = 0;
    playerPhase = 3;
    computerPhase = 1;
}

void cards::createStack()
{
    cardStruct fullStack;
    for(int x = 0; x < 4; x++)
        for(int i = 0; i < 12; i++)
        {
            if(i < 9)
            {
                // for readability reasons, and to be able to easily seperate numbers and colors later, a second space is added for single digits
                std::string card = fullStack.cardValue.at(i)+ "  " +fullStack.cardColor.at(x);
                stack1.push_back(card);
            }
            else
            {
                std::string card = fullStack.cardValue.at(i) + " " + fullStack.cardColor.at(x);
                stack1.push_back(card);
            }
        }
    // double all cards in the stack
    for(std::string i : stack1)
        stack1.push_back(i);

  //on macOS, use
  //shuffle(stack1.begin(), stack1.end(), default_random_engine(rand()));

    std::random_shuffle(stack1.begin(), stack1.end());
}

void cards::giveInitialCards()
{
    std::vector<std::string> *cardPointer;

    if(currentPlayer == 0)
    {
        cardPointer = &playerCards;
    }
    else
        cardPointer = &computerCards;

    for(int i = 0; i < 10; i++)
    {
        cardPointer->push_back(stack1.back());
        stack1.pop_back();
    }
    updateCardProperties();

}

void cards::drawCard()
{
    std::vector<std::string> *cardPointer;
    int choice;

    if(currentPlayer == 0)
    {
        cardPointer = &playerCards;

        if(discardStack.empty())
        {
            choice = 1;
            std::cout << "\nSorry, no cards on the discard stack at the moment! Card will be drawn from normal stack\n";
            cardPointer->push_back(stack1.back());
            stack1.pop_back();
        }
        else
        {
            do
            {
                std::cout << "\nDraw card from normal stack or show the top card of the discard stack?\n(1 for normal, 2 for discard stack): \n";
                if(!(std::cin >> choice))
                {
                    std::cout << "Please select one of the above options.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                switch(choice)
                {
                    case 1:
                    {
                         cardPointer->push_back(stack1.back());
                         stack1.pop_back();
                         break;
                    }
                    case 2:
                    {
                         char yn;
                         std::cout << "\nDo you want to draw " << discardStack.back() << " ? (y\\n)";
                         std::cin >> yn;
                         if(yn == 'y')
                         {
                             cardPointer->push_back(discardStack.back());
                             discardStack.pop_back();
                         }
                         else
                         {
                             cardPointer->push_back(stack1.back());
                             stack1.pop_back();
                         }
                         break;
                    }
                }
            } while(choice != 1 && choice != 2);
        }
        std::cout << "\nYou drew a " << playerCards.at(playerCards.size()-1) << '\n';

    }
    else
    {
        cardPointer = &computerCards;
        cardPointer->push_back(stack1.back());
        stack1.pop_back();
    }

    updateCardProperties();
}

bool cards::checkPhase()
{
    switch(playerPhase)
    {
        case 1: // 4 sets of 2
        {
            std::vector<std::string> *cardPointer;
            int p = 0;
            std::map<std::string, int> countMap;
            if(currentPlayer == 0)
            {
                if(playerPhaseOut == false && playerPhaseCardValues.empty())
                    cardPointer = &playerCardValues;
                else
                    cardPointer = &playerPhaseCardValues;

            }
            else
                cardPointer = &computerCardValues;

            std::sort(cardPointer->begin(), cardPointer->end());

            for(auto &i : *cardPointer)
            {
                auto result = countMap.insert(std::pair<std::string, int>(i, 1));
                if(result.second == false)
                {
                    result.first->second++;
                    p++;
                }
            }
            for(auto &i : countMap)
            {
                if(i.second > 2)
                    p--;
                if(i.second == 5)
                    p--;
            }

            if(p >= 4)
            {
                if(currentPlayer == 0)
                {
                    if(playerPhaseOut == false)
                    {
                        if(!playerPhaseCardValues.empty())
                        {
                            return true;
                        }
                        else
                        {
                            std::cout << "Phase 1 done! Your current amount of sets is: " << p << "\n\n";
                            layOutPhase();
                        }
                    }
                    else
                    {
                        std::cout << "These are your laid out cards: \n";
                        getPlayerCards(1);
                    }
                }
                else
                    std::cout << "The computer finished phase 1. He had " << p << " sets.\n\n";
                return true;
            }
            else
            {
                if(currentPlayer == 0)
                {
                    if(playerPhaseOut == false)
                    {
                        if(!playerPhaseCardValues.empty())
                            return false;
                        else
                            std::cout << "Not enough sets, keep collecting! Your current amount of sets is: " << p << "\n\n";
                    }
                    else
                    {
                        std::cout << "These are your laid out cards: \n";
                            getPlayerCards(1);
                    }
                }
                else
                    std::cout << "The computer couldn't finish phase 1, he only has " << p << " sets\n\n";
                return false;
            }
            break;
        }

        case 2: // 6 cards of one color
        {
            std::vector<std::string> *cardPointer;
            std::string color;
            int p = 0;
            std::map<std::string, int> countMap;
            if(currentPlayer == 0)
            {
                if(playerPhaseOut == false && playerPhaseCardColors.empty())
                    cardPointer = &playerCardColors;
                else
                    cardPointer = &playerPhaseCardColors;
            }
            else
                cardPointer = &computerCardColors;

            std::sort(cardPointer->begin(), cardPointer->end());

            for(auto &i : *cardPointer)
            {
                auto result = countMap.insert(std::pair<std::string, int>(i, 1));
                if(result.second == false)
                {
                    result.first->second++;
                }
            }
            for(auto &i : countMap)
            {
                if(i.second >= 6)
                {
                    color = i.first;
                    p = i.second;
                }
            }

            if(p >= 6)
            {
                if(currentPlayer == 0)
                {
                    if(playerPhaseOut == false)
                    {
                        if(!playerPhaseCardColors.empty())
                        {
                            return true;
                        }
                        else
                        {
                            std::cout << "Phase 2 done! Your current amount of " << color << " is: " << p << "\n\n";
                            layOutPhase();
                        }
                    }
                    else
                    {
                        std::cout << "These are your laid out cards: \n";
                        getPlayerCards(1);
                    }
                }
                else
                    std::cout << "The computer finished phase 2.\n";
                return true;
            }
            else
            {
                if(currentPlayer == 0)
                {
                    if(playerPhaseOut == false)
                    {
                        if(!playerPhaseCardColors.empty())
                            return false;
                        else
                            std::cout << "Not enough colors, keep collecting!\n";
                    }
                    else
                    {
                        std::cout << "These are your laid out cards: \n";
                            getPlayerCards(1);
                    }
                }
                else
                    std::cout << "The computer couldn't finish phase 2!\n\n";
                return false;
            }
            break;
        }

        case 3: // 1 set of 4 + 1 run of 4
        {
            std::vector<std::string> *cardPointer;
            std::vector<int> intUniques;
            std::vector<int> intTwins;
            std::map<int, int> countMap;
            if(currentPlayer == 0)
            {
                if(playerPhaseOut == false && playerPhaseCardValues.empty())
                    cardPointer = &playerCardValues;
                else
                    cardPointer = &playerPhaseCardValues;
            }
            else
                cardPointer = &computerCardValues;

            for(auto &i : *cardPointer)
            {
                std::stringstream parser(i);
                int x = 0;
                parser >> x;

                intTwins.push_back(x);
            }
            std::sort(intTwins.begin(), intTwins.end());
            std::unique_copy(intTwins.begin(), intTwins.end(), std::back_inserter(intUniques));

            int j = 0;
            int k = 0;
            int p = 0;
            bool run = false;
            bool set = false;

            for(auto &i : intTwins)
            {
                auto result = countMap.insert(std::pair<int, int>(i, 1));
                if(result.second == false)
                {
                    result.first->second++;

                }
            }
            for(auto &i : countMap)
            {
                if(i.second >=4)
                {
                    p=i.second;
                }
                if(i.second == 4)
                {
                    ptrdiff_t pos = std::distance(intUniques.begin(), find(intUniques.begin(), intUniques.end(), i.first));
                    intUniques.erase(intUniques.begin()+pos);
                }
            }

            for(unsigned int i = 0; i < intUniques.size(); i++)
            {
              if(i == 0)
              {
                if(intUniques.at(i)+1 == intUniques.at(i+1))
                {
                    j++;
                    if(j>k)
                        k=j;
                }
              }
              else
              {
                 if(i == intUniques.size()-1)
                 {
                     if(intUniques.at(i-1)+1 == intUniques.at(i))
                     {
                         j++;
                         if(j>k)
                             k=j;
                     }
                 }
                 else
                 {
                     if((intUniques.at(i+1)-intUniques.at(i) == 1) || (intUniques.at(i)-intUniques.at(i-1) == 1))
                     {
                         j++;
                         if(j>k)
                             k=j;
                     }
                     if((intUniques.at(i+1)-intUniques.at(i) != 1) || (intUniques.at(i)-intUniques.at(i-1) != 1))
                     {
                         j = 1;
                     }
                 }
              }
            }
            if(k>=4)
            {
                run = true;
            }

            if(p>=4)
            {
                set = true;
            }

            if(run == true && set == true)
            {
                if(currentPlayer == 0)
                {
                    if(playerPhaseOut == false)
                    {
                        if(!playerPhaseCardValues.empty())
                        {
                            return true;
                        }
                        else
                        {
                            std::cout << "Phase 3 done!\n";
                            layOutPhase();
                        }
                    }
                    else
                    {
                        std::cout << "These are your laid out cards: \n";
                        getPlayerCards(1);
                    }
                }
                else
                    std::cout << "The computer finished phase 3.\n";
                return true;
            }
            else
            {
                if(currentPlayer == 0)
                {
                    if(playerPhaseOut == false)
                    {
                        if(!playerPhaseCardValues.empty())
                            return false;
                        else
                            std::cout << "One or more of the phases requirements are not met, keep collecting.\n";
                    }
                    else
                    {
                        std::cout << "These are your laid out cards: \n";
                            getPlayerCards(1);
                    }
                }
                else
                    std::cout << "The computer couldn't finish phase 3.\n";
                return false;
            }
            break;
        }

        case 4: // 1 run of 8
        {
            std::cout << "4 in the making...\n";
            break;
        }

        case 5: // 7 cards of one color
        {
            std::cout << "5 in the making...\n";
            break;
        }

        case 6: // 1 run of 9
        {
            std::cout << "6 in the making...\n";
            break;
        }

        case 7: // 2 sets of 4
        {
            std::cout << "7 in the making...\n";
            break;
        }

        case 8: // 1 run of 4 of one color + 1 set of 3
        {
            std::cout << "8 in the making...\n";
            break;
        }

        case 9: // 1 set of 5 + 1 set of 3;
        {
            std::cout << "9 in the making...\n";
            break;
        }

        case 10: // 1 set of 5 + 1 run of 3 of one color
        {
            std::cout << "10 in the making...\n";
            break;
        }
    }
    std::cout << "nothing, returning false\n";
    return false;
}

void cards::layOutPhase()
{
    if(currentPlayer == 0)
    {
        if(playerPhaseOut == false)
        {
            std::string line;
            int number;
            std::cout << "Choose the cards you want to lay out for your current phase\n";
            getPlayerCards(0);
            std::cout << "Enter all card positions to lay out like this: \"1 2 3 4 5\" etc.\nCard Positions: ";
            getchar();
            std::getline(std::cin, line);
            std::stringstream ss(line);


            while(!ss.eof())
            {
                ss >> number;
                if(number <= 0 || number > 11)
                {
                    std::cout << "One or more of the card positions you entered are not existing, please select option 2 again and re-enter your cards.\n";
                    return;
                }
                else
                    playerPhaseCards.push_back(playerCards.at(number-1));
            }

            updateCardProperties();
            switch(playerPhase)
            {
                case 1:
                case 3:
                {
                    if(checkPhase() == true && playerPhaseCards.size() < 9)
                    {
                        for(unsigned int i = 0; i < playerPhaseCards.size(); i++)
                            playerCards.erase(std::remove(playerCards.begin(), playerCards.end(), playerPhaseCards.at(i)), playerCards.end());
                        playerPhaseOut = true;
                        std::cout << "You succesfully laid out your phase. Check your laid out cards with option 2.\n";
                        menuStringCheck = true;
                    }
                    else
                    {
                        std::cout << "The cards you entered are not valid to pass this phase, please select option 2 again and re-enter your cards.\n";
                        playerPhaseCards.clear();
                    }
                    break;
                }
                case 2:
                {
                    if(checkPhase() == true && playerPhaseCards.size() != 7)
                    {
                        for(unsigned int i = 0; i < playerPhaseCards.size(); i++)
                            playerCards.erase(std::remove(playerCards.begin(), playerCards.end(), playerPhaseCards.at(i)), playerCards.end());
                        playerPhaseOut = true;
                        std::cout << "You succesfully laid out your phase. Check your laid out cards with option 2.\n";
                        menuStringCheck = true;
                    }
                    else
                    {
                        std::cout << "The cards you entered are not valid to pass this phase, please select option 2 again and re-enter your cards.\n";
                        playerPhaseCards.clear();
                    }
                    break;
                }
            }
        }
        else
        {
            std::cout << "These are your laid out cards: \n";
                getPlayerCards(1);
        }
    }

    if(currentPlayer == 1)
    {
        if(computerPhaseOut == false)
        {

        }
    }
    updateCardProperties();
}

bool cards::addPhaseCards()
{
    updateCardProperties();

    unsigned int c;
    std::cout << "Enter the card position you want to add to your phase\n";
    getPlayerCards(0);
    if(!(std::cin >> c))
    {
        std::cout << "Please select one of your cards.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    c--;
    switch(playerPhase)
    {
        case 1:
        case 2:
        {
            std::vector<std::string> *pPlayerCards;
            std::vector<std::string> *pPhaseCards;
            if(playerPhase == 1)
            {
                pPhaseCards = &playerPhaseCardValues;
                pPlayerCards = &playerCardValues;
            }
            if(playerPhase == 2)
            {
                pPhaseCards = &playerPhaseCardColors;
                pPlayerCards = &playerCardColors;
            }

            if(playerPhaseOut == true)
            {
                if(c > playerCards.size())
                {
                    std::cout << "The card position you entered is not valid.\n";
                    return false;
                }

                if(playerCards.size() == 1)
                {
                    if(std::find(pPhaseCards->begin(), pPhaseCards->end(), pPlayerCards->at(c)) != pPhaseCards->end())
                    {
                        playerCards.erase(std::remove(playerCards.begin(), playerCards.end(), playerCards.at(c)), playerCards.end());
                        return true;
                    }
                    else
                    {
                        std::cout << "Your last card does not match the phase, but you're close!\n";
                        return false;
                    }
                }
                else
                {
                    if(std::find(pPhaseCards->begin(), pPhaseCards->end(), pPlayerCards->at(c)) != pPhaseCards->end())
                    {
                        playerPhaseCards.push_back(playerCards.at(c));
                        std::cout << "You added " << playerPhaseCards.at(playerPhaseCards.size()-1) << " to your laid out phase!\nOnly " << playerCards.size()-1 << " more card(s) to get rid of to win this round!\n";
                        playerCards.erase(std::remove(playerCards.begin(), playerCards.end(), playerCards.at(c)), playerCards.end());
                        return true;
                    }
                    else
                    {
                        std::cout << "Can't add " << playerCards.at(c) << " to your phase on the board, it doesnt match any of the colors!\n";
                        return false;
                    }
                }
            }
            else
                std::cout << "You have not finished your phase yet, keep collecting\n";
            break;
        }
        case 3:
        {
            std::map<int, int> countMap;
            std::vector<int> intPlayerCards, intUniques, intTwins;
            int k = 0;
            for(auto &i : playerPhaseCardValues)
            {
                std::stringstream parser(i);
                int x = 0;
                parser >> x;
                intUniques.push_back(x);
            }
            for(auto &i : playerCardValues)
            {
                std::stringstream parser(i);
                int x = 0;
                parser >> x;
                intPlayerCards.push_back(x);
            }
            for(auto &i : intUniques)
            {
                auto result = countMap.insert(std::pair<int, int>(i, 1));
                if(result.second == false)
                {
                    result.first->second++;
                }
            }
            for(auto &i : countMap)
            {
                if(i.second > 1)
                {
                    k=i.first;
                    while(std::find(intUniques.begin(), intUniques.end(), k) != intUniques.end())
                    {
                        intTwins.push_back(k);
                        intUniques.erase(std::remove(intUniques.begin(), intUniques.end(), k));
                    }
                }
            }

            if(playerPhaseOut == true)
            {
                if(std::find(intTwins.begin(), intTwins.end(), intPlayerCards.at(c)) != intTwins.end())
                {
                    playerPhaseCards.push_back(playerCards.at(c));
                    std::cout << "You added " << playerPhaseCards.at(playerPhaseCards.size()-1) << " to your laid out set!\nOnly " << playerCards.size()-1 << " more card(s) to get rid of to win this round!\n";
                    playerCards.erase(std::remove(playerCards.begin(), playerCards.end(), playerCards.at(c)), playerCards.end());
                    return true;
                }
                else
                {
                    if(intPlayerCards.at(c) == 1)
                    {
                        if(std::find(intUniques.begin(), intUniques.end(), intPlayerCards.at(c)+1) != intUniques.end())
                        {
                            if(std::find(intUniques.begin(), intUniques.end(), intPlayerCards.at(c)) != intUniques.end())
                            {
                                std::cout << "Can't add " << playerCards.at(c) << " to your phase on the board, it doesnt match either the set or the run!\n";
                                return false;
                            }
                            else
                            {
                                playerPhaseCards.push_back(playerCards.at(c));
                                std::cout << "You added " << playerPhaseCards.at(playerPhaseCards.size()-1) << " to your laid out run!\nOnly " << playerCards.size()-1 << " more card(s) to get rid of to win this round!\n";
                                playerCards.erase(std::remove(playerCards.begin(), playerCards.end(), playerCards.at(c)), playerCards.end());
                                return true;
                            }
                        }
                        else
                        {
                            std::cout << "Can't add " << playerCards.at(c) << " to your phase on the board, it doesnt match either the set or the run!\n";
                            return false;
                        }
                    }
                    if(intPlayerCards.at(c) != 1)
                    {
                        if(std::find(intUniques.begin(), intUniques.end(), intPlayerCards.at(c)-1) != intUniques.end() || std::find(intUniques.begin(), intUniques.end(), intPlayerCards.at(c)+1) != intUniques.end())
                        {
                            if(std::find(intUniques.begin(), intUniques.end(), intPlayerCards.at(c)) != intUniques.end())
                            {
                                std::cout << "Can't add " << playerCards.at(c) << " to your phase on the board, it doesnt match either the set or the run!\n";
                                return false;
                            }
                            else
                            {
                                playerPhaseCards.push_back(playerCards.at(c));
                                std::cout << "You added " << playerPhaseCards.at(playerPhaseCards.size()-1) << " to your laid out run!\nOnly " << playerCards.size()-1 << " more card(s) to get rid of to win this round!\n";
                                playerCards.erase(std::remove(playerCards.begin(), playerCards.end(), playerCards.at(c)), playerCards.end());
                                return true;
                            }
                        }
                        else
                        {
                            std::cout << "Can't add " << playerCards.at(c) << " to your phase on the board, it doesnt match either the set or the run!\n";
                            return false;
                        }
                    }
                }

            }
            break;
        }
    }

    return 0;
}

void cards::discardCard(int pos)
{
    std::vector<std::string> *cardPointer;
    std::string cardValue;
    if(currentPlayer == 0)
    {
        cardPointer = &playerCards;
        if(cardPointer->size() != 1)
            std::cout << "You discarded: " << cardPointer->at(pos-1) << '\n';
        else
            std::cout << "You discarded your last card.\n";
    }
    else
    {
        cardPointer = &computerCards;
        std::cout << "The computer has discarded a card. Its turn is over.\n";
    }

    if(cardPointer->size() != 1)
    {
        cardValue = cardPointer->at(pos-1);
        cardPointer->erase(cardPointer->begin() + pos -1);
        discardStack.push_back(cardValue);
        updateCardProperties();
    }
    else
        cardPointer->clear();

}

void cards::initiateNextRound()
{
    if(playerCards.empty())
    {
        std::cout << "Congratulations, you won phase " << playerPhase << "and will move up to phase " << playerPhase+1 << ".\n";
        playerPhase++;
    }
    else
    {
        if(computerCards.empty())
        {
            std::cout << "Looks like the computer won phase " << computerPhase << ". It will move up to phase " << computerPhase+1 << ".\n";
            computerPhase++;
        }
    }
    playerPhaseOut = false;
    menuStringCheck = false;
    currentPlayer = 0;
    playerCards.clear();
    computerCards.clear();
    discardStack.clear();
    playerCardValues.clear();
    playerCardColors.clear();
    playerPhaseCards.clear();
    playerPhaseCardValues.clear();
    playerPhaseCardColors.clear();
    stack1.clear();
}

void cards::updateCardProperties()
{
    if(currentPlayer == 0)
    {
        playerCardValues.clear();
        playerCardColors.clear();
        playerPhaseCardValues.clear();
        playerPhaseCardColors.clear();

        for(std::string i : playerCards)
            playerCardValues.push_back(i.substr(0,2));

        for(std::string i : playerCards)
            playerCardColors.push_back(i.substr(3,8));

        if(!playerPhaseCards.empty())
        {
            for(std::string i : playerPhaseCards)
                playerPhaseCardValues.push_back(i.substr(0,2));

            for(std::string i : playerPhaseCards)
                playerPhaseCardColors.push_back(i.substr(3,8));
        }
    }
    else
    {
        computerCardValues.clear();
        computerCardColors.clear();

        for(std::string i : computerCards)
            computerCardValues.push_back(i.substr(0,2));

        for(std::string i : computerCards)
            computerCardColors.push_back(i.substr(3,8));
    }

}

void cards::getPlayerCards(int i)
{
    std::vector<std::string> *cardPointer;
    if(i == 0)
        cardPointer = &playerCards;
    else
        cardPointer = &playerPhaseCards;


    for(unsigned int i = 0; i < cardPointer->size(); i++)
    {
        if(i % 3 == 0)
            std::cout << '\n';

        std::cout << "\t\t(" << i+1 << ") " << cardPointer->at(i) << "\t";

    }
    std::cout << '\n';
}

std::vector<std::string> cards::getPlayerVector()
{
    return playerCards;
}

int cards::getCurrentPlayer()
{
    return currentPlayer;
}

void cards::setCurrentPlayer(int n)
{
    currentPlayer = n;
}

int cards::getPlayerPhase()
{
    return playerPhase;
}

void cards::setPlayerPhase(int n)
{
    playerPhase = n;
}

bool cards::getMenuStringCheck()
{
    return menuStringCheck;
}

void cards::getCurrentPhaseDescr()
{
    switch(playerPhase)
    {
        case 1:
        {
            std::cout << "You are currently in Phase 1. It requires 4 sets of 2 cards.\n";
            break;
        }
        case 2:
        {
            std::cout << "You are currently in Phase 2. It requires 6 cards of one color.\n";
            break;
        }
        case 3:
        {
            std::cout << "You are currently in Phase 3. It requires 1 run of 4 and 1 set of 4.\n";
            break;
        }
    }
}
