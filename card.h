#include "string"

struct Card{
    std::string Color;
    
    std::string Suit;

    //0 = Ace, 2, 3, 4
    //1 5,6,7,8
    //2 9,10,jack queen
    //3 king
    int FourCardBlockNr = 0;
    
    std::string Value;
};
