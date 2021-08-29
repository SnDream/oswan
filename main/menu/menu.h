#ifndef MENU_H
#define MENU_H

#include <stdint.h>

#define RGB565(r,g,b) ((r << 8) | (g << 3) | (b >> 3))

#define Y_MAIN_MENU \
	(  (48 + (menu_oswan.Choose_Menu_value * 24)) - 4)
	
#define Y_MENU_CONTROLS \
	( (48 + (menu_oswan.Choose_Menu_value * 16)) - 4)

#define MAX_TEXT_SIZE 48

#define EMULATOR_MAIN_MENU 0
#define CONTROLS_MENU 1
#define SETTINGS_KEY_SCREEN 2
#define CONTROLS_MENU_NOSET 3

struct Menu__
{
	uint32_t Choose_Menu_value;
	uint32_t maximum_menu;
	uint32_t state_number;
	uint32_t scaling;
	uint32_t menu_state;
};

#define FONT_X_AUTO (-1)
#define FONT_Y_AUTO (-1)

struct menu_item;

struct menu {
	const int				menu_x;
	const int				menu_y;
	int (*const				menu_init_call)	(struct menu* self);
	int (*const 			menu_done_call)	(struct menu* self);
	const int				item_num;
	const int				item_w;
	const int				item_h;
	struct menu_item* const	item;
	const int				font_x;
	const int				font_y;
	struct menu_item*		pitem;
	int						menu_done;
	int						item_sel;
};

struct menu_item {
	const char*				name;
	int (*const 			sel_call)		(struct menu_item *self);
	struct menu* const		sub_menu;
	const int				conf_num;
	const int				conf_x;
	const int				conf_y;
	const char** const		conf_text;
	int (* const			conf_init_call)	(struct menu_item* self);
	int (* const			conf_done_call)	(struct menu_item* self);
	struct menu*			pmenu;
	int						conf_sel;
};

#define REMAP_MODE_NONE		0
#define REMAP_MODE_HOLDXY	1
#define REMAP_MODE_PRESSXY	2
#define REMAP_MODE_SWAPOPT	3

struct hardcoded_keys
{
	uint32_t buttons[14];
};

extern uint32_t profile_config;
extern struct hardcoded_keys keys_config[8];
extern int remap_state[2];
extern uint32_t *menu_key[2][2];
extern struct Menu__ menu_oswan;

extern void Menu(void);
extern void load_config(void);
extern void save_config(void);
extern void update_remap_config();

#endif
