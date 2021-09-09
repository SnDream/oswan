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

int sound_volume = 5;

char lcd_icon_stat[LCD_INDEX__END] = {0};
char lcd_icon_mode = 0;

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

void lcd_icon_stat_init(void)
{
	lcd_icon_stat[LCD_INDEX__BIG_CIRCLE] = LCD_ICON__BIG_CIRCLE_OFF;
	lcd_icon_stat[LCD_INDEX__MIDIUM_CIRCLE] = LCD_ICON__MIDIUM_CIRCLE_OFF;
	lcd_icon_stat[LCD_INDEX__SMALL_CIRCLE] = LCD_ICON__SMALL_CIRCLE_OFF;
	lcd_icon_stat[LCD_INDEX__HORIZONTAL] = LCD_ICON__HORIZONTAL_OFF;
	lcd_icon_stat[LCD_INDEX__VERTICAL] = LCD_ICON__VERTICAL_OFF;
	lcd_icon_stat[LCD_INDEX__EARPHONE] = LCD_ICON__EARPHONE_OFF;
	lcd_icon_stat[LCD_INDEX__VOLUME] = LCD_ICON__VOLUME_L3;
	lcd_icon_stat[LCD_INDEX__BATTERY] = LCD_ICON__BATTERY_NORMAL;
	lcd_icon_stat[LCD_INDEX__POWER_SAVING] = LCD_ICON__POWER_SAVING_OFF;
	lcd_icon_stat[LCD_INDEX__CARTRIDGE] = LCD_ICON__CARTRIDGE_INSTALLED;
	lcd_icon_stat[LCD_INDEX__CONSOLE_POWER] = LCD_ICON__CONSOLE_POWER_ON;
	lcd_icon_stat[LCD_INDEX__PLACEHOLDER] = LCD_ICON__BLANK;
	lcd_icon_stat[LCD_INDEX__REMAP_Y] = LCD_ICON__REMAP_Y_OFF;
	lcd_icon_stat[LCD_INDEX__REMAP_X] = LCD_ICON__REMAP_X_OFF;
	lcd_icon_stat[LCD_INDEX__SWAP_TO_START] = LCD_ICON__SWAP_TO_START_OFF;
	lcd_icon_stat[LCD_INDEX__SWAP_TO_OPTION] = LCD_ICON__SWAP_TO_OPTION_OFF;
	lcd_icon_stat[LCD_INDEX__SWAP_TO_BUTTON_A] = LCD_ICON__SWAP_TO_BUTTON_A_OFF;
	lcd_icon_stat[LCD_INDEX__SWAP_TO_BUTTON_B] = LCD_ICON__SWAP_TO_BUTTON_B_OFF;
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
				if (sound_volume && cartridge_IsLoaded()) 
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
