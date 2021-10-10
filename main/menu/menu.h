#ifndef MENU_H
#define MENU_H

#include <stdint.h>

#define RGB565(r,g,b) ((r << 8) | (g << 3) | (b >> 3))

#define MAX_TEXT_SIZE 48

#define EMULATOR_MAIN_MENU 0
#define CONTROLS_MENU 1
#define SETTINGS_KEY_SCREEN 2
#define CONTROLS_MENU_NOSET 3

struct config_line {
	char *name;
	int  *ref;
	int  min;
	int  max;
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

extern uint32_t profile_config;
extern struct Menu__ menu_oswan;

extern void Menu(void);
extern void default_config();
extern int load_config(const char *gamepath);
extern int save_config(const char *gamepath);
extern int del_config(const char *gamepath);
extern void update_all_config();
extern void update_key_config();
extern void update_volume_config();

#endif
