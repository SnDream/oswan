#include <SDL/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "font_drawing.h"
#include "browser.h"
#include "drawing.h"
#include "shared.h"
#include "input.h"
#include "menu.h"

static uint32_t done_menu = 0;
uint32_t profile_config = 0;

void print_text_center(const char* text, uint32_t y)
{
	uint32_t sizeofarray = strnlen(text, MAX_TEXT_SIZE);
	uint32_t x = (actualScreen->w - (sizeofarray * 8)) / 2;
	
	print_string(text, TextWhite, 0, x, y, Surface_to_Draw_menu);
}

int SDLK2InputSel(SDLKey key)
{
	switch(key) {
	case SDLK_UNKNOWN	:	return  0;	/* None */
	case SDLK_UP		:	return  1;	/* UP button */
	case SDLK_DOWN		:	return  2;	/* DOWN button */
	case SDLK_LEFT		:	return  3;	/* LEFT button */
	case SDLK_RIGHT		:	return  4;	/* RIGHT button */
	case SDLK_LCTRL		:	return  5;	/* A button */
	case SDLK_LALT		:	return  6;	/* B button */
	case SDLK_SPACE		:	return  7;	/* X button */
	case SDLK_LSHIFT	:	return  8;	/* Y button */
	case SDLK_TAB		:	return  9;	/* L button */
	case SDLK_BACKSPACE	:	return 10;	/* R button */
	case SDLK_PAGEUP	:	return 11;	/* L2 button */
	case SDLK_PAGEDOWN	:	return 12;	/* R2 button */
	case SDLK_RETURN	:	return 13;	/* Start button */
	case SDLK_ESCAPE	:	return 14;	/* Select button */
	default				:	return 15;	/* Name Unknown */
	}
}

SDLKey InputSel2SDLK(int sel)
{
	switch(sel) {
	default	:	return SDLK_UNKNOWN		;	/* Name Unknown */
	case  0	:	return SDLK_UNKNOWN		;	/* None */
	case  1	:	return SDLK_UP			;	/* UP button */
	case  2	:	return SDLK_DOWN		;	/* DOWN button */
	case  3	:	return SDLK_LEFT		;	/* LEFT button */
	case  4	:	return SDLK_RIGHT		;	/* RIGHT button */
	case  5	:	return SDLK_LCTRL		;	/* A button */
	case  6	:	return SDLK_LALT		;	/* B button */
	case  7	:	return SDLK_SPACE		;	/* X button */
	case  8	:	return SDLK_LSHIFT		;	/* Y button */
	case  9	:	return SDLK_TAB			;	/* L button */
	case 10	:	return SDLK_BACKSPACE	;	/* R button */
	case 11	:	return SDLK_PAGEUP		;	/* L2 button */
	case 12	:	return SDLK_PAGEDOWN	;	/* R2 button */
	case 13	:	return SDLK_RETURN		;	/* Start button */
	case 14	:	return SDLK_ESCAPE		;	/* Select button */
	}
}

/* Main Menu */

int					Menu_Main_Init(struct menu*);
struct menu_item	Menu_Main_Item[];

struct menu Menu_Main = {
	.menu_x			= (MENU_SCREEN_WIDTH - 240) / 2,
	.menu_y			= (MENU_SCREEN_HEIGHT - 160) / 2 + 24,
	.menu_init_call	= Menu_Main_Init,
	.menu_done_call	= NULL,
	.item_num		= 9,
	.item_w			= 240,
	.item_h			= 15,
	.item			= Menu_Main_Item,
	.font_x			= FONT_X_AUTO,
	.font_y			= FONT_Y_AUTO,
};

int Menu_Main_Init(struct menu* self)
{
#if !defined(RS90) && !defined(RG99)
	print_text_center("Gameblabla's Oswan", 6);
#else
	print_text_center("Oswan+", self->menu_y - 24);
	print_text_center(__DATE__, self->menu_y - 16);
	if (config.custom) {
		print_string("Custom", TextWhite, 0, self->menu_x + 240 - 6 * 8,
				self->menu_y - 24, Surface_to_Draw_menu);
		print_string("Config", TextWhite, 0, self->menu_x + 240 - 6 * 8,
				self->menu_y - 16, Surface_to_Draw_menu);
	}
#endif
}

int			Menu_Continue_Sel(struct menu_item* self);

int			Menu_Load_State_Sel(struct menu_item* self);
int			Menu_Save_State_Sel(struct menu_item* self);
const char*	Menu_State_ConfText[];
int			Menu_State_ConfDone(struct menu_item* self);

const char*	Menu_Scaling_ConfText[];
int			Menu_Scaling_ConfInit(struct menu_item* self);
int			Menu_Scaling_ConfDone(struct menu_item* self);

struct menu	Menu_Input;
const char*	Menu_Input_ConfText[];
int			Menu_Input_ConfInit(struct menu_item* self);

const char*	Menu_Volume_ConfText[];
int			Menu_Volume_ConfInit(struct menu_item* self);
int			Menu_Volume_ConfDone(struct menu_item* self);

const char*	Menu_Config_ConfText[];
int			Menu_Config_ConfInit(struct menu_item* self);
int			Menu_Config_Sel(struct menu_item* self);

int			Menu_Reset_Sel(struct menu_item* self);

int			Menu_Exit_Sel(struct menu_item* self);

