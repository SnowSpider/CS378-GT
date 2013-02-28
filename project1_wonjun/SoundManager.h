#ifndef __SoundManager_h_
#define __SoundManager_h_



#include "SDL.h"
#include "SDL_audio.h"
#define NUM_SOUNDS 2


using namespace std;

   
class SoundManager{


    public:

    SDL_AudioSpec fmt;

    SoundManager(void);
    ~SoundManager(void);

    void open(void);
    void PlaySound(char *file);
    void turnOn(void);
    void turnOff(void);
    void close(void);
    
};





#endif // #ifndef __SoundManager_h_
