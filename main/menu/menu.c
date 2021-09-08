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

struct Menu__ menu_oswan;

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
	case SDLK_LSHIFT	:	return  7;	/* X button */
	case SDLK_SPACE		:	return  8;	/* Y button */
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
	case  7	:	return SDLK_LSHIFT		;	/* X button */
	case  8	:	return SDLK_SPACE		;	/* Y button */
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
	.menu_x			= 0,
	.menu_y			= 24,
	.menu_init_call	= Menu_Main_Init,
	.menu_done_call	= NULL,
	.item_num		= 8,
	.item_w			= 240,
	.item_h			= 16,
	.item			= Menu_Main_Item,
	.font_x			= FONT_X_AUTO,
	.font_y			= FONT_Y_AUTO,
};

int Menu_Main_Init(struct menu* self)
{
	print_text_center("Gameblabla's Oswan", 6);
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

const char*	Menu_Sound_ConfText[];
int			Menu_Sound_ConfInit(struct menu_item* self);
int			Menu_Sound_ConfDone(struct menu_item* self);

int			Menu_Reset_Sel(struct menu_item* self);
const char*	Menu_Reset_ConfText[];
int			Menu_Reset_ConfInit(struct menu_item* self);

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
		.name			= "Sound",
		.sel_call		= NULL,
		.sub_menu		= NULL,
		.conf_num		= 2,
		.conf_x			= 120,
		.conf_y			= 0,
		.conf_text		= Menu_Sound_ConfText,
		.conf_init_call	= Menu_Sound_ConfInit,
		.conf_done_call	= Menu_Sound_ConfDone,
	}, {
		.name			= "Reset",
		.sel_call		= Menu_Reset_Sel,
		.sub_menu		= NULL,
		.conf_num		= 2,
		.conf_x			= 120,
		.conf_y			= 0,
		.conf_text		= Menu_Reset_ConfText,
		.conf_init_call	= Menu_Reset_ConfInit,
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
	self->pmenu->menu_done = 1;
}

/* Save/Load State */

int Menu_Load_State_Sel(struct menu_item* self)
{
	if (!cartridge_IsLoaded()) return 0;
	WsLoadState(gameName, self->conf_sel);
	self->pmenu->menu_done = 1;
}
int Menu_Save_State_Sel(struct menu_item* self)
{
	if (!cartridge_IsLoaded()) return 0;
	WsSaveState(gameName, self->conf_sel);
	self->pmenu->menu_done = 1;
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
#ifndef RS90
	"Native",
	"Fullscreen",
	"Keep Aspect",
#else /* Compatible with upstream */
	"Native",
	"Still Native",
	"Always Native",
#endif
};
int Menu_Scaling_ConfInit(struct menu_item* self)
{
	if (menu_oswan.scaling < 0 || menu_oswan.scaling > self->conf_num) {
		menu_oswan.scaling = 0;
	}
	self->conf_sel = menu_oswan.scaling;
}
int Menu_Scaling_ConfDone(struct menu_item* self)
{
	menu_oswan.scaling = self->conf_sel;
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
	.menu_x			= 0,
	.menu_y			= 0,
	.menu_init_call	= Menu_Input_MenuInit,
	.menu_done_call	= NULL,
	.item_num		= 14,
	.item_w			= 264,
	.item_h			= 10,
	.item			= Menu_Input_Item,
	.font_x			= FONT_X_AUTO,
	.font_y			= FONT_Y_AUTO,
};