struct menu_item Menu_Main_Item[] = {
	{
		.name			= "Continue",
		.sel_call		= Menu_Continue_Sel,
		.sub_menu		= NULL,
		.conf_num		= 0,
		.conf_x			= 0,
		.conf_y			= 0,
		.conf_text		= NULL,
		.conf_init_call	= NULL,
		.conf_done_call	= NULL,
	}, {
		.name			= "Load State",
		.sel_call		= Menu_Load_State_Sel,
		.sub_menu		= NULL,
		.conf_num		= 10,
		.conf_x			= 120,
		.conf_y			= 0,
		.conf_text		= Menu_State_ConfText,
		.conf_init_call	= NULL,
		.conf_done_call	= Menu_State_ConfDone,
	}, {
		.name			= "Save State",
		.sel_call		= Menu_Save_State_Sel,
		.sub_menu		= NULL,
		.conf_num		= 10,
		.conf_x			= 120,
		.conf_y			= 0,
		.conf_text		= Menu_State_ConfText,
		.conf_init_call	= NULL,
		.conf_done_call	= Menu_State_ConfDone,
	}, {
		.name			= "Scaling",
		.sel_call		= NULL,
		.sub_menu		= NULL,
		.conf_num		= 3,
		.conf_x			= 120,
		.conf_y			= 0,
		.conf_text		= Menu_Scaling_ConfText,
		.conf_init_call	= Menu_Scaling_ConfInit,
		.conf_done_call	= Menu_Scaling_ConfDone,
	}, {
		.name			= "Input Options",
		.sel_call		= NULL,
		.sub_menu		= &Menu_Input,
		.conf_num		= 2,
		.conf_x			= 120,
		.conf_y			= 0,
		.conf_text		= Menu_Input_ConfText,
		.conf_init_call	= Menu_Input_ConfInit,
		.conf_done_call	= NULL,
	}, {
		.name			= "Volume",
		.sel_call		= NULL,
		.sub_menu		= NULL,
		.conf_num		= 4,
		.conf_x			= 120,
		.conf_y			= 0,
		.conf_text		= Menu_Volume_ConfText,
		.conf_init_call	= Menu_Volume_ConfInit,
		.conf_done_call	= Menu_Volume_ConfDone,
	}, {
		.name			= "Config File",
		.sel_call		= Menu_Config_Sel,
		.sub_menu		= NULL,
		.conf_num		= 3,
		.conf_x			= 120,
		.conf_y			= 0,
		.conf_text		= Menu_Config_ConfText,
		.conf_init_call	= Menu_Config_ConfInit,
		.conf_done_call	= NULL,
	}, {
		.name			= "Reset",
		.sel_call		= Menu_Reset_Sel,
		.sub_menu		= NULL,
		.conf_num		= 0,
		.conf_x			= 120,
		.conf_y			= 0,
		.conf_text		= NULL,
		.conf_init_call	= NULL,
		.conf_done_call	= NULL,
	}, {
		.name			= "Exit",
		.sel_call		= Menu_Exit_Sel,
		.sub_menu		= NULL,
		.conf_num		= 0,
		.conf_x			= 0,
		.conf_y			= 0,
		.conf_text		= NULL,
		.conf_init_call	= NULL,
		.conf_done_call	= NULL,
	},
};

/* Continue */

int Menu_Continue_Sel(struct menu_item* self)
{
	SDL_Event event;
	self->pmenu->menu_done = 1;
	do {
		SDL_WaitEvent(&event);
	} while (event.type != SDL_KEYUP);
}

/* Save/Load State */

int Menu_Load_State_Sel(struct menu_item* self)
{
	SDL_Event event;
	if (!cartridge_IsLoaded()) return 0;
	if (WsLoadState(gameName, self->conf_sel) != 0) {
		Clear_Menu();
		print_text_center("Load state failed!", 76);
		Update_Screen();
	} else self->pmenu->menu_done = 1;
	do {
		SDL_WaitEvent(&event);
	} while (event.type != SDL_KEYUP);
}
int Menu_Save_State_Sel(struct menu_item* self)
{
	SDL_Event event;
	if (!cartridge_IsLoaded()) return 0;
	if (WsSaveState(gameName, self->conf_sel) != 0) {
		Clear_Menu();
		print_text_center("Save state failed!", 76);
		Update_Screen();
	} else self->pmenu->menu_done = 1;
	do {
		SDL_WaitEvent(&event);
	} while (event.type != SDL_KEYUP);
}
const char* Menu_State_ConfText[] = {
	"0", "1", "2", "3", "4",
	"5", "6", "7", "8", "9",
};
int Menu_State_ConfDone(struct menu_item* self)
{
	int i;
	for (i = 0; i < self->pmenu->item_num; i++) {
		if (strcmp(self->pmenu->item[i].name, "Load State") == 0) {
			self->pmenu->item[i].conf_sel = self->conf_sel;
		} else if (strcmp(self->pmenu->item[i].name, "Save State") == 0) {
			self->pmenu->item[i].conf_sel = self->conf_sel;
		}
	}
	return 0;
}

/* Scaling */

const char* Menu_Scaling_ConfText[] = {
#if !defined(RS90) && !defined(RG99)
	"Native",
	"Fullscreen",
	"Keep Aspect",
#elif defined(RG99) /* Compatible with upstream */
	"Native",
	"Fullscreen",
	"Keep Horizontal",
#elif defined(RS90)
 #ifndef SHOW_LCD_ICON
	"Native",
	"Unused 1",
	"Unused 2",
 #else
	"Native",
	"Native Center",
	"Native w/o Icon",
 #endif
#endif
};
int Menu_Scaling_ConfInit(struct menu_item* self)
{
	if (config.scaling < 0 || config.scaling > self->conf_num) {
		config.scaling = 0;
	}
	self->conf_sel = config.scaling;
}
int Menu_Scaling_ConfDone(struct menu_item* self)
{
	config.scaling = self->conf_sel;
}

/* Input Options */

const char* Menu_Input_ConfText[] = {
	"Horizontal",
	"Vertical",
};
int Menu_Input_ConfInit(struct menu_item* self)
{
	self->conf_sel = (HVMode == 0) ? 0 : 1;
}

/* Input Options Submenu */

int					Menu_Input_MenuInit(struct menu* self);
struct menu_item	Menu_Input_Item[];
const char*			Menu_Input_Button_ConfText[];

struct menu Menu_Input = {
	.menu_x			= (MENU_SCREEN_WIDTH - 240) / 2,
	.menu_y			= (MENU_SCREEN_HEIGHT - 160) / 2,
	.menu_init_call	= Menu_Input_MenuInit,
	.menu_done_call	= NULL,
	.item_num		= 14,
	.item_w			= 264,
	.item_h			= 9,
	.item			= Menu_Input_Item,
	.font_x			= FONT_X_AUTO,
	.font_y			= FONT_Y_AUTO,
};

#ifdef RG99
const char* Menu_Input_MenuInit_ItemNames[2][8] = {
	{
		"WS Y1 (\030)",
		"WS Y2 (\032)",
		"WS Y3 (\031)",
		"WS Y4 (\033)",
		"WS X1 (\030)",
		"WS X2 (\032)",
		"WS X3 (\031)",
		"WS X4 (\033)",
	} , {
		"WS Y1 (\033)",
		"WS Y2 (\030)",
		"WS Y3 (\032)",
		"WS Y4 (\031)",
		"WS X1 (\033)",
		"WS X2 (\030)",
		"WS X3 (\032)",
		"WS X4 (\031)",
	}
};
#endif

