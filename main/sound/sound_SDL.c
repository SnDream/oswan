#include <stdio.h>
#include <SDL/SDL.h>
#include "WSHard.h"
#include "WSApu.h"
#include "shared.h"
#include "sound.h"

#warning "The SDL Sound code is ABSOLUTE COMPLETE CRAP ! Replace, fix, anything please to get rid of this mess !"

#ifdef SOUND_ON
static SDL_mutex *sound_mutex;
static SDL_cond *sound_cv;

static void SDL_callback(void *userdata, uint8_t *stream, int32_t len)
{
	int32_t i;
	uint16_t *buffer = (uint16_t *) stream;
	int rate, repeat;
	uint16_t LL, RR;
	
	/*
    if (len <= 0 || !buffer)
	{
		return;
	}
	*/

	// SDL_LockMutex(sound_mutex);

#ifndef SOUNDFILL_RATE
	int32_t apubuflack = len - (apuBufLen() & ~(1L));

	if (apubuflack > 0){
		rBuf -= (apubuflack < rBuf) ? apubuflack : apubuflack - SND_RNGSIZE;
	}
	len /= 2;
#else
	len /= 2;
	rate = len / (apuBufLen() + 1);
	if (rate == 0) {
#endif
		if (rBuf + len <= SND_RNGSIZE) {
			memcpy(buffer, sndbuffer + rBuf, len * sizeof(uint16_t));
			rBuf += len;
		} else {
			i = SND_RNGSIZE - rBuf;
			memcpy(buffer, sndbuffer + rBuf, i * sizeof(uint16_t));
			memcpy(buffer + i, sndbuffer, (len - i) * sizeof(uint16_t));
			rBuf = len - i;
		}
#ifdef SOUNDFILL_RATE
	} else {
		rate *= 2;
		len /= 2;
		for (i = 0, repeat = 0; i < len ; i++) {
			if (repeat-- == 0) {
				LL = sndbuffer[rBuf++];
				RR = sndbuffer[rBuf++];
				rBuf &= SND_RNGSIZE - 1;
				repeat = rate;
			}
			*(buffer++) = LL;
			*(buffer++) = RR;
		}
	}
#endif

	// SDL_UnlockMutex(sound_mutex);
	// SDL_CondSignal(sound_cv);
}
#endif

void Init_Sound()
{
#ifdef SOUND_ON
	SDL_InitSubSystem(SDL_INIT_AUDIO);
	SDL_AudioSpec fmt, retFmt;
	
	#ifdef NATIVE_AUDIO
	fmt.freq = 12000;
	#else
	fmt.freq = 44800;
	#endif
	fmt.samples = SND_BNKSIZE;
	fmt.format = AUDIO_S16SYS;
	fmt.channels = 2;
	fmt.callback = SDL_callback;
	fmt.userdata = NULL;

    if ( SDL_OpenAudio(&fmt, &retFmt) < 0 )
	{
        fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
        printf("Exiting Oswan...\n");
        //exit(1);
    }
    
	// sound_mutex = SDL_CreateMutex();
	// sound_cv = SDL_CreateCond();
#endif
}

void Pause_Sound()
{
#ifdef SOUND_ON
	SDL_PauseAudio(1);
#endif
}

void Resume_Sound()
{
#ifdef SOUND_ON
	SDL_PauseAudio(0);
#endif
}

void Cleanup_Sound()
{
#ifdef SOUND_ON
	SDL_PauseAudio(1);
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
#endif
}

void Sound_APUClose()
{
#ifdef SOUND_ON
	// SDL_CondSignal(sound_cv);
#endif
}

void Sound_APU_Start()
{
#ifdef SOUND_ON
	// SDL_LockAudio();
	// SDL_LockMutex(sound_mutex);
#endif
}

void Sound_APU_End()
{
#ifdef SOUND_ON
	// SDL_UnlockAudio();
	// SDL_UnlockMutex(sound_mutex);
	// SDL_CondSignal(sound_cv);
#endif
}

void Sound_Update()
{
	
}
