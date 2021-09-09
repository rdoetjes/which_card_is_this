#ifndef H_DIVINATION
#define H_DIVINATION

#include <string>
#include "card.h"

class Trick{
    public:
        enum States { GET_COLOR_AND_SUIT, GET_VALUE_RANGE, GET_VALUE, TELL_THE_CARD, SI_STEBBINS_QUESTION, SI_STEBBINS_REVEAL };
        static std::string divination(std::string text, Trick::States *state, Card *card);
    private:
        static std::string GetColor(std::string text, Trick::States *state, Card *card);
        static std::string GetValueRange(std::string text, Trick::States *state, Card *card);
        static std::string GetValue(std::string text, Trick::States *state, Card *card);
        static std::string Ask4RandomNumber(Trick::States *state);
        static std::string SiStebbinsReveal(std::string text, Trick::States *state, Card *card);
        static std::string FromCardToCardCode(Card *card);
        static std::string toCardValueCode(std::string cardValue);
        static std::string toCardSuiteCode(std::string cardSuite);
        static std::string FromCardCodetoCard(std::string cardCode);
        static std::string FromCardCodeSuitToCompleteSuit(std::string cardCodeSuiteLetter);
        static void resetCard(Card *card);
};
#endif