int Menu_Input_MenuInit(struct menu* self)
{
	int remap_mode, type;

	update_key_config();

	type = self->pitem->conf_sel;
	if (type != HC_H && type != HC_V) return 0;

#ifdef RG99
	for (int i = 0; i < 8; i++) {
		self->item[i].name = Menu_Input_MenuInit_ItemNames[type][i];
	}
#endif

	remap_mode = config.remap_mode[type];
	switch (remap_mode) {
	case REMAP_MODE_HOLDY2X:
		print_string("Hold         :",
				TextWhite, 0, self->menu_x +     0, self->menu_y + 132, Surface_to_Draw_menu);
		print_string(self->item[12].conf_text[self->item[12].conf_sel],
				TextWhite, 0, self->menu_x + 6 * 8, self->menu_y + 132, Surface_to_Draw_menu);
		print_string("Press X1/2/3/4 as Y1/2/3/4",
				TextWhite, 0, self->menu_x + 4 * 8, self->menu_y + 142, Surface_to_Draw_menu);
		break;
	case REMAP_MODE_HOLDX2Y:
		print_string("Hold         :",
				TextWhite, 0, self->menu_x +     0, self->menu_y + 132, Surface_to_Draw_menu);
		print_string(self->item[12].conf_text[self->item[12].conf_sel],
				TextWhite, 0, self->menu_x + 6 * 8, self->menu_y + 132, Surface_to_Draw_menu);
		print_string("Press Y1/2/3/4 as X1/2/3/4",
				TextWhite, 0, self->menu_x + 4 * 8, self->menu_y + 142, Surface_to_Draw_menu);
		break;
	case REMAP_MODE_PRESSXY:
		print_string("Press        :",
				TextWhite, 0, self->menu_x +     0, self->menu_y + 132, Surface_to_Draw_menu);
		print_string(self->item[12].conf_text[self->item[12].conf_sel],
				TextWhite, 0, self->menu_x + 6 * 8, self->menu_y + 132, Surface_to_Draw_menu);
		print_string("Swap X1/2/3/4 and Y1/2/3/4",
				TextWhite, 0, self->menu_x + 4 * 8, self->menu_y + 142, Surface_to_Draw_menu);
		break;
	case REMAP_MODE_ACTIVATE:
		print_string("Press        : START\035A\035B",
				TextWhite, 0, self->menu_x +     0, self->menu_y + 132, Surface_to_Draw_menu);
		print_string(self->item[12].conf_text[self->item[12].conf_sel],
				TextWhite, 0, self->menu_x + 6 * 8, self->menu_y + 132, Surface_to_Draw_menu);
		print_string("Press        as Activation",
				TextWhite, 0, self->menu_x + 4 * 8, self->menu_y + 142, Surface_to_Draw_menu);
		print_string(self->item[8].conf_text[self->item[8].conf_sel],
				TextWhite, 0, self->menu_x +10 * 8, self->menu_y + 142, Surface_to_Draw_menu);
		break;
	}

	if (menu_key[type][0] == NULL) {
		print_string(Menu_Input_Button_ConfText[SDLK2InputSel(SDLK_ESCAPE)],
				TextWhite, 0, self->menu_x +     0, self->menu_y + 152, Surface_to_Draw_menu);
		print_string("+",
				TextWhite, 0, self->menu_x + 6 * 8, self->menu_y + 152, Surface_to_Draw_menu);
		print_string(Menu_Input_Button_ConfText[SDLK2InputSel(SDLK_RETURN)],
				TextWhite, 0, self->menu_x + 7 * 8, self->menu_y + 152, Surface_to_Draw_menu);
	} else if (menu_key[type][1] == NULL) {
		print_string(Menu_Input_Button_ConfText[SDLK2InputSel(*menu_key[type][0])],
				TextWhite, 0, self->menu_x +     0, self->menu_y + 152, Surface_to_Draw_menu);
	} else {
		print_string(Menu_Input_Button_ConfText[SDLK2InputSel(*menu_key[type][0])],
				TextWhite, 0, self->menu_x +     0, self->menu_y + 152, Surface_to_Draw_menu);
		print_string("+",
				TextWhite, 0, self->menu_x + 6 * 8, self->menu_y + 152, Surface_to_Draw_menu);
		print_string(Menu_Input_Button_ConfText[SDLK2InputSel(*menu_key[type][1])],
				TextWhite, 0, self->menu_x + 7 * 8, self->menu_y + 152, Surface_to_Draw_menu);
	}
	print_string(": Emulator Menu",
			TextWhite, 0, self->menu_x + 13 * 8, self->menu_y + 152, Surface_to_Draw_menu);
}

int			Menu_Input_Button_Sel(struct menu_item*);
const char*	Menu_Input_Button_ConfText[];
int			Menu_Input_Button_ConfInit(struct menu_item*);
int			Menu_Input_Button_ConfDone(struct menu_item*);
const char*	Menu_Input_RemapMode_ConfText[];
int			Menu_Input_RemapMode_ConfInit(struct menu_item*);
int			Menu_Input_RemapMode_ConfDone(struct menu_item*);

#define MENU_INPUT_ITEM(btn_name) {			\
	.name			= (btn_name),					\
	.sel_call		= Menu_Input_Button_Sel,		\
	.sub_menu		= NULL,							\
	.conf_num		= 16,							\
	.conf_x			= 160,							\
	.conf_y			= 0,							\
	.conf_text		= Menu_Input_Button_ConfText,	\
	.conf_init_call	= Menu_Input_Button_ConfInit,	\
	.conf_done_call	= Menu_Input_Button_ConfDone,	\
}

struct menu_item Menu_Input_Item[] = {
	MENU_INPUT_ITEM("WS Y1 (\030)"),
	MENU_INPUT_ITEM("WS Y2 (\032)"),
	MENU_INPUT_ITEM("WS Y3 (\031)"),
	MENU_INPUT_ITEM("WS Y4 (\033)"),
	MENU_INPUT_ITEM("WS X1 (\030)"),
	MENU_INPUT_ITEM("WS X2 (\032)"),
	MENU_INPUT_ITEM("WS X3 (\031)"),
	MENU_INPUT_ITEM("WS X4 (\033)"),
	MENU_INPUT_ITEM("WS START"    ),
	MENU_INPUT_ITEM("WS BUTTON A" ),
	MENU_INPUT_ITEM("WS BUTTON B" ),
	MENU_INPUT_ITEM("Emu Menu"    ),
	MENU_INPUT_ITEM("Remap Switch"),
	{
		.name			= "Remap Mode",
		.sel_call		= NULL,
		.sub_menu		= NULL,
		.conf_num		= 5,
		.conf_x			= 160,
		.conf_y			= 0,
		.conf_text		= Menu_Input_RemapMode_ConfText,
		.conf_init_call	= Menu_Input_RemapMode_ConfInit,
		.conf_done_call	= Menu_Input_RemapMode_ConfDone,
	}
};

