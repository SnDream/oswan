#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <SDL/SDL.h>
#include "shared.h"
#include "drawing.h"
#include "input.h"
#include "menu.h"
#include "sound.h"

#ifndef NO_WAIT
void msleep(long milisec);
#endif

uint32_t m_Flag;
static long interval;
static long nextTick, lastTick = 0, newTick, currentTick, wait;
static long skipTick = INT32_MIN;
static int32_t FPS = 60; 
static int32_t pastFPS = 0; 
char gameName[512];

int sound_on = 0;

uint32_t game_alreadyloaded = 0;

#ifdef FRAMESKIP
extern int32_t FrameSkip;
#endif

#define GRA_FRAME 75 /* Same as FPS? */
#define SKIP_RATE 15
void graphics_paint(void) 
{
#ifdef FRAMESKIP
	newTick = SDL_GetTicks();
	
	skipTick += newTick > lastTick ? (newTick - lastTick) : 0 - 1000 / GRA_FRAME;
	if (skipTick < 0) skipTick = 0;
	lastTick = newTick;

	if (FrameSkip == 0) {
		screen_draw();
		FrameSkip = (skipTick / (1000 / GRA_FRAME));
		if (FrameSkip > GRA_FRAME / SKIP_RATE) FrameSkip = GRA_FRAME / SKIP_RATE;
		skipTick %= 1000 / GRA_FRAME;
	}
	else {
		FrameSkip--;
	}
#else
	screen_draw();
#endif
}

static void initSDL(void) 
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	SDL_ShowCursor(SDL_DISABLE);
	
	SetVideo(0);
	Init_Sound();
}

static void exit_oswan()
{
	save_config();
	WsDeInit();
	Cleanup_Sound();
	Cleanup_Screen();
}

int main(int argc, char *argv[]) 
{
#ifdef NOROMLOADER
	if (argc < 2) return 0;	
#endif
	m_Flag = GF_MAINUI;
	
	load_config();
	
	/* Init graphics & sound */
	initSDL();

    /*	load rom file via args if a rom path is supplied	*/
	snprintf(gameName, sizeof(gameName) ,"%");
	
	if(argc > 1) 
	{
#ifdef NATIVE_RESOLUTION
		SetVideo(1);
#endif
		snprintf(gameName, sizeof(gameName) ,"%s", argv[1]);
		m_Flag = GF_GAMEINIT;
		game_alreadyloaded = 1;
	}

	while (m_Flag != GF_GAMEQUIT) 
	{
		switch (m_Flag) 
		{
			case GF_MAINUI:
				Pause_Sound();
				Menu();
				if (sound_on && cartridge_IsLoaded()) 
				{
					Resume_Sound();
				}
				#ifndef NO_WAIT
				nextTick = SDL_GetTicks() + interval;
				#endif
				FrameSkip = 0;
				lastTick = SDL_GetTicks();
				skipTick = INT32_MIN;
				break;

			case GF_GAMEINIT:
				if (WsCreate(gameName)) 
				{
					WsInit();
					m_Flag = GF_GAMERUNNING;
					Resume_Sound();
					game_alreadyloaded = 1;
					lastTick = SDL_GetTicks();
					#ifndef NO_WAIT
					/* Init timing */
					interval = 1000 / 60; // (1.0f / 60) * 1000000;
					nextTick = SDL_GetTicks() + interval;
					#endif
				}
				else 
				{
					fprintf(stderr,"Can't load %s", gameName); 
					fflush(stderr);
					m_Flag = GF_GAMEQUIT;
				}
				
				break;
		
			case GF_GAMERUNNING:	
				#ifndef NO_WAIT
				currentTick = SDL_GetTicks(); 
				wait = (nextTick - currentTick);
				if (wait > 25) 
				{
					if (wait < 1000) 
					{
						SDL_Delay(wait);
					}
				}
				nextTick += interval;
				#endif
				exit_button();
				WsRun();
				Sound_Update();
				break;
		}
	}
	
	exit_oswan();
	
	return 0;
}
