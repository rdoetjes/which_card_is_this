#ifndef H_DIVINATION
#define H_DIVINATION

#include <string>
#include "card.h"

class Trick{
    public:
        enum States { GET_COLOR_AND_SUIT, GET_VALUE_RANGE, GET_VALUE, TELL_THE_CARD };
        static std::string divination(std::string text, Trick::States *state, Card *card);
    private:
        static std::string GetColor(std::string text, Trick::States *state, Card *card);
        static std::string GetValueRange(std::string text, Trick::States *state, Card *card);
        static std::string GetValue(std::string text, Trick::States *state, Card *card);
};
#endif