int Menu_Input_ItemSel2HCKey(int item_sel)
{
	switch (item_sel) {
		case  0: item_sel = HC_KEY_Y1; break;
		case  1: item_sel = HC_KEY_Y2; break;
		case  2: item_sel = HC_KEY_Y3; break;
		case  3: item_sel = HC_KEY_Y4; break;
		case  4: item_sel = HC_KEY_X1; break;
		case  5: item_sel = HC_KEY_X2; break;
		case  6: item_sel = HC_KEY_X3; break;
		case  7: item_sel = HC_KEY_X4; break;
		case  8: item_sel = HC_KEY_START; break;
		case  9: item_sel = HC_KEY_BTN_A; break;
		case 10: item_sel = HC_KEY_BTN_B; break;
		case 11: item_sel = HC_KEY_EMENU; break;
		case 12: item_sel = HC_KEY_REMAP; break;
		default: item_sel = -1;
	}
	return item_sel;
}

int Menu_Input_Button_Sel(struct menu_item* self)
{
	int type, item_sel;
	SDL_Event event;

	type = self->pmenu->pitem->conf_sel;
	item_sel = self->pmenu->item_sel;
	if (type != HC_H && type != HC_V) return 0;
	item_sel = Menu_Input_ItemSel2HCKey(item_sel);
	if (item_sel < 0 || item_sel >= HC_KEY_END) return 0;

	Clear_Menu();
	print_text_center("Press a key for", 72 - 2);
	print_text_center(self->name, 80 + 2);
	Update_Screen();

	do {
		SDL_WaitEvent(&event);
	} while (event.type != SDL_KEYDOWN);
	self->conf_sel = SDLK2InputSel(event.key.keysym.sym);
	if (self->conf_sel < 0 || self->conf_sel >= self->conf_num) {
		self->conf_sel = 0;
		keys_config[type].buttons[item_sel] = SDLK_UNKNOWN;
	} else {
		keys_config[type].buttons[item_sel] = event.key.keysym.sym;
	}
	return 0;
}
const char*	Menu_Input_Button_ConfText[] = {
	"None",
	"Dpad\030",	"Dpad\031",	"Dpad\033",	"Dpad\032",
	"(A)",	"(B)",	"(X)",	"(Y)",
	"(L)",	"(R)",	"(L2)",	"(R2)",
	"Start",	"Select",
	"?",
};
int Menu_Input_Button_ConfInit(struct menu_item* self)
{
	int type, item_sel;
	SDLKey key;

	type = self->pmenu->pitem->conf_sel;
	item_sel = self->pmenu->item_sel;
	if (type != HC_H && type != HC_V) return 0;
	item_sel = Menu_Input_ItemSel2HCKey(item_sel);
	if (item_sel < 0 || item_sel >= HC_KEY_END) return 0;

	key = keys_config[type].buttons[item_sel];
	self->conf_sel = SDLK2InputSel(key);
	return 0;
}

int Menu_Input_Button_ConfDone(struct menu_item* self)
{
	int type, item_sel;
	SDLKey key;

	type = self->pmenu->pitem->conf_sel;
	item_sel = self->pmenu->item_sel;
	if (type != HC_H && type != HC_V) return 0;
	item_sel = Menu_Input_ItemSel2HCKey(item_sel);
	if (item_sel < 0 || item_sel >= HC_KEY_END) return 0;

	key = InputSel2SDLK(self->conf_sel);
	keys_config[type].buttons[item_sel] = key;
	return 0;
}

const char*	Menu_Input_RemapMode_ConfText[] = {
	"None",
	"Hold Y\032X",
	"Hold X\032Y",
	"Press X\035Y",
	"Activate",
};
int Menu_Input_RemapMode_ConfInit(struct menu_item* self)
{
	int type;
	SDLKey key;

	type = self->pmenu->pitem->conf_sel;
	if (type != HC_H && type != HC_V) return 0;

	key = config.remap_mode[type];
	if (key < REMAP_MODE_NONE || key >= REMAP_MODE_END) {
		key = REMAP_MODE_NONE;
		config.remap_mode[type] = key;
	}
	self->conf_sel = key;
	return 0;
}

int Menu_Input_RemapMode_ConfDone(struct menu_item* self)
{
	int type;
	SDLKey key;

	type = self->pmenu->pitem->conf_sel;
	if (type != HC_H && type != HC_V) return 0;

	config.remap_mode[type] = self->conf_sel;
	return 0;
}

/* Volume */

const char* Menu_Volume_ConfText[] = {
	"Mute", "Level 1", "Level 2", "Level 3",
};
int Menu_Volume_ConfInit(struct menu_item* self)
{
	switch(config.volume) {
	default: config.volume = 0; /* FALL THROUGH */
	case 0: self->conf_sel = 0; break;
	case 1: self->conf_sel = 1; break;
	case 3: self->conf_sel = 2; break;
	case 5: self->conf_sel = 3; break;
	}
}
int Menu_Volume_ConfDone(struct menu_item* self)
{
	int i;
	switch(self->conf_sel) {
	default: self->conf_sel = 0; /* FALL THROUGH */
	case 0: config.volume = 0; break;
	case 1: config.volume = 1; break;
	case 2: config.volume = 3; break;
	case 3: config.volume = 5; break;
	}
	update_volume_config();
	for (i = 0x80; i <= 0x90; i++)
	{
		WriteIO(i, IO[i]);
	}
}

/* Config */

