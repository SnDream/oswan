#include "input.h"
#include "shared.h"
#include "menu.h"
#include "drawing.h"

static uint8_t *keys;

struct hardcoded_keys keys_config[8];
int remap_state[2];
uint32_t *menu_key[2][2];

/*
* 0 = Up  (Y1)
* 1 = Right (Y1)
* 2 = Down (Y1)
* 3 = Left (Y1)
* 4 = Up  (X1)
* 5 = Right (X1)
* 6 = Down (X1)
* 7 = Left (X1)
* 8 = START
* 9 = OPTION
* 10 = A 
* 11 = B
*/

void exit_button(void)
{
	uint32_t rotate_input = 0;
	uint32_t key1, key2;
	keys = SDL_GetKeyState(NULL);
	if (HVMode != 0) rotate_input = 1;

	if (menu_key[rotate_input][0] == NULL || menu_key[rotate_input][1] == NULL) {
		key1 = SDLK_ESCAPE;
		key2 = SDLK_RETURN;
	} else {
		key1 = *menu_key[rotate_input][0];
		key2 = *menu_key[rotate_input][1];
	}

	/* Get to Menu hotkey */
	if ( ((keys[key1] == SDL_PRESSED) && (keys[key2] == SDL_PRESSED)) 
	#ifdef RG99
	|| keys[SDLK_PAGEUP] == SDL_PRESSED
	#endif
	) 
	{ 
		m_Flag = GF_MAINUI;
		SetVideo(1);
	}
}

uint32_t WsInputGetState()
{
	static uint8_t kupdate = 0, kpress = 0;
	uint32_t rotate_input = 0;
	SDL_Event event;
	static int32_t button = 0;
	static int32_t button_hold = 0;
	struct hardcoded_keys *key_conf;
	
	if (HVMode != 0) rotate_input = 1;
	
	SDL_PollEvent(&event);
	keys = SDL_GetKeyState(NULL);

	key_conf = &keys_config[rotate_input];
	kpress = keys[ key_conf->buttons[12] ];
	kupdate ^= kpress;
	switch (key_conf->buttons[13]) {
	case REMAP_MODE_HOLDY2X:
		if (kpress) {
			button = 0;
			key_conf += 2;
			break;
		}
		if (kupdate) button_hold = button & ((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7));
		button = button_hold;
		break;
	case REMAP_MODE_HOLDX2Y:
		if (kpress) {
			button = 0;
			key_conf += 2;
			break;
		}
		if (kupdate) button_hold = button & ((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
		button = button_hold;
		break;
	case REMAP_MODE_PRESSXY:
		button = 0;
		key_conf += remap_state[rotate_input];
		if (!kpress || !kupdate) break;
		remap_state[rotate_input] += 2;
		remap_state[rotate_input] &= 3;
		break;
	case REMAP_MODE_SWAPOPT:
		button = 0;
		key_conf += remap_state[rotate_input];
		if (!kpress || !kupdate) break;
		remap_state[rotate_input] += 2;
		remap_state[rotate_input] &= 7;
		break;
	default:
	case REMAP_MODE_NONE:
		button = 0;
	}
	kupdate = kpress;
	
	// UP -> Y1
	if (keys[ key_conf->buttons[0] ] == SDL_PRESSED)
	{
		button |= (1<<0);
	}
	
	// RIGHT -> Y2
	if (keys[ key_conf->buttons[1] ] == SDL_PRESSED)
	{
		button |= (1<<1);
	}
	
	// DOWN -> Y3
	if (keys[ key_conf->buttons[2] ] == SDL_PRESSED)
	{
		button |= (1<<2);
	}
	
	// LEFT -> Y4
	if (keys[ key_conf->buttons[3] ] == SDL_PRESSED)
	{
		button |= (1<<3);
	}
	
	// UP -> X1
	if (keys[ key_conf->buttons[4] ] == SDL_PRESSED)
	{
		button |= (1<<4);
	}
	
	// RIGHT -> X2
	if (keys[ key_conf->buttons[5] ] == SDL_PRESSED)
	{
		button |= (1<<5);
	}
	
	// DOWN -> X3
	if (keys[ key_conf->buttons[6] ] == SDL_PRESSED)
	{
		button |= (1<<6);
	}
	
	// LEFT -> X4
	if (keys[ key_conf->buttons[7] ] == SDL_PRESSED)
	{
		button |= (1<<7);
	}

	// SELECT/OTHER -> OPTION (Wonderswan button)
	if (keys[ key_conf->buttons[8] ] == SDL_PRESSED)
	{
		button |= (1<<8);
	}

	// START -> START (Wonderswan button)
	if (keys[ key_conf->buttons[9] ] == SDL_PRESSED)
	{
		button |= (1<<9);
	}
	
	// A -> A (Wonderswan button)
	if (keys[ key_conf->buttons[10] ] == SDL_PRESSED)
	{
		button |= (1<<10);
	}
	
	// B -> B (Wonderswan button)
	if (keys[ key_conf->buttons[11] ] == SDL_PRESSED)
	{
		button |= (1<<11);
	}
	
	return button;
}
