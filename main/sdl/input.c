#include "input.h"
#include "shared.h"
#include "menu.h"
#include "drawing.h"

static uint8_t *keys;

struct hardcoded_keys keys_config[6];
int remap_state[2];
int input_icon_reload = 1;
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
	uint32_t key1, key2;
	keys = SDL_GetKeyState(NULL);

	if (menu_key[HVMode][0] == NULL || menu_key[HVMode][1] == NULL) {
		key1 = SDLK_ESCAPE;
		key2 = SDLK_RETURN;
	} else {
		key1 = *menu_key[HVMode][0];
		key2 = *menu_key[HVMode][1];
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
	SDL_Event event;
	static int32_t button = 0;
	static int32_t button_hold = 0;
	static uint8_t rotate_hold = 2;
	struct hardcoded_keys *key_conf;
	
	SDL_PollEvent(&event);
	keys = SDL_GetKeyState(NULL);

	#ifdef SHOW_LCD_ICON
	if (rotate_hold != HVMode) {
		rotate_hold = HVMode;
		input_icon_reload = 1;
	}
	#endif

	key_conf = &keys_config[HVMode];
	kpress = keys[key_conf->buttons[HC_KEY_REMAP]];
	kupdate ^= kpress;
	switch (key_conf->buttons[HC_KEY_REMAPMODE]) {
	case REMAP_MODE_HOLDY2X:
		#ifdef SHOW_LCD_ICON
		if (input_icon_reload) {
			lcd_icon_stat[LCD_INDEX__REMAP_Y] = LCD_ICON__REMAP_Y_OFF;
			lcd_icon_stat[LCD_INDEX__REMAP_X] = LCD_ICON__REMAP_X_ON;
			lcd_icon_stat[LCD_INDEX__SWAP_TO_START] = LCD_ICON__SWAP_TO_START_OFF;
			lcd_icon_stat[LCD_INDEX__SWAP_TO_BTN_A] = LCD_ICON__SWAP_TO_BTN_A_OFF;
			lcd_icon_stat[LCD_INDEX__SWAP_TO_BTN_B] = LCD_ICON__SWAP_TO_BTN_B_OFF;
			input_icon_reload = 0;
		}
		#endif
		if (kpress) {
			button = 0;
			key_conf += 2;
			#ifdef SHOW_LCD_ICON
			if (kupdate) {
				lcd_icon_stat[LCD_INDEX__REMAP_Y] = LCD_ICON__REMAP_Y_ON;
				lcd_icon_stat[LCD_INDEX__REMAP_X] = LCD_ICON__REMAP_X_OFF;
			}
			#endif
			break;
		}
		if (kupdate) {
			#ifdef SHOW_LCD_ICON
			lcd_icon_stat[LCD_INDEX__REMAP_Y] = LCD_ICON__REMAP_Y_OFF;
			lcd_icon_stat[LCD_INDEX__REMAP_X] = LCD_ICON__REMAP_X_ON;
			#endif
			button_hold = button & ((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7));
		}
		button = button_hold;
		break;
	case REMAP_MODE_HOLDX2Y:
		#ifdef SHOW_LCD_ICON
		if (input_icon_reload) {
			lcd_icon_stat[LCD_INDEX__REMAP_Y] = LCD_ICON__REMAP_Y_ON;
			lcd_icon_stat[LCD_INDEX__REMAP_X] = LCD_ICON__REMAP_X_OFF;
			lcd_icon_stat[LCD_INDEX__SWAP_TO_START] = LCD_ICON__SWAP_TO_START_OFF;
			lcd_icon_stat[LCD_INDEX__SWAP_TO_BTN_A] = LCD_ICON__SWAP_TO_BTN_A_OFF;
			lcd_icon_stat[LCD_INDEX__SWAP_TO_BTN_B] = LCD_ICON__SWAP_TO_BTN_B_OFF;
			input_icon_reload = 0;
		}
		#endif
		if (kpress) {
			button = 0;
			key_conf += 2;
			#ifdef SHOW_LCD_ICON
			if (kupdate) {
				lcd_icon_stat[LCD_INDEX__REMAP_Y] = LCD_ICON__REMAP_Y_OFF;
				lcd_icon_stat[LCD_INDEX__REMAP_X] = LCD_ICON__REMAP_X_ON;
			}
			#endif
			break;
		}
		if (kupdate) {
			#ifdef SHOW_LCD_ICON
			lcd_icon_stat[LCD_INDEX__REMAP_Y] = LCD_ICON__REMAP_Y_ON;
			lcd_icon_stat[LCD_INDEX__REMAP_X] = LCD_ICON__REMAP_X_OFF;
			#endif
			button_hold = button & ((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
		}
		button = button_hold;
		break;
	case REMAP_MODE_PRESSXY:
		#ifdef SHOW_LCD_ICON
		if (input_icon_reload) {
			switch(remap_state[HVMode] & 0x02) {
			default:
			case 0:
				lcd_icon_stat[LCD_INDEX__REMAP_Y] = LCD_ICON__REMAP_Y_OFF;
				lcd_icon_stat[LCD_INDEX__REMAP_X] = LCD_ICON__REMAP_X_ON;
				break;
			case 2:
				lcd_icon_stat[LCD_INDEX__REMAP_Y] = LCD_ICON__REMAP_Y_ON;
				lcd_icon_stat[LCD_INDEX__REMAP_X] = LCD_ICON__REMAP_X_OFF;
				break;
			}
			lcd_icon_stat[LCD_INDEX__SWAP_TO_START] = LCD_ICON__SWAP_TO_START_OFF;
			lcd_icon_stat[LCD_INDEX__SWAP_TO_BTN_A] = LCD_ICON__SWAP_TO_BTN_A_OFF;
			lcd_icon_stat[LCD_INDEX__SWAP_TO_BTN_B] = LCD_ICON__SWAP_TO_BTN_B_OFF;
			input_icon_reload = 0;
		}
		#endif
		button = 0;
		key_conf += remap_state[HVMode];
		if (!kpress || !kupdate) break;
		if (remap_state[HVMode] < 2) remap_state[HVMode] += 2;
		else remap_state[HVMode] &= 1;
		#ifdef SHOW_LCD_ICON
		switch(remap_state[HVMode] & 0x02) {
		default:
		case 0:
			lcd_icon_stat[LCD_INDEX__REMAP_Y] = LCD_ICON__REMAP_Y_OFF;
			lcd_icon_stat[LCD_INDEX__REMAP_X] = LCD_ICON__REMAP_X_ON;
			break;
		case 2:
			lcd_icon_stat[LCD_INDEX__REMAP_Y] = LCD_ICON__REMAP_Y_ON;
			lcd_icon_stat[LCD_INDEX__REMAP_X] = LCD_ICON__REMAP_X_OFF;
			break;
		}
		#endif
		break;
	case REMAP_MODE_ACTIVATE:
		#ifdef SHOW_LCD_ICON
		if (input_icon_reload) {
			lcd_icon_stat[LCD_INDEX__REMAP_Y] = LCD_ICON__REMAP_Y_OFF;
			lcd_icon_stat[LCD_INDEX__REMAP_X] = LCD_ICON__REMAP_X_OFF;
			lcd_icon_stat[LCD_INDEX__SWAP_TO_START] = LCD_ICON__SWAP_TO_START_OFF;
			lcd_icon_stat[LCD_INDEX__SWAP_TO_BTN_A] = LCD_ICON__SWAP_TO_BTN_A_OFF;
			lcd_icon_stat[LCD_INDEX__SWAP_TO_BTN_B] = LCD_ICON__SWAP_TO_BTN_B_OFF;
			switch(remap_state[HVMode] & 0x06) {
			default:
			case 0:
				lcd_icon_stat[LCD_INDEX__SWAP_TO_BTN_B] = LCD_ICON__SWAP_TO_BTN_B_OFF;
				lcd_icon_stat[LCD_INDEX__SWAP_TO_START] = LCD_ICON__SWAP_TO_START_ON;
				break;
			case 2:
				lcd_icon_stat[LCD_INDEX__SWAP_TO_START] = LCD_ICON__SWAP_TO_START_OFF;
				lcd_icon_stat[LCD_INDEX__SWAP_TO_BTN_A] = LCD_ICON__SWAP_TO_BTN_A_ON;
				break;
			case 4:
				lcd_icon_stat[LCD_INDEX__SWAP_TO_BTN_A] = LCD_ICON__SWAP_TO_BTN_A_OFF;
				lcd_icon_stat[LCD_INDEX__SWAP_TO_BTN_B] = LCD_ICON__SWAP_TO_BTN_B_ON;
				break;
			}
			input_icon_reload = 0;
		}
		#endif
		button = 0;
		key_conf += remap_state[HVMode];
		if (!kpress || !kupdate) break;
		if (remap_state[HVMode] < 4) remap_state[HVMode] += 2;
		else remap_state[HVMode] &= 1;
		#ifdef SHOW_LCD_ICON
		switch(remap_state[HVMode] & 0x06) {
		default:
		case 0:
			lcd_icon_stat[LCD_INDEX__SWAP_TO_BTN_B] = LCD_ICON__SWAP_TO_BTN_B_OFF;
			lcd_icon_stat[LCD_INDEX__SWAP_TO_START] = LCD_ICON__SWAP_TO_START_ON;
			break;
		case 2:
			lcd_icon_stat[LCD_INDEX__SWAP_TO_START] = LCD_ICON__SWAP_TO_START_OFF;
			lcd_icon_stat[LCD_INDEX__SWAP_TO_BTN_A] = LCD_ICON__SWAP_TO_BTN_A_ON;
			break;
		case 4:
			lcd_icon_stat[LCD_INDEX__SWAP_TO_BTN_A] = LCD_ICON__SWAP_TO_BTN_A_OFF;
			lcd_icon_stat[LCD_INDEX__SWAP_TO_BTN_B] = LCD_ICON__SWAP_TO_BTN_B_ON;
			break;
		}
		#endif
		break;
	default:
	case REMAP_MODE_NONE:
		#ifdef SHOW_LCD_ICON
		if (input_icon_reload) {
			lcd_icon_stat[LCD_INDEX__REMAP_Y] = LCD_ICON__REMAP_Y_OFF;
			lcd_icon_stat[LCD_INDEX__REMAP_X] = LCD_ICON__REMAP_X_OFF;
			lcd_icon_stat[LCD_INDEX__SWAP_TO_START] = LCD_ICON__SWAP_TO_START_OFF;
			lcd_icon_stat[LCD_INDEX__SWAP_TO_BTN_A] = LCD_ICON__SWAP_TO_BTN_A_OFF;
			lcd_icon_stat[LCD_INDEX__SWAP_TO_BTN_B] = LCD_ICON__SWAP_TO_BTN_B_OFF;
			input_icon_reload = 0;
		}
		#endif
		button = 0;
	}
	kupdate = kpress;
	
	// UP -> Y1
	if (keys[key_conf->buttons[HC_KEY_Y1]] == SDL_PRESSED)
	{
		button |= (1<<0);
	}
	
	// RIGHT -> Y2
	if (keys[key_conf->buttons[HC_KEY_Y2]] == SDL_PRESSED)
	{
		button |= (1<<1);
	}
	
	// DOWN -> Y3
	if (keys[key_conf->buttons[HC_KEY_Y3]] == SDL_PRESSED)
	{
		button |= (1<<2);
	}
	
	// LEFT -> Y4
	if (keys[key_conf->buttons[HC_KEY_Y4]] == SDL_PRESSED)
	{
		button |= (1<<3);
	}
	
	// UP -> X1
	if (keys[key_conf->buttons[HC_KEY_X1]] == SDL_PRESSED)
	{
		button |= (1<<4);
	}
	
	// RIGHT -> X2
	if (keys[key_conf->buttons[HC_KEY_X2]] == SDL_PRESSED)
	{
		button |= (1<<5);
	}
	
	// DOWN -> X3
	if (keys[key_conf->buttons[HC_KEY_X3]] == SDL_PRESSED)
	{
		button |= (1<<6);
	}
	
	// LEFT -> X4
	if (keys[key_conf->buttons[HC_KEY_X4]] == SDL_PRESSED)
	{
		button |= (1<<7);
	}

	// SELECT/OTHER -> OPTION (Wonderswan button)
	// if (keys[key_conf->buttons[HC_KEY_OPTION]] == SDL_PRESSED)
	// {
	// 	button |= (1<<8);
	// }

	// START -> START (Wonderswan button)
	if (keys[key_conf->buttons[HC_KEY_START]] == SDL_PRESSED)
	{
		button |= (1<<9);
	}
	
	// A -> A (Wonderswan button)
	if (keys[key_conf->buttons[HC_KEY_BTN_A]] == SDL_PRESSED)
	{
		button |= (1<<10);
	}
	
	// B -> B (Wonderswan button)
	if (keys[key_conf->buttons[HC_KEY_BTN_B]] == SDL_PRESSED)
	{
		button |= (1<<11);
	}
	
	return button;
}
