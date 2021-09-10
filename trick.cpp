#include "trick.h"
#include <iostream>
#include <regex>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include <cctype>


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
        return "My neurons tell me it is a "+card->Color+" card, is that correct?";
    }

    if (std::regex_search(text, spades)) {
        card->Color="Black";
        card->Suit="Spade";
        *state=nextState;
        return "Something tells me it is a "+card->Color+" card, is that correct?";
    }

    if (std::regex_search(text, hearts)) {
        card->Color="Red";
        card->Suit="Heart";
        *state=nextState;        
        return "My testicles reveal to me that it is a "+card->Color+" card, is that correct?";
    }

    if (std::regex_search(text, diamonds)) {
        card->Color="Red";
        card->Suit="Diamond";
        *state=nextState;        
        return "My spider sense tells me that it is a "+card->Color+" card, is that correct?";
    }

    return "";
}

/*
    Set the card blanc, this is when we are in the 
    GET_COLOR_AND_SUIT State
*/
void Trick::resetCard(Card *card){
    card->Color="Black";
    card->Suit="Spade";
    card->Value="King";
    card->FourCardBlockNr=3;
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
    std::regex five_to_eight("[yes|sure]?[,]?it is*", std::regex_constants::ECMAScript | std::regex_constants::icase);
    std::regex nine_to_queen("correct*", std::regex_constants::ECMAScript | std::regex_constants::icase);
    std::regex king("[that is|you are] correct*", std::regex_constants::ECMAScript | std::regex_constants::icase);
    std::regex wrong("no*", std::regex_constants::ECMAScript | std::regex_constants::icase);

    if (std::regex_search(text, wrong)) {
        card->FourCardBlockNr = 1;
        *state=Trick::States::GET_COLOR_AND_SUIT;
        return "Sorry let's try again";
    }

    Trick::States nextState = Trick::States::GET_VALUE;
    
    std::string reply = "Is the card that you are holding a "+card->Suit+"?";

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

    return "I have no fuckin idea! Do I look like a mindreader to you?";
}

/*
    We tell the computer which value to pick from the range, that we signaled before.
*/
std::string Trick::GetValue(std::string text, Trick::States *state, Card *card){
    static int iterations = 0;
    iterations++;
    //The CardRangeBlockNr and the Index
    std::string CardValues[4][4] = {
        {"Ace", "Two", "Three", "Four"},
        {"Five", "Six", "Seven", "Eight"},
        {"Nine", "Ten", "Jack", "Queen"},
        {"King", "King", "King", "King"}
    };

    std::regex one_to_four("yes*", std::regex_constants::ECMAScript | std::regex_constants::icase); //first item in the nested array
    std::regex five_to_eight("[yes|sure]?[,]?it is*", std::regex_constants::ECMAScript | std::regex_constants::icase); //second item in the nested array
    std::regex nine_to_queen("correct*", std::regex_constants::ECMAScript | std::regex_constants::icase); //third item in the second array
    std::regex king("[that is|you are] correct*", std::regex_constants::ECMAScript | std::regex_constants::icase); //fourth eitem in the second array

    Trick::States nextState = (iterations % 2 == 0) ? Trick::States::CYCLIC_STACK_QUESTION : Trick::States::GET_COLOR_AND_SUIT;
    
    //Order is important to avoid partial matches of the text ("yes it is" needs to be evaluated before "yes")
    if (std::regex_search(text,five_to_eight)){
        *state = nextState;
        card->Value = CardValues[card->FourCardBlockNr][1];
        return "Your card is the "+CardValues[card->FourCardBlockNr][1]+" of "+card->Suit+"s";
    }

    if (std::regex_search(text,king)) {
        *state = nextState;
        card->Value = CardValues[card->FourCardBlockNr][3];
        return "Your card can only be the "+CardValues[card->FourCardBlockNr][3]+" of "+card->Suit+"s";
    }

    if (std::regex_search(text,one_to_four)) {
        card->Value = CardValues[card->FourCardBlockNr][0];
        *state = nextState;
        return "Your card must be the "+CardValues[card->FourCardBlockNr][0]+" of "+card->Suit+"s";
    }

    if (std::regex_search(text,nine_to_queen)) {
        *state = nextState;
        card->Value = CardValues[card->FourCardBlockNr][2];
        return "The card your are holding is the "+CardValues[card->FourCardBlockNr][2]+" of "+card->Suit+"s";
    }

    //We should never get here
    return "I'll be fucked if I knew!";
}

/*
    Now the computer will ask for a random number between 1 and 52
*/
std::string Trick::Ask4RandomNumber(Trick::States *state){
    *state = Trick::CYCLIC_STACK_REVEAL;
    return "Please put the card back on top. And then give me a number between 1 and 52.";
}

/*
    Returns the card value code based on the card value
    for example Ace returns A (AC is for example ace of club in card code format)
*/
std::string Trick::CardValueToCardCode(std::string cardValue){
    std::map<std::string, std::string> cardValueToCardCode = { 
        {"Ace", "A"}, {"Two", "2"}, {"Three", "3"}, {"Four", "4"}, 
        {"Five", "5"}, {"Six", "6"}, {"Seven", "7"}, {"Eight", "8"}, 
        {"Nine", "9"}, {"Ten", "10"}, {"Jack", "J"}, {"Queen", "Q"},
        {"King", "K"}
    } ;

    return cardValueToCardCode[cardValue];
}

