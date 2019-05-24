/*
  Copyright (c) 2019 blaargh
  cards.cpp
  https://github.com/blaargh/phase10
*/

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
    playerPhase = 8;
    computerPhase = 1;
}

void cards::createStack()
{
    cardStruct stackCards;
    for(int j = 0; j < 2; j++)
     for(int x = 0; x < 4; x++)
         for(int i = 0; i < 12; i++)
         {
             cardStack.push_back(std::make_pair(stackCards.cardValue.at(i), stackCards.cardColor.at(x)));
         }

  //on macOS, use
  //shuffle(cardStack.begin(), cardStack.end(), std::default_random_engine(rand()));

    std::random_shuffle(cardStack.begin(), cardStack.end());
}

void cards::giveInitialCards()
{
    std::vector<std::pair<int, std::string>> *cardPointer;

    if(currentPlayer == 0)
    {
        cardPointer = &playerCards;
    }
    else
        cardPointer = &computerCards;

    for(int i = 0; i < 10; i++)
    {
        cardPointer->push_back(cardStack.back());
        cardStack.pop_back();
    }
    updateCardProperties();
}

void cards::drawCard()
{
    std::vector<std::pair<int, std::string>> *cardPointer;
    int choice;

    if(currentPlayer == 0)
    {
        cardPointer = &playerCards;
        if(discardStack.empty())
        {
            std::cout << "\nSorry, no cards on the discard stack at the moment! Card will be drawn from normal stack\n";
            cardPointer->push_back(cardStack.back());
            cardStack.pop_back();
        }
        else
        {
            do
            {
                std::cout << "\n\nThere's currently a " << discardStack.back().first << " " << discardStack.back().second << " on the discard stack.\n";
                std::cout << "\nDraw card from normal stack or from discard stack?\n(1 for normal, 2 for discard stack): \n";
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
                         cardPointer->push_back(cardStack.back());
                         cardStack.pop_back();
                         break;
                    }
                    case 2:
                    {
                         cardPointer->push_back(discardStack.back());
                         discardStack.pop_back();
                         break;
                    }
                }
            } while(choice != 1 && choice != 2);
        }
        std::cout << "\nYou drew a " << playerCards.at(playerCards.size()-1).first << " " << playerCards.at(playerCards.size()-1).second << '\n';

    }
    else
    {
        cardPointer = &computerCards;
        cardPointer->push_back(cardStack.back());
        cardStack.pop_back();
    }

    updateCardProperties();
}

