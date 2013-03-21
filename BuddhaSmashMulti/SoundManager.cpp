#include "SoundManager.h"
#include <stdlib.h>

struct sample {
       Uint8 *data;
       Uint32 dpos;
       Uint32 dlen;
} soundss[NUM_SOUNDS];

void mixaudio(void *unused, Uint8 *stream, int len)
{
    int i;
    Uint32 amount;

    for ( i=0; i<NUM_SOUNDS; ++i ) {
        amount = (soundss[i].dlen-soundss[i].dpos);
        if ( amount > len ) {
            amount = len;
        }
        SDL_MixAudio(stream, &soundss[i].data[soundss[i].dpos], amount,    SDL_MIX_MAXVOLUME);
        soundss[i].dpos += amount;
    }
}


//-------------------------------------------------------------------------------------
SoundManager::SoundManager(void)
{
    extern void mixaudio(void *unused, Uint8 *stream, int len);
    /* Set 16-bit stereo audio at 22Khz */
    fmt.freq = 22050;
    fmt.format = AUDIO_S16;
    fmt.channels = 2;
    fmt.samples = 512;        /* A good value for games */
    fmt.callback = mixaudio;
    fmt.userdata = NULL;
}
//-------------------------------------------------------------------------------------
SoundManager::~SoundManager(void)
{
}


void SoundManager::open(void){
    if ( SDL_OpenAudio(&fmt, NULL) < 0 ) {
        fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
        exit(1);
    }
}

void SoundManager::turnOn(void){
    SDL_PauseAudio(0);
}

void SoundManager::turnOff(void){
    SDL_PauseAudio(1);
}

void SoundManager::close(void){
    SDL_CloseAudio();
}

void SoundManager::PlaySound(char *file)
{
    int index;
    SDL_AudioSpec wave;
    Uint8 *data;
    Uint32 dlen;
    SDL_AudioCVT cvt;

    /* Look for an empty (or finished) sound slot */
    for ( index=0; index<NUM_SOUNDS; ++index ) {
        if ( soundss[index].dpos == soundss[index].dlen ) {
            break;
        }
    }
    if ( index == NUM_SOUNDS )
        return;

    /* Load the sound file and convert it to 16-bit stereo at 22kHz */
    if ( SDL_LoadWAV(file, &wave, &data, &dlen) == NULL ) {
        fprintf(stderr, "Couldn't load %s: %s\n", file, SDL_GetError());
        return;
    }
    SDL_BuildAudioCVT(&cvt, wave.format, wave.channels, wave.freq,
                            AUDIO_S16,   2,             22050);
    cvt.buf = (Uint8*)malloc(dlen*cvt.len_mult);
    memcpy(cvt.buf, data, dlen);
    cvt.len = dlen;
    SDL_ConvertAudio(&cvt);
    SDL_FreeWAV(data);

    /* Put the sound data in the slot (it starts playing immediately) */
    if ( soundss[index].data ) {
        free(soundss[index].data);
    }
    SDL_LockAudio();
    soundss[index].data = cvt.buf;
    soundss[index].dlen = cvt.len_cvt;
    soundss[index].dpos = 0;
    SDL_UnlockAudio();
}





