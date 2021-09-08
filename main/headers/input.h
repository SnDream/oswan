#ifndef WSINPUT_H_
#define WSINPUT_H_

#include <stdint.h>

uint32_t WsInputGetState(void);
void exit_button(void);

struct hardcoded_keys
{
	uint32_t buttons[14];
};

#define REMAP_MODE_NONE		0
#define REMAP_MODE_HOLDY2X	1
#define REMAP_MODE_HOLDX2Y	2
#define REMAP_MODE_PRESSXY	3
#define REMAP_MODE_SWAPOPT	4

extern struct hardcoded_keys keys_config[8];
extern int remap_state[2];
extern uint32_t *menu_key[2][2];

#endif