int Menu_Input_MenuInit(struct menu* self)
{
	int remap_mode, input_mode;

	update_remap_config();

	remap_mode = self->item[13].conf_sel;
	switch (remap_mode) {
	case REMAP_MODE_HOLDY2X:
		print_string("Hold         : Overlap Y to X",
				TextWhite, 0,     0, 142, Surface_to_Draw_menu);
		print_string(self->item[12].conf_text[self->item[12].conf_sel],
				TextWhite, 0, 6 * 8, 142, Surface_to_Draw_menu);
		break;
	case REMAP_MODE_HOLDX2Y:
		print_string("Hold         : Overlap X to Y",
				TextWhite, 0,     0, 142, Surface_to_Draw_menu);
		print_string(self->item[12].conf_text[self->item[12].conf_sel],
				TextWhite, 0, 6 * 8, 142, Surface_to_Draw_menu);
		break;
	case REMAP_MODE_PRESSXY:
		print_string("Press        : Swap X and Y",
				TextWhite, 0,     0, 142, Surface_to_Draw_menu);
		print_string(self->item[12].conf_text[self->item[12].conf_sel],
				TextWhite, 0, 6 * 8, 142, Surface_to_Draw_menu);
		break;
	case REMAP_MODE_SWAPOPT:
		print_string("Press        : START\035OPTN\035A\035B",
				TextWhite, 0,     0, 142, Surface_to_Draw_menu);
		print_string(self->item[12].conf_text[self->item[12].conf_sel],
				TextWhite, 0, 6 * 8, 142, Surface_to_Draw_menu);
		break;
	}

	input_mode = self->pitem->conf_sel;
	if (menu_key[input_mode][0] == NULL || menu_key[input_mode][1] == NULL) {
		print_string("+      : Oswan Emu Menu",
				TextWhite, 0, 6 * 8, 152, Surface_to_Draw_menu);
		print_string(Menu_Input_Button_ConfText[SDLK2InputSel(SDLK_ESCAPE)],
				TextWhite, 0, 0,     152, Surface_to_Draw_menu);
		print_string(Menu_Input_Button_ConfText[SDLK2InputSel(SDLK_RETURN)],
				TextWhite, 0, 7 * 8, 152, Surface_to_Draw_menu);
	} else {
		print_string("+      : Oswan Emu Menu",
				TextWhite, 0, 6 * 8, 152, Surface_to_Draw_menu);
		print_string(Menu_Input_Button_ConfText[SDLK2InputSel(*menu_key[input_mode][0])],
				TextWhite, 0, 0,     152, Surface_to_Draw_menu);
		print_string(Menu_Input_Button_ConfText[SDLK2InputSel(*menu_key[input_mode][1])],
				TextWhite, 0, 7 * 8, 152, Surface_to_Draw_menu);
	}
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
	.conf_x			= 144,							\
	.conf_y			= 0,							\
	.conf_text		= Menu_Input_Button_ConfText,	\
	.conf_init_call	= Menu_Input_Button_ConfInit,	\
	.conf_done_call	= Menu_Input_Button_ConfDone,	\
}

struct menu_item Menu_Input_Item[] = {
	MENU_INPUT_ITEM("Y1 (\030)"),
	MENU_INPUT_ITEM("Y2 (\032)"),
	MENU_INPUT_ITEM("Y3 (\031)"),
	MENU_INPUT_ITEM("Y4 (\033)"),
	MENU_INPUT_ITEM("X1 (\030)"),
	MENU_INPUT_ITEM("X2 (\032)"),
	MENU_INPUT_ITEM("X3 (\031)"),
	MENU_INPUT_ITEM("X4 (\033)"),
	MENU_INPUT_ITEM("OPTION"   ),
	MENU_INPUT_ITEM("START"    ),
	MENU_INPUT_ITEM("BUTTON A" ),
	MENU_INPUT_ITEM("BUTTON B" ),
	MENU_INPUT_ITEM("Remap Switch"),
	{
		.name			= "Remap Mode",
		.sel_call		= NULL,
		.sub_menu		= NULL,
		.conf_num		= 5,
		.conf_x			= 144,
		.conf_y			= 0,
		.conf_text		= Menu_Input_RemapMode_ConfText,
		.conf_init_call	= Menu_Input_RemapMode_ConfInit,
		.conf_done_call	= Menu_Input_RemapMode_ConfDone,
	}
};

int Menu_Input_Button_Sel(struct menu_item* self)
{
	int type, item_sel;
	SDL_Event event;

	type = self->pmenu->pitem->conf_sel;
	item_sel = self->pmenu->item_sel;
	if (type != 0 && type != 1) return 0;
	if (item_sel < 0 || item_sel >= self->pmenu->item_num) return 0;

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
	if (type != 0 && type != 1) return 0;
	if (item_sel < 0 || item_sel >= self->pmenu->item_num) return 0;

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
	if (type != 0 && type != 1) return 0;
	if (item_sel < 0 || item_sel >= self->pmenu->item_num) return 0;

	key = InputSel2SDLK(self->conf_sel);
	keys_config[type].buttons[item_sel] = key;
	return 0;
}

const char*	Menu_Input_RemapMode_ConfText[] = {
	"None",
	"Hold Y\032X",
	"Hold X\032Y",
	"Press X\035Y",
	"Swap START",
};
int Menu_Input_RemapMode_ConfInit(struct menu_item* self)
{
	int type, item_sel;
	SDLKey key;

	type = self->pmenu->pitem->conf_sel;
	item_sel = self->pmenu->item_sel;
	if (type != 0 && type != 1) return 0;
	if (item_sel < 0 || item_sel >= self->pmenu->item_num) return 0;

	key = keys_config[type].buttons[item_sel];
	self->conf_sel = key;
	return 0;
}

