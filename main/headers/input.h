#ifndef WSINPUT_H_
#define WSINPUT_H_

#include <stdint.h>

uint32_t WsInputGetState(void);
void exit_button(void);

enum hc_hvmode {
	HC_H = 0,
	HC_V,
	HC_END,
};

enum hc_key {
	HC_KEY_Y1 = 0,
	HC_KEY_Y2,
	HC_KEY_Y3,
	HC_KEY_Y4,
	HC_KEY_X1,
	HC_KEY_X2,
	HC_KEY_X3,
	HC_KEY_X4,
	HC_KEY_OPTION, /* Useless */
	HC_KEY_START,
	HC_KEY_BTN_A,
	HC_KEY_BTN_B,
	HC_KEY_GAME_END,

	HC_KEY_REMAP = HC_KEY_GAME_END,
	HC_KEY_EMENU,
	HC_KEY_END,
};

enum remap_mode {
	REMAP_MODE_NONE = 0,
	REMAP_MODE_HOLDY2X,
	REMAP_MODE_HOLDX2Y,
	REMAP_MODE_PRESSXY,
	REMAP_MODE_ACTIVATE,
	REMAP_MODE_END,
};

struct hardcoded_keys
{
	uint32_t buttons[HC_KEY_END];
};

extern struct hardcoded_keys keys_config[6];
extern int remap_state[2];
extern int input_icon_reload;
extern uint32_t *menu_key[2][2];

#endif