const char* Menu_Config_ConfText[] = {
	"Save Global", "Save Custom", "Delete Custom",
};
int Menu_Config_ConfInit(struct menu_item* self)
{
	switch(config.custom) {
	default: config.custom = 0; /* FALL THROUGH */
	case 0: self->conf_sel = 0; break;
	case 1: self->conf_sel = 1; break;
	}
}
int Menu_Config_Sel(struct menu_item* self)
{
	struct menu *m;
	struct menu_item *mi;
	SDL_Event event;
	int i, ret, item_sel;

	switch(self->conf_sel) {
	default: return 0;
	case 0: ret = save_config(NULL);     break;
	case 1: ret = save_config(gameName); break;
	case 2: ret =  del_config(gameName); break;
	}

	Clear_Menu();
	switch(self->conf_sel) {
	case 1:
		if (ret) config.custom = 1;
		/* FALL THROUGH */
	case 0:
		print_text_center(ret ? "Save config success!" : "Save config failed!", 76);
		break;
	case 2:
		print_text_center(ret ? "Delete config success!" : "Delete config failed!", 76);
		if (!ret) break;
		default_config();
		load_config(NULL);
		m = self->pmenu;
		item_sel = m->item_sel; /* Backup */
		for (i = 0; i < m->item_num; i++) {
			m->item_sel = i;
			mi = &(m->item[i]);
			if (mi->conf_init_call) mi->conf_init_call(mi);
		}
		m->item_sel = item_sel;
		break;
	}
	Update_Screen();

	do {
		SDL_WaitEvent(&event);
	} while (event.type != SDL_KEYDOWN);
}

/* Reset */

int Menu_Reset_Sel(struct menu_item* self)
{
	SDL_Event event;
	do {
		SDL_WaitEvent(&event);
		if (event.type == SDL_KEYDOWN) return 0;
	} while (event.type != SDL_KEYUP);
	WsReset();
	self->pmenu->menu_done = 1;
}

/* Exit */

int Menu_Exit_Sel(struct menu_item* self)
{
	SDL_Event event;
	do {
		SDL_WaitEvent(&event);
		if (event.type == SDL_KEYDOWN) return 0;
	} while (event.type != SDL_KEYUP);
	m_Flag = GF_GAMEQUIT;
	self->pmenu->menu_done = 1;
}

/* Menu Functions */

int Draw_Menu(struct menu* m)
{
	int i;
	int item_x, item_y;
	int font_x, font_y;

	if (!m || m->item_num == 0) return 0;

	if (m->font_x == FONT_X_AUTO) font_x = 2;
	if (m->font_y == FONT_Y_AUTO) font_y = (m->item_h > 8) ? (m->item_h - 7) / 2 : 0;

	item_x = m->menu_x;
	item_y = m->menu_y;

	for (i = 0; i < m->item_num; i++) {
		if(i == m->item_sel) Draw_Rect_Menu(item_x, item_y, m->item_w, m->item_h);
		if (m->item[i].name) {
			print_string(
					m->item[i].name,
					TextWhite, 0,
					item_x + font_x,
					item_y + font_y,
					Surface_to_Draw_menu);
		}
		if (m->item[i].conf_num) {
			print_string(
					m->item[i].conf_text[m->item[i].conf_sel],
					TextWhite, 0,
					item_x + font_x + m->item[i].conf_x, 
					item_y + font_y + m->item[i].conf_y,
					Surface_to_Draw_menu);
		}
		item_y += m->item_h;
	}
	return 0;
}

int Handle_Menu(struct menu* m)
{
	int ret;
	int i;
	SDL_Event event;
	struct menu_item* mi;

	if (!m || m->item_num == 0) return 0;

	m->menu_done = 0;
	for (i = 0; i < m->item_num; i++) {
		m->item_sel = i;
		mi = &(m->item[i]);
		mi->pmenu = m;
		if (mi->conf_init_call) mi->conf_init_call(mi);
	}

	m->item_sel = 0;
	while (!m->menu_done) {
		Clear_Menu();
		if (m->menu_init_call) m->menu_init_call(m);
		Draw_Menu(m);
		Update_Screen();

		do {
			SDL_WaitEvent(&event);
		} while (event.type != SDL_KEYDOWN);
		
		mi = &(m->item[m->item_sel]);
		switch (event.key.keysym.sym) {
		case SDLK_UP:
			if (m->item_sel == 0) m->item_sel = m->item_num;
			--(m->item_sel);
			mi = &(m->item[m->item_sel]);
			break;
		case SDLK_DOWN:
			++(m->item_sel);
			if (m->item_sel == m->item_num) m->item_sel = 0;
			mi = &(m->item[m->item_sel]);
			break;
		case SDLK_RIGHT:
			++(mi->conf_sel);
			if (mi->conf_sel == mi->conf_num) mi->conf_sel = 0;
			if (mi->conf_done_call) mi->conf_done_call(mi);
			break;
		case SDLK_LEFT:
			if (mi->conf_sel == 0) mi->conf_sel = mi->conf_num;
			--(mi->conf_sel);
			if (mi->conf_done_call) mi->conf_done_call(mi);
			break;
		case SDLK_LCTRL: /* A */
			if (mi->sel_call) mi->sel_call(mi);
			if (mi->sub_menu) {
				mi->sub_menu->pitem = mi;
				Handle_Menu(mi->sub_menu);
			}
			break;
		case SDLK_LALT:  /* B */
			if (m->menu_done_call) m->menu_done_call(m);
			m->menu_done = 1;
			break;
		default:
			break;
		}
	}
}

void Menu()
{
	Handle_Menu(&Menu_Main);

	if (m_Flag == GF_MAINUI) m_Flag = GF_GAMERUNNING;

	#ifndef RS90
	SetVideo(config.scaling);
	#endif

	/* Clear the screen before going back to Game or exiting the emulator */
	Clear_Screen();
}

/* Configuration files */