int Menu_Input_RemapMode_ConfDone(struct menu_item* self)
{
	int type, item_sel;
	SDLKey key;

	type = self->pmenu->pitem->conf_sel;
	item_sel = self->pmenu->item_sel;
	if (type != 0 && type != 1) return 0;
	if (item_sel < 0 || item_sel >= self->pmenu->item_num) return 0;

	keys_config[type].buttons[item_sel] = self->conf_sel;
	return 0;
}

/* Sound */

const char* Menu_Sound_ConfText[] = {
	"OFF", "ON",
};
int Menu_Sound_ConfInit(struct menu_item* self)
{
	self->conf_sel = sound_on;
}
int Menu_Sound_ConfDone(struct menu_item* self)
{
	sound_on = self->conf_sel;
}

/* Reset */

int Menu_Reset_Sel(struct menu_item* self)
{
	if (self->conf_sel != 1) return 0;
	WsReset();
	self->pmenu->menu_done = 1;
}
const char* Menu_Reset_ConfText[] = {
	"NO", "YES",
};
int Menu_Reset_ConfInit(struct menu_item* self)
{
	self->conf_sel = 0;
}

/* Exit */

int Menu_Exit_Sel(struct menu_item* self)
{
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

	SetVideo(menu_oswan.scaling);

	/* Clear the screen before going back to Game or exiting the emulator */
	Clear_Screen();
}

/* Configuration files */

void load_config(void)
{
	uint32_t i, a;
	char home_path[PATH_MAX], cfg_path[PATH_MAX];
	FILE* fp;
	
	snprintf(home_path, sizeof(home_path), "%s%s", PATH_DIRECTORY, SAVE_DIRECTORY);
	snprintf(cfg_path, sizeof(cfg_path), "%sconfig.bin", home_path);
	
	if (access( home_path, F_OK ) == -1)
	{
		mkdir(home_path, 0755);
	}
	
	fp = fopen(cfg_path, "rb");
	if (fp)
	{
		fread(&menu_oswan, sizeof(uint8_t), sizeof(menu_oswan), fp);
		fread(&keys_config, sizeof(uint8_t), (14*2)*sizeof(uint32_t), fp);
		fclose(fp);
	}
	else
	{
		/* Set default settings */
		for (i=0;i<8;i++)
		{
			for (a=0;a<12;a++)
			{
				keys_config[i].buttons[a] = 0;
			}
		}
		
		/* Default profile */
		/* Should work for landscape. They can always configure it themselves should they need portrait mode. */
		keys_config[0].buttons[0] = SDLK_UNKNOWN;
		keys_config[0].buttons[1] = SDLK_UNKNOWN;
		keys_config[0].buttons[2] = SDLK_UNKNOWN;
		keys_config[0].buttons[3] = SDLK_UNKNOWN;
		keys_config[0].buttons[4] = SDLK_UP;
		keys_config[0].buttons[5] = SDLK_RIGHT;
		keys_config[0].buttons[6] = SDLK_DOWN;
		keys_config[0].buttons[7] = SDLK_LEFT;
		keys_config[0].buttons[8] = SDLK_ESCAPE;
		keys_config[0].buttons[9] = SDLK_RETURN;
		keys_config[0].buttons[10] = SDLK_LCTRL;
		keys_config[0].buttons[11] = SDLK_LALT;
		
#ifndef RS90
		keys_config[1].buttons[0] = SDLK_LEFT;
		keys_config[1].buttons[1] = SDLK_UP;
		keys_config[1].buttons[2] = SDLK_RIGHT;
		keys_config[1].buttons[3] = SDLK_DOWN;
#else
		keys_config[1].buttons[0] = SDLK_UP;
		keys_config[1].buttons[1] = SDLK_RIGHT;
		keys_config[1].buttons[2] = SDLK_DOWN;
		keys_config[1].buttons[3] = SDLK_LEFT;
#endif
		
		keys_config[1].buttons[4] = SDLK_LCTRL;
		keys_config[1].buttons[5] = SDLK_LALT;
#ifndef RS90
		keys_config[1].buttons[6] = SDLK_LSHIFT;
		keys_config[1].buttons[7] = SDLK_SPACE;
#else
		keys_config[1].buttons[6] = SDLK_TAB;
		keys_config[1].buttons[7] = SDLK_BACKSPACE;
#endif
		
		keys_config[1].buttons[8] = SDLK_ESCAPE;
		keys_config[1].buttons[9] = SDLK_RETURN;
				
		keys_config[1].buttons[10] = SDLK_UNKNOWN;
		keys_config[1].buttons[11] = SDLK_UNKNOWN;
	}

	sound_on = 1;
	update_remap_config();
}


