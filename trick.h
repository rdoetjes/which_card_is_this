#ifndef H_DIVINATION
#define H_DIVINATION

#include <string>
#include "card.h"

class Trick{
    public:
        enum States { GET_COLOR_AND_SUIT, GET_VALUE_RANGE, GET_VALUE, TELL_THE_CARD, CYCLIC_STACK_QUESTION, CYCLIC_STACK_REVEAL };
        static std::string divination(std::string text, Trick::States *state, Card *card, const std::string cyclicCardStack[52]);
    private:
        static std::string GetColor(std::string text, Trick::States *state, Card *card);
        static std::string GetValueRange(std::string text, Trick::States *state, Card *card);
        static std::string GetValue(std::string text, Trick::States *state, Card *card);
        static std::string Ask4RandomNumber(Trick::States *state);
        static std::string RevealCardAtPosition(std::string text, Trick::States *state, Card *card, const std::string cyclicCardStack[52]);
        static std::string FromCardToCardCode(Card *card);
        static std::string toCardValueCode(std::string cardValue);
        static std::string toCardSuiteCode(std::string cardSuite);
        static std::string FromCardCodetoCard(std::string cardCode);
        static std::string FromCardCodeSuitToCompleteSuit(std::string cardCodeSuiteLetter);
        static void resetCard(Card *card);
};
#endif