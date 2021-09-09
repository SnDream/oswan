#ifndef SHARED_H
#define SHARED_H

#ifdef _TINSPIRE
#include <os.h>
#else
#include <SDL/SDL.h>
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

#ifndef PATH_MAX
#define PATH_MAX 2048
#endif

#ifndef RS90
#define MENU_SCREEN_WIDTH 320
#define MENU_SCREEN_HEIGHT 240
#else
#define MENU_SCREEN_WIDTH 240
#define MENU_SCREEN_HEIGHT 160
#endif

#ifdef GCW0
	#ifndef HOME_SUPPORT
	#define HOME_SUPPORT
	#endif
	#ifndef JOYSTICK
	#define JOYSTICK
	#endif
	#ifndef POSIX
	#define POSIX
	#endif
#endif

#ifdef BITTBOY
	#ifndef HOME_SUPPORT
	#define HOME_SUPPORT
	#endif
	#ifndef POSIX
	#define POSIX
	#endif
#endif

#ifdef RG99
	#ifndef HOME_SUPPORT
	#define HOME_SUPPORT
	#endif
	#ifndef POSIX
	#define POSIX
	#endif
	/*#ifndef NO_WAIT
	#define NO_WAIT
	#endif*/
#endif

#ifdef RS97
	#ifndef HOME_SUPPORT
	#define HOME_SUPPORT
	#endif
	#ifndef POSIX
	#define POSIX
	#endif
	/*#ifndef NO_WAIT
	#define NO_WAIT
	#endif*/
#endif

#ifdef RS90
	#ifndef HOME_SUPPORT
	#define HOME_SUPPORT
	#endif
	#ifndef POSIX
	#define POSIX
	#endif
	/*#ifndef NO_WAIT
	#define NO_WAIT
	#endif*/
#endif

#ifdef DINGOO
	#ifndef NOWAIT
	#define NOWAIT
	#endif
#endif

#ifdef _TINSPIRE
	#ifndef NO_WAIT
	#define NO_WAIT
	#endif
	#ifndef HOME_SUPPORT
	#define HOME_SUPPORT
	#endif
	#define SDL_Surface int32_t
#endif

#ifdef DREAMCAST
	#ifndef POSIX
	#define POSIX
	#endif
	#ifndef JOYSTICK
	#define JOYSTICK
	#endif
#endif

#ifndef SDL_TRIPLEBUF
#define SDL_TRIPLEBUF SDL_DOUBLEBUF
#endif

#ifdef _TINSPIRE
	#define BITDEPTH_OSWAN 16
	#define FLAG_VIDEO SDL_SWSURFACE
	#define REAL_SCREEN_WIDTH 320
	#define REAL_SCREEN_HEIGHT 240
#elif defined(GCW)
	#define BITDEPTH_OSWAN 16
	#define FLAG_VIDEO SDL_HWSURFACE
	#define REAL_SCREEN_WIDTH 320
	#define REAL_SCREEN_HEIGHT 240
#elif defined(BITTBOY)
	#define BITDEPTH_OSWAN 16
	#define FLAG_VIDEO SDL_HWSURFACE
	#define REAL_SCREEN_WIDTH 320
	#define REAL_SCREEN_HEIGHT 240
#elif defined(RS97)
	#define BITDEPTH_OSWAN 16
	#define FLAG_VIDEO SDL_HWSURFACE
	#define REAL_SCREEN_WIDTH 320
	#define REAL_SCREEN_HEIGHT 240
#elif defined(RG99)
	#define BITDEPTH_OSWAN 16
	#define FLAG_VIDEO SDL_HWSURFACE | SDL_TRIPLEBUF
	#define REAL_SCREEN_WIDTH 320
	#define REAL_SCREEN_HEIGHT 240
#elif defined(RS90)
	#define BITDEPTH_OSWAN 16
	#define FLAG_VIDEO SDL_HWSURFACE | SDL_DOUBLEBUF
	#define REAL_SCREEN_WIDTH 240
	#define REAL_SCREEN_HEIGHT 160
#elif defined(DREAMCAST)
	#define BITDEPTH_OSWAN 16
	#define FLAG_VIDEO SDL_SWSURFACE
	#define REAL_SCREEN_WIDTH 320
	#define REAL_SCREEN_HEIGHT 240
#else
	#define BITDEPTH_OSWAN 16
	#define FLAG_VIDEO SDL_HWSURFACE | SDL_TRIPLEBUF
	#define REAL_SCREEN_WIDTH 320
	#define REAL_SCREEN_HEIGHT 240
#endif

#ifdef _TINSPIRE
	#define PATH_DIRECTORY "./"
	#define SAVE_DIRECTORY ".oswan/"
	#define EXTENSION ".tns"