const struct config_line Config_Lines[] = {
	{ "Scaling",       &(config.scaling),                          0,          2,         },
	{ "RemapH",        &(config.remap_mode[0]),        REMAP_MODE_NONE, REMAP_MODE_END-1, },
	{ "RemapV",        &(config.remap_mode[1]),        REMAP_MODE_NONE, REMAP_MODE_END-1, },
	{ "Volume",        &(config.volume),                           0,          5,         },
	{ "ButtonH_Y1",    &(keys_config[HC_H].buttons[HC_KEY_Y1]),    SDLK_FIRST, SDLK_LAST, },
	{ "ButtonH_Y2",    &(keys_config[HC_H].buttons[HC_KEY_Y2]),    SDLK_FIRST, SDLK_LAST, },
	{ "ButtonH_Y3",    &(keys_config[HC_H].buttons[HC_KEY_Y3]),    SDLK_FIRST, SDLK_LAST, },
	{ "ButtonH_Y4",    &(keys_config[HC_H].buttons[HC_KEY_Y4]),    SDLK_FIRST, SDLK_LAST, },
	{ "ButtonH_X1",    &(keys_config[HC_H].buttons[HC_KEY_X1]),    SDLK_FIRST, SDLK_LAST, },
	{ "ButtonH_X2",    &(keys_config[HC_H].buttons[HC_KEY_X2]),    SDLK_FIRST, SDLK_LAST, },
	{ "ButtonH_X3",    &(keys_config[HC_H].buttons[HC_KEY_X3]),    SDLK_FIRST, SDLK_LAST, },
	{ "ButtonH_X4",    &(keys_config[HC_H].buttons[HC_KEY_X4]),    SDLK_FIRST, SDLK_LAST, },
	{ "ButtonH_Start", &(keys_config[HC_H].buttons[HC_KEY_START]), SDLK_FIRST, SDLK_LAST, },
	{ "ButtonH_A",     &(keys_config[HC_H].buttons[HC_KEY_BTN_A]), SDLK_FIRST, SDLK_LAST, },
	{ "ButtonH_B",     &(keys_config[HC_H].buttons[HC_KEY_BTN_B]), SDLK_FIRST, SDLK_LAST, },
	{ "ButtonH_Remap", &(keys_config[HC_H].buttons[HC_KEY_REMAP]), SDLK_FIRST, SDLK_LAST, },
	{ "ButtonH_EMenu", &(keys_config[HC_H].buttons[HC_KEY_EMENU]), SDLK_FIRST, SDLK_LAST, },
	{ "ButtonV_Y1",    &(keys_config[HC_V].buttons[HC_KEY_Y1]),    SDLK_FIRST, SDLK_LAST, },
	{ "ButtonV_Y2",    &(keys_config[HC_V].buttons[HC_KEY_Y2]),    SDLK_FIRST, SDLK_LAST, },
	{ "ButtonV_Y3",    &(keys_config[HC_V].buttons[HC_KEY_Y3]),    SDLK_FIRST, SDLK_LAST, },
	{ "ButtonV_Y4",    &(keys_config[HC_V].buttons[HC_KEY_Y4]),    SDLK_FIRST, SDLK_LAST, },
	{ "ButtonV_X1",    &(keys_config[HC_V].buttons[HC_KEY_X1]),    SDLK_FIRST, SDLK_LAST, },
	{ "ButtonV_X2",    &(keys_config[HC_V].buttons[HC_KEY_X2]),    SDLK_FIRST, SDLK_LAST, },
	{ "ButtonV_X3",    &(keys_config[HC_V].buttons[HC_KEY_X3]),    SDLK_FIRST, SDLK_LAST, },
	{ "ButtonV_X4",    &(keys_config[HC_V].buttons[HC_KEY_X4]),    SDLK_FIRST, SDLK_LAST, },
	{ "ButtonV_Start", &(keys_config[HC_V].buttons[HC_KEY_START]), SDLK_FIRST, SDLK_LAST, },
	{ "ButtonV_A",     &(keys_config[HC_V].buttons[HC_KEY_BTN_A]), SDLK_FIRST, SDLK_LAST, },
	{ "ButtonV_B",     &(keys_config[HC_V].buttons[HC_KEY_BTN_B]), SDLK_FIRST, SDLK_LAST, },
	{ "ButtonV_Remap", &(keys_config[HC_V].buttons[HC_KEY_REMAP]), SDLK_FIRST, SDLK_LAST, },
	{ "ButtonV_EMenu", &(keys_config[HC_V].buttons[HC_KEY_EMENU]), SDLK_FIRST, SDLK_LAST, },
	{ NULL,            NULL,                                       0,          0,         },
};

