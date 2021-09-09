#include "trick.h"
#include <iostream>
#include <regex>
#include <stdlib.h>

/*
Person wil ask the computer a question, revealing the suit and implicetely the colour
    -"Tell me" indicates to the computer it is red heart
    -"Tell us" indicates it is a red diamonds
    -"Now tell me" indicates it is a black club
    -"Now tell us" indicates it is a black spade
*/
std::string Trick::GetColor(std::string text, Trick::States *state, Card *card){    
    std::regex hearts("Tell me*", std::regex_constants::ECMAScript | std::regex_constants::icase);
    std::regex diamonds("Tell us*", std::regex_constants::ECMAScript | std::regex_constants::icase);

    std::regex clubs("Now Tell me*", std::regex_constants::ECMAScript | std::regex_constants::icase);
    std::regex spades("Now Tell us*", std::regex_constants::ECMAScript | std::regex_constants::icase);

    Trick::States nextState = Trick::States::GET_VALUE_RANGE;

    //Order is importend, we should firdt match the sentecnes that start with "Now"!!!
    if (std::regex_search(text, clubs)) {
        card->Color="Black";
        card->Suit="Club";
        *state=nextState;
        return "My neurons tell me it is a "+card->Color+" card";
    }

    if (std::regex_search(text, spades)) {
        card->Color="Black";
        card->Suit="Spade";
        *state=nextState;
        return "Something tells me it is a "+card->Color+" card";
    }

    if (std::regex_search(text, hearts)) {
        card->Color="Red";
        card->Suit="Heart";
        *state=nextState;        
        return "The tickle in my testicles reveal to me that it is a "+card->Color+" card";
    }

    if (std::regex_search(text, diamonds)) {
        card->Color="Red";
        card->Suit="Diamond";
        *state=nextState;        
        return "My spider sense tells me it is a "+card->Color+" card";
    }

    return "";
}

/*
    The computer will say: 
                            -"It is a <color> card"
    The person will answer: 
                            -"yes" for values Ace,2,3,4 
                            -"yes it is" for values 5,6,7,8
                            -"correct" for values 9, 10, jack, queen, 
                            -"that is correct" for the  (king)
*/
std::string Trick::GetValueRange(std::string text, Trick::States *state, Card *card){
    std::regex one_to_four("yes*", std::regex_constants::ECMAScript | std::regex_constants::icase);
    std::regex five_to_eight("yes it is*", std::regex_constants::ECMAScript | std::regex_constants::icase);
    std::regex nine_to_queen("correct*", std::regex_constants::ECMAScript | std::regex_constants::icase);
    std::regex king("[that is|you are] correct*", std::regex_constants::ECMAScript | std::regex_constants::icase);

    Trick::States nextState = Trick::States::GET_VALUE;
    
    std::string reply = "The card you are holding is a "+card->Suit+"!?";

    //Order is important to avoid partial matches of the text ("yes it is" needs to be evaluated before "yes")
    if (std::regex_search(text,five_to_eight)) {
        card->FourCardBlockNr = 1;
        *state=nextState;
        return reply;
    }

    if (std::regex_search(text,king)) {
        card->FourCardBlockNr = 3;
        *state=nextState;
        return reply;
    }

    if (std::regex_search(text,one_to_four)) {
        card->FourCardBlockNr = 0;
        *state=nextState;
        return reply;
    }

    if (std::regex_search(text,nine_to_queen)) {
        card->FourCardBlockNr = 2;
        *state=nextState;
        return reply;
    }

    return "I have no fuckin idea!";
}

std::string Trick::GetValue(std::string text, Trick::States *state, Card *card){
    std::cout << "Reveal the card\'n";

    //The CardRangeBlockNr and the Index
    std::string CardValues[4][4] = {
        {"Ace", "Two", "Three", "Four"},
        {"Five", "Six", "Seven", "Eight"},
        {"Nine", "Ten", "Jack", "Queen"},
        {"King", "King", "King", "King"}
    };

    std::regex one_to_four("yes*", std::regex_constants::ECMAScript | std::regex_constants::icase); //first item in the nested array
    std::regex five_to_eight("yes it is*", std::regex_constants::ECMAScript | std::regex_constants::icase); //second item in the nested array
    std::regex nine_to_queen("correct*", std::regex_constants::ECMAScript | std::regex_constants::icase); //third item in the second array
    std::regex king("[that is|you are] correct*", std::regex_constants::ECMAScript | std::regex_constants::icase); //fourth eitem in the second array

    Trick::States nextState = Trick::States::GET_COLOR_AND_SUIT;
    
    //Order is important to avoid partial matches of the text ("yes it is" needs to be evaluated before "yes")
    if (std::regex_search(text,five_to_eight)){
        *state = nextState;
        return "Your card is the "+CardValues[card->FourCardBlockNr][1]+" of "+card->Suit+"s";
    }

    if (std::regex_search(text,king)) {
        *state = nextState;
        return "Your card can only be the "+CardValues[card->FourCardBlockNr][3]+" of "+card->Suit+"s";
    }

    if (std::regex_search(text,one_to_four)) {
                std::cout << "okay";

        *state = nextState;
        return "Your card must be the "+CardValues[card->FourCardBlockNr][0]+" of "+card->Suit+"s";
    }

    if (std::regex_search(text,nine_to_queen)) {
        *state = nextState;
        return "The card your are holding is the "+CardValues[card->FourCardBlockNr][2]+" of "+card->Suit+"s";
    }

    //We should never get here
    return "I'll be fucked if I knew!";
}


/*This step will verbally encode the color and the suit
    The person says: Tell me = hearts, Tell us = diamands, Now Tell me = clubs, Now Tell us = diamonds
    The computer will then say: 
                            -"It is a <color> card"
    The person will answer: 
                            -"yes" for values Ace,2,3,4 
                            -"yes it is" for values 5,6,7,8
                            -"correct" for values 9, 10, jack, queen, 
                            -"that is correct" for the  (king)
    The computer will say: 
                            -"It is a <suit>"
    The person will anwer: 
                            -yes for 1st item 
                            -yes it is, 2nd item
                            -correct 3rd item
                            -that is correct 4 item
*/
std::string Trick::divination(std::string text, Trick::States *state, Card *card){
    std::cout << "state: " << *state << " text: " << text << std::endl;
    
    if (*state==Trick::States::GET_COLOR_AND_SUIT) return Trick::GetColor(text, state, card);
    
    if (*state==Trick::States::GET_VALUE_RANGE) return Trick::GetValueRange(text, state, card);

    if (*state==Trick::States::GET_VALUE) return Trick::GetValue(text, state, card);

    return "";
}