void save_config(void)
{
	char home_path[PATH_MAX], cfg_path[PATH_MAX];
	FILE* fp;

	snprintf(home_path, sizeof(home_path), "%s%s", PATH_DIRECTORY, SAVE_DIRECTORY);
	snprintf(cfg_path, sizeof(cfg_path), "%sconfig.bin", home_path);
	
	if (access( home_path, F_OK ) == -1)
	{
		mkdir(home_path, 0755);
	}
	
	fp = fopen(cfg_path, "wb");
	if (fp)
	{
		fwrite(&menu_oswan, sizeof(uint8_t), sizeof(menu_oswan), fp);
		fwrite(&keys_config, sizeof(uint8_t), (14*2)*sizeof(uint32_t), fp);
		fclose(fp);
	}
}

void update_remap_config()
{
	int i, a;
	uint32_t key;
	
	for (i = 0; i < 2; i++) {
		/* add remap configs */
		for (a = 0; a < 12; a++) {
			key = keys_config[i].buttons[a];
			switch(keys_config[i].buttons[13]) {
			default:
			case REMAP_MODE_NONE:
				break;
			case REMAP_MODE_HOLDY2X:
			case REMAP_MODE_HOLDX2Y:
			case REMAP_MODE_PRESSXY:
				switch(a) {
				case 0:
				case 1:
				case 2:
				case 3:
					keys_config[i + 2].buttons[a + 4] = key;
					break;
				case 4:
				case 5:
				case 6:
				case 7:
					keys_config[i + 2].buttons[a - 4] = key;
					break;
				default:
					keys_config[i + 2].buttons[a] = key;
				}
				break;
			case REMAP_MODE_SWAPOPT:
				switch(a) {
				case 8: 
					keys_config[i + 2].buttons[9] = key;
					keys_config[i + 4].buttons[8] = key;
					keys_config[i + 6].buttons[8] = key;
					break;
				case 9: 
					keys_config[i + 2].buttons[8] = key;
					keys_config[i + 4].buttons[10] = key;
					keys_config[i + 6].buttons[11] = key;
					break;
				case 10: 
					keys_config[i + 2].buttons[10] = key;
					keys_config[i + 4].buttons[9] = key;
					keys_config[i + 6].buttons[10] = key;
					break;
				case 11: 
					keys_config[i + 2].buttons[11] = key;
					keys_config[i + 4].buttons[11] = key;
					keys_config[i + 6].buttons[9] = key;
					break;
				default: 
					keys_config[i + 2].buttons[a] = key;
					keys_config[i + 4].buttons[a] = key;
					keys_config[i + 6].buttons[a] = key;
				}
			}
		}

		/* reset remap state */
		remap_state[i] = 0;

		/* emu menu */
		switch(keys_config[i].buttons[12]) {
		default:
			menu_key[i][0] = NULL;
			menu_key[i][1] = NULL;
			break;
#ifndef RS90
		case SDLK_LSHIFT:
		case SDLK_SPACE:
		case SDLK_PAGEUP:
		case SDLK_PAGEDOWN:
#endif
		case SDLK_UP:
		case SDLK_DOWN:
		case SDLK_LEFT:
		case SDLK_RIGHT:
		case SDLK_LCTRL:
		case SDLK_LALT:
		case SDLK_TAB:
		case SDLK_BACKSPACE:
		case SDLK_RETURN:
		case SDLK_ESCAPE:
			menu_key[i][0] = &keys_config[i].buttons[12];
			menu_key[i][1] = &keys_config[i].buttons[9];
		}
	}
}

/* Save states */

/* Save current state of game emulated	*/
void Save_State(void) 
{
    char szFile[PATH_MAX];
	
	if (cartridge_IsLoaded()) 
	{
		m_Flag = GF_GAMERUNNING;
		strcpy(szFile, gameName);
		WsSaveState(szFile, menu_oswan.menu_state);
		done_menu = 1;
	}
}

/* Load current state of game emulated	*/
void Load_State(void) 
{
    char szFile[PATH_MAX];
	
	if (cartridge_IsLoaded()) 
	{
		m_Flag = GF_GAMERUNNING;
		strcpy(szFile, gameName);
		WsLoadState(szFile, menu_oswan.menu_state);
		done_menu = 1;
	}
}

/* Menu code */