void default_config()
{
	/* Default profile */
	config.custom = 0;
	config.remap_mode[HC_H] = REMAP_MODE_HOLDY2X;
#ifndef RS90
	config.remap_mode[HC_V] = REMAP_MODE_NONE;
#else
	config.remap_mode[HC_V] = REMAP_MODE_ACTIVATE;
#endif
	config.scaling = 0;
	config.volume = 5;

	/* Should work for landscape. They can always configure it themselves should they need portrait mode. */
	memset(&keys_config, 0, sizeof(keys_config));
#ifndef RS90
	keys_config[HC_H].buttons[HC_KEY_Y1] = SDLK_UNKNOWN;
	keys_config[HC_H].buttons[HC_KEY_Y2] = SDLK_UNKNOWN;
	keys_config[HC_H].buttons[HC_KEY_Y3] = SDLK_UNKNOWN;
	keys_config[HC_H].buttons[HC_KEY_Y4] = SDLK_UNKNOWN;
	keys_config[HC_H].buttons[HC_KEY_X1] = SDLK_UP;
	keys_config[HC_H].buttons[HC_KEY_X2] = SDLK_RIGHT;
	keys_config[HC_H].buttons[HC_KEY_X3] = SDLK_DOWN;
	keys_config[HC_H].buttons[HC_KEY_X4] = SDLK_LEFT;
	keys_config[HC_H].buttons[HC_KEY_START] = SDLK_RETURN;
	keys_config[HC_H].buttons[HC_KEY_BTN_A] = SDLK_LCTRL;
	keys_config[HC_H].buttons[HC_KEY_BTN_B] = SDLK_LALT;
	keys_config[HC_H].buttons[HC_KEY_REMAP] = SDLK_TAB;
	keys_config[HC_H].buttons[HC_KEY_EMENU] = SDLK_ESCAPE;

	keys_config[HC_V].buttons[HC_KEY_Y1] = SDLK_LEFT;
	keys_config[HC_V].buttons[HC_KEY_Y2] = SDLK_UP;
	keys_config[HC_V].buttons[HC_KEY_Y3] = SDLK_RIGHT;
	keys_config[HC_V].buttons[HC_KEY_Y4] = SDLK_DOWN;
	keys_config[HC_V].buttons[HC_KEY_X1] = SDLK_LSHIFT;
	keys_config[HC_V].buttons[HC_KEY_X2] = SDLK_SPACE;
	keys_config[HC_V].buttons[HC_KEY_X3] = SDLK_LCTRL;
	keys_config[HC_V].buttons[HC_KEY_X4] = SDLK_LALT;
	keys_config[HC_V].buttons[HC_KEY_START] = SDLK_RETURN;
	keys_config[HC_V].buttons[HC_KEY_BTN_A] = SDLK_BACKSPACE;
	keys_config[HC_V].buttons[HC_KEY_BTN_B] = SDLK_TAB;
	keys_config[HC_V].buttons[HC_KEY_REMAP] = SDLK_UNKNOWN;
	keys_config[HC_V].buttons[HC_KEY_EMENU] = SDLK_ESCAPE;
#else
	keys_config[HC_H].buttons[HC_KEY_Y1] = SDLK_UNKNOWN;
	keys_config[HC_H].buttons[HC_KEY_Y2] = SDLK_UNKNOWN;
	keys_config[HC_H].buttons[HC_KEY_Y3] = SDLK_UNKNOWN;
	keys_config[HC_H].buttons[HC_KEY_Y4] = SDLK_UNKNOWN;
	keys_config[HC_H].buttons[HC_KEY_X1] = SDLK_UP;
	keys_config[HC_H].buttons[HC_KEY_X2] = SDLK_RIGHT;
	keys_config[HC_H].buttons[HC_KEY_X3] = SDLK_DOWN;
	keys_config[HC_H].buttons[HC_KEY_X4] = SDLK_LEFT;
	keys_config[HC_H].buttons[HC_KEY_START] = SDLK_RETURN;
	keys_config[HC_H].buttons[HC_KEY_BTN_A] = SDLK_LCTRL;
	keys_config[HC_H].buttons[HC_KEY_BTN_B] = SDLK_LALT;
	keys_config[HC_H].buttons[HC_KEY_REMAP] = SDLK_TAB;
	keys_config[HC_H].buttons[HC_KEY_EMENU] = SDLK_ESCAPE;

	keys_config[HC_V].buttons[HC_KEY_Y1] = SDLK_UP;
	keys_config[HC_V].buttons[HC_KEY_Y2] = SDLK_RIGHT;
	keys_config[HC_V].buttons[HC_KEY_Y3] = SDLK_DOWN;
	keys_config[HC_V].buttons[HC_KEY_Y4] = SDLK_LEFT;
	keys_config[HC_V].buttons[HC_KEY_X1] = SDLK_LALT;
	keys_config[HC_V].buttons[HC_KEY_X2] = SDLK_LCTRL;
	keys_config[HC_V].buttons[HC_KEY_X3] = SDLK_RETURN;
	keys_config[HC_V].buttons[HC_KEY_X4] = SDLK_ESCAPE;
	keys_config[HC_V].buttons[HC_KEY_START] = SDLK_BACKSPACE;
	keys_config[HC_V].buttons[HC_KEY_BTN_A] = SDLK_UNKNOWN;
	keys_config[HC_V].buttons[HC_KEY_BTN_B] = SDLK_UNKNOWN;
	keys_config[HC_V].buttons[HC_KEY_REMAP] = SDLK_TAB;
	keys_config[HC_V].buttons[HC_KEY_EMENU] = SDLK_UNKNOWN;
#endif
	update_all_config();
}

int load_config(const char *gamepath)
{
	char path[PATH_MAX];
	FILE* fp;
	const struct config_line *cfg;
	char line[CONFIG_LINE_MAX], *linep, *token1, *token2;
	int state, value;
	const char *cfgname;

	if (gamepath) {
		cfgname = strrchr(gamepath, '/');
		cfgname = cfgname ? cfgname + 1 : gamepath;
	} else {
		cfgname = "oswan";
	}

	memset(path, 0, sizeof(path));
	snprintf(path, sizeof(path) - 1, "%s%s", PATH_DIRECTORY, SAVE_DIRECTORY);
	if (access(path, F_OK ) == -1) mkdir(path, 0755);
	snprintf(path, sizeof(path) - 1, "%s%s%s.cfg", PATH_DIRECTORY, SAVE_DIRECTORY, cfgname);

	if ((fp = fopen(path, "r")) == NULL) return 0;
	
	while (fgets(line, sizeof(line), fp) != NULL) {
		state = 1;
		token1 = NULL, token2 = NULL, linep = &line[0];
		while (*linep && state && state != 7) {
			switch (*linep) {
			case '\0': /* Impossible? */
				if (state <= 4) state = 0;
				break;
			case '#':
				if (state <= 4) state = 0;
				else *linep = '\0';
				break;
			case '\r':
			case '\n':
				if (state <= 4) state = 0;
				else *(linep++) = '\0';
				break;
			case '=':
				if (state++ != 3) state = 0;
				else *(linep++) = '\0';
				break;
			case ' ':
			case '\t':
				*(linep++) = '\0';
				break;
			default:
				switch (state){
				case 1: token1 = linep++, state++; break;
				case 4: token2 = linep++, state++; break;
				case 2:
				case 5: if (strchr(" \t=#\r\n", *(++linep))) state++; break;
				default: state = 0;
				}
			}
		}
		if (state <= 4) continue;

		for (cfg = &Config_Lines[0]; cfg && cfg->name; cfg++) {
			if (!(cfg->ref) || (cfg->min > cfg->max)) continue;
			if (strcmp(cfg->name, token1)) continue;
			value = strtol(token2, NULL, 0);
			value = (value < cfg->min) ? cfg->min : ((value > cfg->max) ? cfg->max : value);
			*(cfg->ref) = value;
		}
	}
	fclose(fp);
	update_all_config();

	return 1;
}


int save_config(const char *gamepath)
{
	char path[PATH_MAX];
	FILE* fp;
	const struct config_line *cfg;
	const char *cfgname;

	if (gamepath) {
		cfgname = strrchr(gamepath, '/');
		cfgname = cfgname ? cfgname + 1 : gamepath;
	} else {
		cfgname = "oswan";
	}

	memset(path, 0, sizeof(path));
	snprintf(path, sizeof(path) - 1, "%s%s%s.cfg", PATH_DIRECTORY, SAVE_DIRECTORY, cfgname);

	if ((fp = fopen(path, "w")) == NULL) return 0;
	
	fprintf(fp, "# Generated by Oswan+\n");
	fprintf(fp, "# Config of %s\n", cfgname);
	for (cfg = &Config_Lines[0]; cfg && cfg->name; cfg++) {
		if (!(cfg->ref) || (cfg->min > cfg->max)) continue;
		fprintf(fp, "%s = %d\n", cfg->name, (int)*(cfg->ref));
	}
	fclose(fp);

	return 1;
}

