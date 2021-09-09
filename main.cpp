#include <iostream> 
#include <speechapi_cxx.h>
#include <fstream>
#include "trick.h"
#include "cyclicstacks.h"

using namespace std;
using namespace Microsoft::CognitiveServices::Speech;

static Trick::States currentState = Trick::States::GET_COLOR_AND_SUIT;
static Card card;

int main(){
    promise<void> recognitionEnd;

    auto config = SpeechConfig::FromSubscription("f3f37dca61334fe9b5207079b71e5d95", "westeurope");
    config->SetSpeechSynthesisVoiceName("en-GB-RyanNeural");
    
    //open the speech synthezier
    auto speak = SpeechSynthesizer::FromConfig(config);

    //open a speech recognozer for text to speech
    auto recognizer = SpeechRecognizer::FromConfig(config);

    auto result = recognizer->RecognizeOnceAsync().get();
    while(1){
        std::string say = Trick::divination(result->Text, &currentState, &card, SiStebbingsStack);
        std::cout << say << std::endl;
        speak->SpeakTextAsync(say).get();        
        result = recognizer->RecognizeOnceAsync().get();
    }
    return 0;
}