/*
    Takes the name of the suit of a card and returns the card code suit letter
    for example: 
        Clubs returns "C"
        Diamonds returns "D"
    The algorithm, just takes the first letter of the suit and captitalizes it
*/
std::string Trick::CardSuitToCardSuitCode(std::string cardSuite){
    std::string result = "";
    result = toupper(cardSuite[0]);
    return result;
}

/*
    A Card object will be converted to a card code.
    For example:
        Card->Value = "Jack"
        Card->Suit = "Diamonds"
        The result would be the card code: "JD"
*/
std::string Trick::FromCardToCardCode(Card *card){
    std::string result = CardValueToCardCode(card->Value) + CardSuitToCardSuitCode(card->Suit);
    return result;
}

/*
    Convert the suit code from a card code to a complete suit name
    For example: C returns Clubs
                 D returns Diamonds
                 S returns Spades
                 H returns hearts 
*/
std::string Trick::FromCardCodeSuitToCompleteSuit(std::string cardCodeSuiteLetter){
    if (cardCodeSuiteLetter=="C" or cardCodeSuiteLetter=="c") return "Clubs";
    if (cardCodeSuiteLetter=="H" or cardCodeSuiteLetter=="h") return "Hearts";
    if (cardCodeSuiteLetter=="S" or cardCodeSuiteLetter=="s") return "Spades";
    if (cardCodeSuiteLetter=="D" or cardCodeSuiteLetter=="s") return "Diamonds";

    return "";
}

/*
    Convert from a Card Code valye to a Card value
    for example:
        A becomes Aces
        10 becomes Ten
*/
std::string Trick::FromCardCodeValueToCardValue(std::string cardCodeValue){
     std::map<std::string, std::string> fromCardCodeValueToCardValue = { 
        {"A", "Ace"}, {"2", "Two"}, {"3", "Three"}, {"4", "Four"},
        {"5", "Five"}, {"6", "Six"}, {"7", "Seven"}, {"8", "Eight"},
        {"9", "Nine"}, {"10", "Ten"}, {"J", "Jack"}, {"Q", "Queen"},
        {"K", "King"}                
    } ;

    return fromCardCodeValueToCardValue[cardCodeValue];
}

/*
    Converts a card code, for example 10C to it's spoken format. 
    In case of 10C that would be: "10 of Clubs"
*/
std::string Trick::FromCardCodetoCard(std::string cardCode){
    return (cardCode.length() > 2) ? FromCardCodeValueToCardValue(cardCode.substr(0, 2)) + " of " + FromCardCodeSuitToCompleteSuit( cardCode.substr(2,1) ) : FromCardCodeValueToCardValue(cardCode.substr(0, 1)) + " of " + FromCardCodeSuitToCompleteSuit(cardCode.substr(1,1) );   
}

/*
    The text spoken by the person will be a number between 1 and 52
    The computer will then devine, which card is located at that number in the deck of cards.
    Which can be done because it's a Si Stebbins Stack
*/
std::string Trick::RevealCardAtPosition(std::string text, Trick::States *state, Card *card, const std::string cyclicCardStack[52]){
    std::stringstream ss;  
    unsigned int num;
    unsigned int cardLocationInStack;

    if (text == "") return "I asked for a number between 1 and 52!";

    ss << text;  
    ss >> num;  

    if (num < 1 and num > 52) return "You need to listen, I asked for a number between 1 and 52!";

    //find what the current offset is in the stack
    std::string cardCode = FromCardToCardCode(card);

    cardLocationInStack = 0;
    for (int i=0; i<52; i++){
        if (cyclicCardStack[i] == cardCode){
            cardLocationInStack = i;
            break;
        }
    }

    int index = cardLocationInStack + num -1;

    //restart the state machine, after this marvelous feat of mentalism
    *state = Trick::States::GET_COLOR_AND_SUIT;
    
    std::string ins = "Count down "+std::to_string(num)+" cards and you find the ";
    return std::string( (index > 51) ? ins+FromCardCodetoCard(cyclicCardStack[index-52]) : ins+FromCardCodetoCard(cyclicCardStack[index] ));
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
std::string Trick::divination(std::string text, Trick::States *state, Card *card, const std::string cyclicCardStack[52]){    
    if (*state==Trick::States::GET_COLOR_AND_SUIT) {
        Trick::resetCard(card);
        return Trick::GetColor(text, state, card);
    }

    if (*state==Trick::States::GET_VALUE_RANGE) return Trick::GetValueRange(text, state, card);

    if (*state==Trick::States::GET_VALUE) return Trick::GetValue(text, state, card);

    if (*state==Trick::States::CYCLIC_STACK_QUESTION) return Trick::Ask4RandomNumber(state);

    if (*state==Trick::States::CYCLIC_STACK_REVEAL) return Trick::RevealCardAtPosition(text, state, card, cyclicCardStack);        

    return "";
}