#elif defined(GCW)
	#define PATH_DIRECTORY getenv("HOME")
	#define SAVE_DIRECTORY "/.oswan/"
	#define EXTENSION ""
#elif defined(DREAMCAST)
	#define PATH_DIRECTORY "/ram/"
	#define SAVE_DIRECTORY ""
	#define EXTENSION ""
#else
	#ifdef HOME_SUPPORT
		#define PATH_DIRECTORY getenv("HOME")
		#define SAVE_DIRECTORY "/.oswanemu/"
	#else
		#define PATH_DIRECTORY "./"
		#define SAVE_DIRECTORY ""
	#endif
	#define EXTENSION ""
#endif

#define MAX__PATH 1024
#define FILE_LIST_ROWS 19

#define SYSVID_WIDTH	224
#define SYSVID_HEIGHT	144

#define GF_GAMEINIT    1
#define GF_MAINUI      2
#define GF_GAMEQUIT    3
#define GF_GAMERUNNING 4

#ifndef O_BINARY
#define O_BINARY 0
#endif


/* Oswan dependencies */
#include "../emu/WS.h"
#include "../emu/WSApu.h"
#include "../emu/WSFileio.h"
#include "../emu/WSRender.h"

#define cartridge_IsLoaded() (strlen(gameName) != 0)

/* SDL drawing screen */
extern void graphics_paint(void);

extern void lcd_icon_stat_init(void);

enum lcd_index {
	LCD_INDEX__BIG_CIRCLE = 0,
	LCD_INDEX__MIDIUM_CIRCLE,
	LCD_INDEX__SMALL_CIRCLE,
	LCD_INDEX__HORIZONTAL,
	LCD_INDEX__VERTICAL,
	LCD_INDEX__EARPHONE,
	LCD_INDEX__VOLUME,
	LCD_INDEX__BATTERY,
	LCD_INDEX__POWER_SAVING,
	LCD_INDEX__CARTRIDGE,
	LCD_INDEX__CONSOLE_POWER,
	LCD_INDEX__PLACEHOLDER,
	LCD_INDEX__PLACEHOLDER2,
	LCD_INDEX__REMAP_Y,
	LCD_INDEX__REMAP_X,
	LCD_INDEX__SWAP_TO_START,
	LCD_INDEX__SWAP_TO_BTN_A,
	LCD_INDEX__SWAP_TO_BTN_B,
	LCD_INDEX__END,
};

enum lcd_icon {
	LCD_ICON__BLANK = 0,
	LCD_ICON__BIG_CIRCLE_OFF,
	LCD_ICON__BIG_CIRCLE_ON,
	LCD_ICON__MIDIUM_CIRCLE_OFF,
	LCD_ICON__MIDIUM_CIRCLE_ON,
	LCD_ICON__SMALL_CIRCLE_OFF,
	LCD_ICON__SMALL_CIRCLE_ON,
	LCD_ICON__HORIZONTAL_OFF,
	LCD_ICON__HORIZONTAL_ON,
	LCD_ICON__VERTICAL_OFF,
	LCD_ICON__VERTICAL_ON,
	LCD_ICON__EARPHONE_OFF,
	LCD_ICON__EARPHONE_ON,
	LCD_ICON__VOLUME_MUTE,
	LCD_ICON__VOLUME_L1,
	LCD_ICON__VOLUME_L2,
	LCD_ICON__VOLUME_L3,
	LCD_ICON__BATTERY_NORMAL,
	LCD_ICON__BATTERY_DRY,
	LCD_ICON__POWER_SAVING_OFF,
	LCD_ICON__POWER_SAVING_ON,
	LCD_ICON__CARTRIDGE_EMPTY,
	LCD_ICON__CARTRIDGE_INSTALLED,
	LCD_ICON__CONSOLE_POWER_OFF,
	LCD_ICON__CONSOLE_POWER_ON,
	LCD_ICON__REMAP_Y_OFF,
	LCD_ICON__REMAP_Y_ON,
	LCD_ICON__REMAP_X_OFF,
	LCD_ICON__REMAP_X_ON,
	LCD_ICON__SWAP_TO_START_OFF,
	LCD_ICON__SWAP_TO_START_ON,
	LCD_ICON__SWAP_TO_BTN_A_OFF,
	LCD_ICON__SWAP_TO_BTN_A_ON,
	LCD_ICON__SWAP_TO_BTN_B_OFF,
	LCD_ICON__SWAP_TO_BTN_B_ON,
	LCD_ICON__END
};

extern char lcd_icon_stat[LCD_INDEX__END];
extern char lcd_icon_mode;

extern uint32_t m_Flag;
extern char gameName[512];
extern uint32_t game_alreadyloaded;

extern int sound_volume;

#endif