bool cards::checkPhase()
{
    updateCardProperties();

    switch(playerPhase)
    {
        case 1: // 4 sets of 2
        {
            std::vector<int> *cardPointer;
            int p = 0;
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

            std::sort(cardPointer->begin(), cardPointer->end());

            for(auto &i : *cardPointer)
            {
                auto result = countMap.insert(std::pair<int, int>(i, 1));
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
        case 5: // 7 cards of one color
        {
            std::vector<std::string> *cardPointer;
            std::string color;
            int p = 0;
            int limit = 0;
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

            if(playerPhase == 2)
                limit = 6;
            else
                limit = 7;

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
                if(i.second >= limit)
                {
                    color = i.first;
                    p = i.second;
                }
            }
            if(p >= limit)
            {
                if(currentPlayer == 0)
                {
                    if(playerPhaseOut == false)
                    {
                        if(!playerPhaseCards.empty())
                        {
                            return true;
                        }
                        else
                        {
                            std::cout << "Phase " << playerPhase <<" done! Your current amount of " << color << " is: " << p << "\n\n";
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
                    std::cout << "The computer finished phase " << computerPhase << ".\n";
                return true;
            }
            else
            {
                if(currentPlayer == 0)
                {
                    if(playerPhaseOut == false)
                    {
                        if(!playerPhaseCards.empty())
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
                    std::cout << "The computer couldn't finish phase " << computerPhase << "!\n\n";
                return false;
            }
            break;
        }

        case 3: // 1 set of 4 + 1 run of 4
        {
            std::vector<int> *cardPointer;
            std::vector<int> intUniques;
            std::vector<int> intTwins;
            std::map<int, int> countMap;
            if(currentPlayer == 0)
            {
                if(playerPhaseOut == false && playerPhaseCards.empty())
                    cardPointer = &playerCardValues;
                else
                    cardPointer = &playerPhaseCardValues;
            }
            else
                cardPointer = &computerCardValues;

            for(auto &i : *cardPointer)
            {
                intTwins.push_back(i);
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
                        if(!playerPhaseCards.empty())
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
                        if(!playerPhaseCards.empty())
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
        case 6: // 1 run of 9
        {
            std::vector<int> *cardPointer;
            std::vector<int> intUniques;
            std::vector<int> intTwins;
            std::map<int, int> countMap;
            int limit = 0;
            if(currentPlayer == 0)
            {
                if(playerPhaseOut == false && playerPhaseCards.empty())
                    cardPointer = &playerCardValues;
                else
                    cardPointer = &playerPhaseCardValues;
            }
            else
                cardPointer = &computerCardValues;

            if(playerPhase == 4)
                limit = 8;
            else
                limit = 9;

            for(auto &i : *cardPointer)
            {
                intTwins.push_back(i);
            }
            std::sort(intTwins.begin(), intTwins.end());
            std::unique_copy(intTwins.begin(), intTwins.end(), std::back_inserter(intUniques));

            int j = 0;
            int k = 0;

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
            if(k>=limit)
            {
                if(currentPlayer == 0)
                {
                    if(playerPhaseOut == false)
                    {
                        if(!playerPhaseCards.empty())
                        {
                            return true;
                        }
                        else
                        {
                            std::cout << "Phase " << playerPhase << " done!\n";
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
                    std::cout << "The computer finished phase " << computerPhase << ".\n";
                return true;
            }
            else
            {
                if(currentPlayer == 0)
                {
                    if(playerPhaseOut == false)
                    {
                        if(!playerPhaseCards.empty())
                            return false;
                        else
                            std::cout << "You have not finished the run. You currently have " << k << " consecutive cards. Keep collecting.\n";
                    }
                    else
                    {
                        std::cout << "These are your laid out cards: \n";
                            getPlayerCards(1);
                    }
                }
                else
                    std::cout << "The computer couldn't finish phase " << computerPhase << ".\n";
                return false;
            }
            break;
        }
        case 7: // 2 sets of 4
        case 9: // 1 set of 5 + 1 set of 3
        {
            std::vector<int> *cardPointer;
            int p = 0;
            bool countTwins = false;
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

            std::sort(cardPointer->begin(), cardPointer->end());

            for(auto &i : *cardPointer)
            {
                auto result = countMap.insert(std::pair<int, int>(i, 1));
                if(result.second == false)
                {
                    result.first->second++;
                }
            }
            for(auto &i : countMap)
            {
                if(playerPhase == 7)
                {
                    if(i.second == 4)
                        p++;
                    if(i.second == 8)
                        p = 2;
                    if(i.second > 4 && i.second < 8)
                        p = 1;
                }
                else
                {
                    if(i.second == 5)
                        p++;
                    if(i.second == 3 && countTwins == false)
                    {
                        p++;
                        countTwins = true;
                    }
                }
            }

            if(p == 2)
            {
                if(currentPlayer == 0)
                {
                    if(playerPhaseOut == false)
                    {
                        if(!playerPhaseCards.empty())
                        {
                            return true;
                        }
                        else
                        {
                            std::cout << "Phase " << playerPhase << " done!\n";
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
                    std::cout << "The computer finished phase 7.\n";
                return true;
            }
            else
            {
                if(currentPlayer == 0)
                {
                    if(playerPhaseOut == false)
                    {
                        if(!playerPhaseCards.empty())
                            return false;
                        else
                            std::cout << "You have not finished the sets. You currently have " << p << " complete set(s)! Keep collecting.\n";
                    }
                    else
                    {
                        std::cout << "These are your laid out cards: \n";
                            getPlayerCards(1);
                    }
                }
                else
                    std::cout << "The computer couldn't finish phase " << computerPhase << ".\n";
                return false;
            }
            break;
        }

        case 8: // 1 run of 4 of one color + 1 set of 3
        case 10: // 1 set of 5 + 1 run of 3 of one color
        {
            std::vector<int> intTwins;
            std::map<int, int> countMap;
            std::vector<std::vector<std::pair<int, std::string>>> vec_2d;
            std::vector<std::pair<int, std::string>> bluePairs, blueUniques, greenPairs, greenUniques, redPairs, redUniques, yellowPairs, yellowUniques, twinPairs, locPlayerCards, *cardPointer;
            int k = 0, l = 0, p = 0, m = 0, limit_run = 0, limit_set = 0;
            bool run = false;
            bool set = false;

            if(playerPhase == 8)
            {
                limit_run = 4;
                limit_set = 3;
            }
            else
            {
                limit_run = 3;
                limit_set = 5;
            }

            if(currentPlayer == 0)
            {
                if(playerPhaseOut == false && playerPhaseCards.empty())
                    cardPointer = &playerCards;
                else
                    cardPointer = &playerPhaseCards;
            }
            else
                cardPointer = &computerCards;

            for(unsigned int i = 0; i < cardPointer->size(); i++)
                locPlayerCards.push_back(cardPointer->at(i));

            for(unsigned int i = 0; i < locPlayerCards.size(); i++)
            {
                if(locPlayerCards.at(i).second == "blue")
                {
                    bluePairs.push_back(std::make_pair(locPlayerCards.at(i).first, locPlayerCards.at(i).second));
                }
                if(locPlayerCards.at(i).second == "green")
                {
                    greenPairs.push_back(std::make_pair(playerCards.at(i).first, locPlayerCards.at(i).second));
                }
                if(locPlayerCards.at(i).second == "red")
                {
                    redPairs.push_back(std::make_pair(locPlayerCards.at(i).first, locPlayerCards.at(i).second));
                }
                if(locPlayerCards.at(i).second == "yellow")
                {
                    yellowPairs.push_back(std::make_pair(locPlayerCards.at(i).first, locPlayerCards.at(i).second));
                }
            }
            std::unique_copy(bluePairs.begin(), bluePairs.end(), std::back_inserter(blueUniques));
            std::unique_copy(yellowPairs.begin(), yellowPairs.end(), std::back_inserter(yellowUniques));
            std::unique_copy(redPairs.begin(), redPairs.end(), std::back_inserter(redUniques));
            std::unique_copy(greenPairs.begin(), greenPairs.end(), std::back_inserter(greenUniques));

            std::sort(blueUniques.begin(), blueUniques.end());
            std::sort(greenUniques.begin(), greenUniques.end());
            std::sort(redUniques.begin(), redUniques.end());
            std::sort(yellowUniques.begin(), yellowUniques.end());

            bluePairs.clear();
            yellowPairs.clear();
            redPairs.clear();
            greenPairs.clear();

            if(blueUniques.size() > 3)
                vec_2d.push_back(blueUniques);
            if(greenUniques.size() > 3)
                vec_2d.push_back(greenUniques);
            if(redUniques.size() > 3)
                vec_2d.push_back(redUniques);
            if(yellowUniques.size() > 3)
                vec_2d.push_back(yellowUniques);

            std::sort(vec_2d.begin(), vec_2d.end());

            for(unsigned int i = 0; i < vec_2d.size(); i++)
            {
                if(vec_2d.at(i).size() >= 3)
                {
                    for(unsigned int j = 0; j < vec_2d.at(i).size(); j++)
                    {
                           if(j == vec_2d.at(i).size()-1)
                           {
                               if(vec_2d.at(i).at(j-1).first+1 == vec_2d.at(i).at(j).first)
                               {
                                   l++;
                                   if(l>k)
                                       k=l;
                                   if(k == 4)
                                   {
                                       while(m < 4)
                                       {
                                            twinPairs.push_back(vec_2d.at(i).at(j-m));
                                            m++;
                                       }
                                   }
                               }
                           }
                           else
                           {
                               if(j == 0)
                               {
                                   if(vec_2d.at(i).at(j).first+1 == vec_2d.at(i).at(j+1).first)
                                   {
                                       l++;
                                       if(l>k)
                                           k=l;
                                   }
                               }
                               else
                               {
                                   if((vec_2d.at(i).at(j+1).first-vec_2d.at(i).at(j).first == 1) || (vec_2d.at(i).at(j).first-vec_2d.at(i).at(j-1).first == 1))
                                   {
                                       l++;
                                       if(l>k)
                                           k=l;
                                       if(k == 4)
                                       {
                                           while(m < 4)
                                           {
                                                twinPairs.push_back(vec_2d.at(i).at(j-m));
                                                m++;
                                           }
                                       }
                                   }
                                   if((vec_2d.at(i).at(j+1).first-vec_2d.at(i).at(j).first != 1) || (vec_2d.at(i).at(j).first-vec_2d.at(i).at(j-1).first != 1))
                                   {
                                       if(l>k)
                                           k=l;
                                       l = 1;
                                   }
                               }
                           }
                        }
                    if(l>k)
                        k=l;
                    l = 1;
                }
            }
            if(!twinPairs.empty())
            {
                for(unsigned int i = 0; i < twinPairs.size(); i++)
                    locPlayerCards.erase(std::remove(locPlayerCards.begin(), locPlayerCards.end(), twinPairs.at(i)), locPlayerCards.end());
            }

            for(unsigned int i = 0; i < locPlayerCards.size(); i++)
                intTwins.push_back(locPlayerCards.at(i).first);

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
                if(p < i.second)
                {
                    p=i.second;
                }
            }
            std::cout << "k: " << k << '\n';
            std::cout << "p: " << p << '\n';
            if(k>=limit_run)
                run = true;
            if(p>=limit_set)
                set = true;

            if(run == true && set == true)
            {
                if(currentPlayer == 0)
                {
                    if(playerPhaseOut == false)
                    {
                        if(!playerPhaseCards.empty())
                        {
                            return true;
                        }
                        else
                        {
                            std::cout << "Phase " << playerPhase << " done!\n";
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
                    std::cout << "The computer finished phase " << computerPhase << ".\n";
                return true;
            }
            else
            {
                if(currentPlayer == 0)
                {  
                    if(playerPhaseOut == false)
                    {
                        if(!playerPhaseCards.empty())
                            return false;
                        else
                            std::cout << "One or more of your phases requirments are not met. Keep collecting.\n";
                    }
                    else
                    {
                        std::cout << "These are your laid out cards: \n";
                        getPlayerCards(1);
                    }
                }
                else
                    std::cout << "The computer couldn't finish phase " << computerPhase << ".\n";
                return false;
            }
            break;
        }
    }

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
            unsigned int limit = 0;
            std::vector<int> cardPositions;
            std::cout << "Choose the cards you want to lay out for your current phase\n";
            getPlayerCards(0);
            std::cout << "Enter all card positions to lay out like this: \"1 2 3 4 5\" etc.\nCard Positions: ";
            getchar();
            std::getline(std::cin, line);
            std::stringstream ss(line);

            if(playerPhase == 1 || playerPhase == 3 || playerPhase == 4 || playerPhase == 7 || playerPhase == 9 || playerPhase == 10)
                limit = 8;
            if(playerPhase == 6)
                limit = 9;
            if(playerPhase == 2)
                limit = 6;
            if(playerPhase == 5 || playerPhase == 8)
                limit = 7;

            while(!ss.eof())
            {
                ss >> number;
                if(number <= 0 || number > 11)
                {
                    std::cout << "One or more of the card positions you entered are not existing, please select option 2 again and re-enter your cards.\n";
                    return;
                }
                else
                {
                    playerPhaseCards.push_back(playerCards.at(number-1));
                    cardPositions.push_back(number-1);
                }
            }

            updateCardProperties();

            if(checkPhase() == true && playerPhaseCards.size() == limit)
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
    if(playerPhaseOut == true)
    {
        std::cout << "Enter the card position you want to add to your phase\n";
        getPlayerCards(0);
        std::cout << "Card position: ";
        if(!(std::cin >> c))
        {
            std::cout << "Please select one of your cards.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    else
    {
        std::cout << "You have not finished your phase yet, keep collecting\n";
        return false;
    }
    c--;
    if(c > playerCards.size()-1)
    {
        std::cout << "The card position you entered is not valid.\n";
        return false;
    }
    switch(playerPhase)
    {
        case 1:
        case 7:
        case 9:
        {
            std::vector<int> *pPhaseCards,*pPlayerCards;
            if(currentPlayer == 0)
            {
                pPhaseCards = &playerPhaseCardValues;
                pPlayerCards = &playerCardValues;
            }
            else
            {
                pPhaseCards = &computerPhaseCardValues;
                pPlayerCards = &computerCardValues;
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
                    std::cout << "You added " << playerPhaseCards.at(playerPhaseCards.size()-1).first << " " << playerPhaseCards.at(playerPhaseCards.size()-1).second << " to your laid out phase!\nOnly " << playerCards.size()-1 << " more card(s) to get rid of to win this round!\n";
                    playerCards.erase(std::remove(playerCards.begin(), playerCards.end(), playerCards.at(c)), playerCards.end());
                    return true;
                }
                else
                {
                    std::cout << "Can't add " << playerCards.at(c).first << " " << playerCards.at(c).second << " to your phase on the board, it doesnt match the sets!\n";
                    return false;
                }
            }
            break;
        }
        case 2:
        case 5:
        {
            std::vector<std::string> *pPhaseCards,*pPlayerCards;
            if(currentPlayer == 0)
            {
                pPhaseCards = &playerPhaseCardColors;
                pPlayerCards = &playerCardColors;
            }
            else
            {
                pPhaseCards = &computerPhaseCardColors;
                pPlayerCards = &computerCardColors;
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
                    std::cout << "You added " << playerPhaseCards.at(playerPhaseCards.size()-1).first << " " << playerPhaseCards.at(playerPhaseCards.size()-1).second << " to your laid out phase!\nOnly " << playerCards.size()-1 << " more card(s) to get rid of to win this round!\n";
                    playerCards.erase(std::remove(playerCards.begin(), playerCards.end(), playerCards.at(c)), playerCards.end());
                    return true;
                }
                else
                {
                    std::cout << "Can't add " << playerCards.at(c).first << " " << playerCards.at(c).second << " to your phase on the board, it doesnt match the color!\n";
                    return false;
                }
            }
            break;
        }
        case 3:
        case 4:
        case 6:
        {
            std::map<int, int> countMap;
            std::vector<int> intPlayerCards, intUniques, intTwins;
            int k = 0;
            for(auto &i : playerPhaseCardValues)
            {
                intUniques.push_back(i);
            }
            for(auto &i : playerCardValues)
            {
                intPlayerCards.push_back(i);
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

            if(std::find(intTwins.begin(), intTwins.end(), playerCards.at(c).first) != intTwins.end())
            {
                if(playerCards.size() == 1)
                {
                    playerCards.erase(std::remove(playerCards.begin(), playerCards.end(), playerCards.at(c)), playerCards.end());
                    return true;
                }
                else
                {
                    playerPhaseCards.push_back(playerCards.at(c));
                    std::cout << "You added " << playerPhaseCards.at(playerPhaseCards.size()-1).first << " " << playerPhaseCards.at(playerPhaseCards.size()-1).second << " to your laid out set!\nOnly " << playerCards.size()-1 << " more card(s) to get rid of to win this round!\n";
                    playerCards.erase(std::remove(playerCards.begin(), playerCards.end(), playerCards.at(c)), playerCards.end());
                    return true;
                }
            }
            else
            {
                if(intPlayerCards.at(c) == 1)
                {
                    if(std::find(intUniques.begin(), intUniques.end(), intPlayerCards.at(c)+1) != intUniques.end())
                    {
                        if(std::find(intUniques.begin(), intUniques.end(), intPlayerCards.at(c)) != intUniques.end())
                        {
                            std::cout << "Can't add " << playerCards.at(c).first << " " << playerCards.at(c).second << "  to your phase on the board, it already exists in the run!\n";
                            return false;
                        }
                        else
                        {
                            if(playerCards.size() == 1)
                            {
                                playerCards.erase(std::remove(playerCards.begin(), playerCards.end(), playerCards.at(c)), playerCards.end());
                                return true;
                            }
                            else
                            {
                                playerPhaseCards.push_back(playerCards.at(c));
                                std::cout << "You added " << playerPhaseCards.at(playerPhaseCards.size()-1).first << " " << playerPhaseCards.at(playerPhaseCards.size()-1).second << " to your laid out run!\nOnly " << playerCards.size()-1 << " more card(s) to get rid of to win this round!\n";
                                playerCards.erase(std::remove(playerCards.begin(), playerCards.end(), playerCards.at(c)), playerCards.end());
                                return true;
                            }
                        }
                    }
                    else
                    {
                        std::cout << "Can't add " << playerCards.at(c).first << " " << playerCards.at(c).second << " to your phase on the board, it doesnt match either the set or the run!\n";
                        return false;
                    }
                }
                if(intPlayerCards.at(c) != 1)
                {
                    if(std::find(intUniques.begin(), intUniques.end(), intPlayerCards.at(c)-1) != intUniques.end() || std::find(intUniques.begin(), intUniques.end(), intPlayerCards.at(c)+1) != intUniques.end())
                    {
                        if(std::find(intUniques.begin(), intUniques.end(), intPlayerCards.at(c)) != intUniques.end())
                        {
                            std::cout << "Can't add " << playerCards.at(c).first << " " << playerCards.at(c).second << " to your phase on the board, it already exists in the run!\n";
                            return false;
                        }
                        else
                        {
                            if(playerCards.size() == 1)
                            {
                                playerCards.erase(std::remove(playerCards.begin(), playerCards.end(), playerCards.at(c)), playerCards.end());
                                return true;
                            }
                            else
                            {
                                playerPhaseCards.push_back(playerCards.at(c));
                                std::cout << "You added " << playerPhaseCards.at(playerPhaseCards.size()-1).first << " " << playerPhaseCards.at(playerPhaseCards.size()-1).second << " to your laid out run!\nOnly " << playerCards.size()-1 << " more card(s) to get rid of to win this round!\n";
                                playerCards.erase(std::remove(playerCards.begin(), playerCards.end(), playerCards.at(c)), playerCards.end());
                                return true;
                            }
                        }
                    }
                    else
                    {
                        std::cout << "Can't add " << playerCards.at(c).first << " " << playerCards.at(c).second << " to your phase on the board, it doesnt match either the set or the run!\n";
                        return false;
                    }
                }
            }
        break;
        }
        case 8:
        case 10:
        {
            std::map<int, int> countMap;
            std::vector<std::pair<int, std::string>> tempPairs;
            std::vector<int> intTwins;
            int k = 0, l = 0;
            for(auto &i : playerPhaseCardValues)
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
                    while(std::find(playerPhaseCardValues.begin(), playerPhaseCardValues.end(), k) != playerPhaseCardValues.end() && l < i.second)
                    {
                        intTwins.push_back(k);
                        l++;
                    }
                }
            }
            for(unsigned int j = 0; j < playerPhaseCards.size(); j++)
            {
                if(playerPhaseCards.at(j).first != k)
                    tempPairs.push_back(playerPhaseCards.at(j));
            }

            if(std::find(intTwins.begin(), intTwins.end(), playerCards.at(c).first) != intTwins.end())
            {
                if(playerCards.size() == 1)
                {
                    playerCards.erase(std::remove(playerCards.begin(), playerCards.end(), playerCards.at(c)), playerCards.end());
                    return true;
                }
                else
                {
                    playerPhaseCards.push_back(playerCards.at(c));
                    std::cout << "You added " << playerPhaseCards.at(playerPhaseCards.size()-1).first << " " << playerPhaseCards.at(playerPhaseCards.size()-1).second << " to your laid out set!\nOnly " << playerCards.size()-1 << " more card(s) to get rid of to win this round!\n";
                    playerCards.erase(std::remove(playerCards.begin(), playerCards.end(), playerCards.at(c)), playerCards.end());
                    return true;
                }
            }
            else
            {
                if(playerCards.at(c).first == 1)
                {
                    if(std::find(playerPhaseCardValues.begin(), playerPhaseCardValues.end(), playerCards.at(c).first+1) != playerPhaseCardValues.end())
                    {
                        if(std::find(playerPhaseCardValues.begin(), playerPhaseCardValues.end(), playerCards.at(c).first) != playerPhaseCardValues.end())
                        {
                            std::cout << "Can't add " << playerCards.at(c).first << " " << playerCards.at(c).second << "  to your phase on the board, it already exists in the run!\n";
                            return false;
                        }
                        else
                        {
                            if(playerCards.at(c).second == tempPairs.at(0).second)
                            {
                                if(playerCards.size() == 1)
                                {
                                    playerCards.erase(std::remove(playerCards.begin(), playerCards.end(), playerCards.at(c)), playerCards.end());
                                    return true;
                                }
                                else
                                {
                                    playerPhaseCards.push_back(playerCards.at(c));
                                    std::cout << "You added " << playerPhaseCards.at(playerPhaseCards.size()-1).first << " " << playerPhaseCards.at(playerPhaseCards.size()-1).second << " to your laid out run!\nOnly " << playerCards.size()-1 << " more card(s) to get rid of to win this round!\n";
                                    playerCards.erase(std::remove(playerCards.begin(), playerCards.end(), playerCards.at(c)), playerCards.end());
                                    return true;
                                }
                            }
                            else
                            {
                                std::cout << "Can't add " << playerCards.at(c).first << " " << playerCards.at(c).second << " to your phase on the board, it doesnt match the color of the run!\n";
                                return false;
                            }
                        }
                    }
                    else
                    {
                        std::cout << "Can't add " << playerCards.at(c).first << " " << playerCards.at(c).second << " to your phase on the board, it doesnt match either the set or the run!\n";
                        return false;
                    }
                }
                if(playerCards.at(c).first != 1)
                {
                    if(std::find(playerPhaseCardValues.begin(), playerPhaseCardValues.end(), playerCards.at(c).first-1) != playerPhaseCardValues.end() || std::find(playerPhaseCardValues.begin(), playerPhaseCardValues.end(), playerCards.at(c).first+1) != playerPhaseCardValues.end())
                    {
                        if(std::find(playerPhaseCardValues.begin(), playerPhaseCardValues.end(), playerCards.at(c).first) != playerPhaseCardValues.end())
                        {
                            std::cout << "Can't add " << playerCards.at(c).first << " " << playerCards.at(c).second << " to your phase on the board, it already exists in the run!\n";
                            return false;
                        }
                        else
                        {
                            if(playerCards.at(c).second == tempPairs.at(0).second)
                            {
                                if(playerCards.size() == 1)
                                {
                                    playerCards.erase(std::remove(playerCards.begin(), playerCards.end(), playerCards.at(c)), playerCards.end());
                                    return true;
                                }
                                else
                                {
                                    playerPhaseCards.push_back(playerCards.at(c));
                                    std::cout << "You added " << playerPhaseCards.at(playerPhaseCards.size()-1).first << " " << playerPhaseCards.at(playerPhaseCards.size()-1).second << " to your laid out run!\nOnly " << playerCards.size()-1 << " more card(s) to get rid of to win this round!\n";
                                    playerCards.erase(std::remove(playerCards.begin(), playerCards.end(), playerCards.at(c)), playerCards.end());
                                    return true;
                                }
                            }
                            else
                            {
                                std::cout << "Can't add " << playerCards.at(c).first << " " << playerCards.at(c).second << " to your phase on the board, it doesnt match the color of the run!\n";
                                return false;
                            }
                        }
                    }
                    else
                    {
                        std::cout << "Can't add " << playerCards.at(c).first << " " << playerCards.at(c).second << " to your phase on the board, it doesnt match either the set or the run!\n";
                        return false;
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
    std::vector<std::pair<int, std::string>> *cardPointer, cardValue;
    if(currentPlayer == 0)
    {
        cardPointer = &playerCards;
        if(cardPointer->size() != 1)
            std::cout << "You discarded: " << cardPointer->at(pos-1).first << " " << cardPointer->at(pos-1).second << '\n';
        else
            std::cout << "You discarded your last card.\n";

    }
    else
    {
        cardPointer = &computerCards;
        std::cout << "The computer has discarded a card. Its turn is over.\n";
    }

    if(cardPointer->size() == 1)
    {
        cardPointer->clear();
    }
    else
    {
        discardStack.push_back(std::make_pair(cardPointer->at(pos-1).first, cardPointer->at(pos-1).second));
        cardPointer->erase(cardPointer->begin() + pos -1);
    }

}

void cards::initiateNextRound()
{
    if(playerCards.empty())
    {
        std::cout << "Congratulations, you won phase " << playerPhase << " and will move up to phase " << playerPhase+1 << ".\n";
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
    cardStack.clear();
}

void cards::updateCardProperties()
{
    if(currentPlayer == 0)
    {
        playerCardValues.clear();
        playerCardColors.clear();
        playerPhaseCardValues.clear();
        playerPhaseCardColors.clear();

        for(unsigned int i = 0; i < playerCards.size(); i++)
            playerCardValues.push_back(playerCards.at(i).first);

        for(unsigned int i = 0; i < playerCards.size(); i++)
            playerCardColors.push_back(playerCards.at(i).second);

        if(!playerPhaseCards.empty())
        {
            for(unsigned int i = 0; i < playerPhaseCards.size(); i++)
                playerPhaseCardValues.push_back(playerPhaseCards.at(i).first);

            for(unsigned int i = 0; i < playerPhaseCards.size(); i++)
                playerPhaseCardColors.push_back(playerPhaseCards.at(i).second);
        }
    }
    else
    {
        computerCardValues.clear();
        computerCardColors.clear();

        for(unsigned int i = 0; i < computerCards.size(); i++)
            computerCardValues.push_back(computerCards.at(i).first);

        for(unsigned int i = 0; i < computerCards.size(); i++)
            computerCardColors.push_back(computerCards.at(i).second);

        if(!computerPhaseCards.empty())
        {
            for(unsigned int i = 0; i < computerPhaseCards.size(); i++)
                computerPhaseCardValues.push_back(computerPhaseCards.at(i).first);

            for(unsigned int i = 0; i < playerPhaseCards.size(); i++)
                computerPhaseCardColors.push_back(computerPhaseCards.at(i).second);
        }
    }

}

void cards::getPlayerCards(int i)
{
    std::vector<std::pair<int, std::string>> *cardPointer;
    if(i == 0)
        cardPointer = &playerCards;
    else
        cardPointer = &playerPhaseCards;

    for(unsigned int i = 0; i < cardPointer->size(); i++)
    {
        if(i % 3 == 0)
            std::cout << '\n';

        std::cout << "\t\t(" << i+1 << ") " << cardPointer->at(i).first << " " << cardPointer->at(i).second << "\t";

    }
    std::cout << '\n';
}

std::vector<std::pair<int, std::string>> cards::getPlayerVector()
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
        case 4:
        {
            std::cout << "You are currently in Phase 4. It requires 1 run of 8 cards.\n";
            break;
        }
        case 5:
        {
            std::cout << "You are currently in Phase 5. It requires 7 cards of one color.\n";
            break;
        }
        case 6:
        {
            std::cout << "You are currently in Phase 6. It requires 1 run of 9 cards.\n";
            break;
        }
        case 7:
        {
            std::cout << "You are currently in Phase 7. It requires 2 sets of 4 cards.\n";
            break;
        }
        case 8:
        {
            std::cout << "You are currently in Phase 8. It requires 1 run of 4 of one color and 1 set of 3.\n";
            break;
        }
        case 9:
        {
            std::cout << "You are currently in Phase 9. Itr requires 1 set of 5 and 1 set of 3.\n";
            break;
        }
        case 10:
        {
            std::cout << "You are currently in Phase 10. It requires 1 set of 5 and 1 run of 3 of one color.\n";
            break;
        }


    }
}