int del_config(const char *gamepath)
{
	char path[PATH_MAX];
	const char *cfgname;

	if (gamepath) {
		cfgname = strrchr(gamepath, '/');
		cfgname = cfgname ? cfgname + 1 : gamepath;
	} else {
		return 0;
	}

	memset(path, 0, sizeof(path));
	snprintf(path, sizeof(path) - 1, "%s%s%s.cfg", PATH_DIRECTORY, SAVE_DIRECTORY, cfgname);

	return remove(path) ? 0 : 1;
}

#define COMBINE_MENU_KEYS(x, y) ((x) << 16 | (y))
int check_menu_keys(SDLKey key1, SDLKey key2, int keys)
{
	int i;
	for (i = 0; i < keys; i++) {
		switch (i ? key2 : key1) {
#ifndef RS90
		case SDLK_LSHIFT:
		case SDLK_SPACE:
		case SDLK_PAGEUP:
		case SDLK_PAGEDOWN:
#endif
		case SDLK_TAB:
		case SDLK_ESCAPE:
		case SDLK_UP:
		case SDLK_DOWN:
		case SDLK_LEFT:
		case SDLK_RIGHT:
		case SDLK_LCTRL:
		case SDLK_LALT:
		case SDLK_BACKSPACE:
		case SDLK_RETURN:
			break;
		default:
			return 0;
		}
	}

	if (keys == 1) return 1;

	switch (COMBINE_MENU_KEYS(key1, key2)) {
	case COMBINE_MENU_KEYS(SDLK_UP, SDLK_DOWN):
	case COMBINE_MENU_KEYS(SDLK_DOWN, SDLK_UP):
	case COMBINE_MENU_KEYS(SDLK_LEFT, SDLK_RIGHT):
	case COMBINE_MENU_KEYS(SDLK_RIGHT, SDLK_LEFT):
#if 1 /* In some cases L + Select works? */
	case COMBINE_MENU_KEYS(SDLK_ESCAPE, SDLK_TAB):
	case COMBINE_MENU_KEYS(SDLK_TAB, SDLK_ESCAPE):
#endif
		return 0;
	}

	return 1;
}

void update_all_config()
{
	update_volume_config();
	update_key_config();
}

void update_volume_config()
{
	switch(config.volume) {
		default: config.volume = 5; /* FALL THROUGH */
		case 5:  lcd_icon_stat[LCD_INDEX__VOLUME] = LCD_ICON__VOLUME_L3; break;
		case 4:  config.volume = 3; /* FALL THROUGH */
		case 3:  lcd_icon_stat[LCD_INDEX__VOLUME] = LCD_ICON__VOLUME_L2; break;
		case 2:  config.volume = 1; /* FALL THROUGH */
		case 1:  lcd_icon_stat[LCD_INDEX__VOLUME] = LCD_ICON__VOLUME_L1; break;
		case 0:  lcd_icon_stat[LCD_INDEX__VOLUME] = LCD_ICON__VOLUME_MUTE;
	}
}

void update_key_config()
{
	int i, a;
	uint32_t key;
	
	for (i = 0; i < HC_END; i++) {
		/* add remap configs */
		for (a = 0; a < HC_KEY_END; a++) {
			if (a == HC_KEY_OPTION) continue;
			key = keys_config[i].buttons[a];
			switch(config.remap_mode[i]) {
			default:
			case REMAP_MODE_NONE:
				break;
			case REMAP_MODE_HOLDY2X:
			case REMAP_MODE_HOLDX2Y:
			case REMAP_MODE_PRESSXY:
				switch(a) {
				case HC_KEY_OPTION: break;
				case HC_KEY_Y1: keys_config[i + 2].buttons[HC_KEY_X1] = key; break;
				case HC_KEY_Y2: keys_config[i + 2].buttons[HC_KEY_X2] = key; break;
				case HC_KEY_Y3: keys_config[i + 2].buttons[HC_KEY_X3] = key; break;
				case HC_KEY_Y4: keys_config[i + 2].buttons[HC_KEY_X4] = key; break;
				case HC_KEY_X1: keys_config[i + 2].buttons[HC_KEY_Y1] = key; break;
				case HC_KEY_X2: keys_config[i + 2].buttons[HC_KEY_Y2] = key; break;
				case HC_KEY_X3: keys_config[i + 2].buttons[HC_KEY_Y3] = key; break;
				case HC_KEY_X4: keys_config[i + 2].buttons[HC_KEY_Y4] = key; break;
				default:        keys_config[i + 2].buttons[a] = key;
				}
				break;
			case REMAP_MODE_ACTIVATE:
				switch(a) {
				case HC_KEY_OPTION: break;
				case HC_KEY_START: 
					keys_config[i + 2].buttons[HC_KEY_BTN_A] = key;
					keys_config[i + 4].buttons[HC_KEY_BTN_B] = key;
					break;
				case HC_KEY_BTN_A: 
					keys_config[i + 2].buttons[HC_KEY_START] = key;
					keys_config[i + 4].buttons[HC_KEY_BTN_A] = key;
					break;
				case HC_KEY_BTN_B: 
					keys_config[i + 2].buttons[HC_KEY_BTN_B] = key;
					keys_config[i + 4].buttons[HC_KEY_START] = key;
					break;
				default: 
					keys_config[i + 2].buttons[a] = key;
					keys_config[i + 4].buttons[a] = key;
				}
			}
		}

		/* reset remap state */
		remap_state[i] = 0;
		#ifdef SHOW_LCD_ICON
		input_icon_reload = 1;
		#endif

		/* emu menu keys */
		if (check_menu_keys(keys_config[i].buttons[HC_KEY_EMENU], SDLK_UNKNOWN, 1)) {
			menu_key[i][0] = &keys_config[i].buttons[HC_KEY_EMENU];
			menu_key[i][1] = NULL;
		} else if (check_menu_keys(keys_config[i].buttons[HC_KEY_REMAP], keys_config[i].buttons[HC_KEY_START], 2)) {
			menu_key[i][0] = &keys_config[i].buttons[HC_KEY_REMAP];
			menu_key[i][1] = &keys_config[i].buttons[HC_KEY_START];
		} else {
			menu_key[i][0] = NULL;
			menu_key[i][1] = NULL;
		}
	}
}

/* Menu code */
