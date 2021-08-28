#include "drawing.h"
#include "menu.h"
#include "scaler.h"

SDL_Surface *actualScreen, *menuscreen, *ws_backbuffer;
struct scaling screen_scale;

void SetVideo(uint8_t mode)
{
	int32_t flags = FLAG_VIDEO;
	
	actualScreen = SDL_SetVideoMode(REAL_SCREEN_WIDTH, REAL_SCREEN_HEIGHT, BITDEPTH_OSWAN, FLAG_VIDEO);
	if (!menuscreen) menuscreen = SDL_CreateRGBSurface(SDL_SWSURFACE, MENU_SCREEN_WIDTH, MENU_SCREEN_HEIGHT, BITDEPTH_OSWAN, 0,0,0,0);
	
	#ifndef RS90
	if (!ws_backbuffer) ws_backbuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, 240, 144, BITDEPTH_OSWAN, 0,0,0,0); */
	
	FrameBuffer = ws_backbuffer->pixels;
	#else
	/* Write directly in RS90 */
	FrameBuffer = actualScreen->pixels + (8 + 8 * 240) * sizeof(uint16_t);
	#endif
}

void Clear_Screen(void)
{
	SDL_FillRect(menuscreen, NULL, 0);
	
	SDL_FillRect(actualScreen, NULL, 0);
	Update_Screen();
#if defined(SDL_TRIPLEBUF) || defined(SDL_DOUBLEBUF)
	SDL_FillRect(actualScreen, NULL, 0);
	Update_Screen();
	SDL_FillRect(actualScreen, NULL, 0);
	Update_Screen();
#endif
}

void Clear_Menu(void)
{
	SDL_FillRect(menuscreen, NULL, 0);
}


void Clear_Screen_Norefresh(void)
{
	//memset(Surface_to_Draw, 0, (actualScreen->w*actualScreen->h)*2);
	SDL_FillRect(menuscreen, NULL, 0);
	SDL_FillRect(actualScreen, NULL, 0);
}

void Draw_Rect_Menu(int x, int y, int w, int h)
{
	SDL_Rect pos = {.x = x, .y = y, .w = w, .h = h};
	SDL_FillRect(menuscreen, &pos, RGB565(4,3,95));
}

void Update_Screen()
{
	if (m_Flag == GF_MAINUI)
	{
		SDL_BlitSurface(menuscreen, NULL, actualScreen, NULL);
	}
	SDL_Flip(actualScreen);
	#ifdef RS90
	FrameBuffer = actualScreen->pixels + (8 + 8 * 240) * sizeof(uint16_t);
	#endif
}


/* It's worth noting that the virtual screen width is SCREEN_REAL_WIDTH in pixels, not 224. (Wonderswan's screen width)
 * Thus, it must be taken in account accordingly. */
void screen_draw(void)
{
	#ifndef RS90
	SDL_Rect rct, rct2;
	uint16_t *src = (uint16_t *) FrameBuffer+8;	// +8 offset , width = 240
	uint16_t *dst = (uint16_t *) actualScreen->pixels;
	uint32_t x, y;
	
	if (HVMode == 0)
	{
		switch(menu_oswan.scaling)
		{
			//Thanks Pingflood. Unscaled crap
			case 0:
				rct.x = 8;
				rct.y = 0;
				rct.w = 224;
				rct.h = 144;
				rct2.x = (actualScreen->w - 224) / 2;
				rct2.y = (actualScreen->h - 144) / 2;
				SDL_BlitSurface(ws_backbuffer, &rct, actualScreen, &rct2);
			break;
			#ifdef RS90
			case 1:
			case 2:
				bitmap_scale(0,0,224,144,actualScreen->w,actualScreen->h,240,0,src,dst);
			break;
			#else
			// Fullscreen
			case 1:
				upscale_224x144_to_320xXXX(actualScreen->pixels, FrameBuffer+8, 240);
			break;
			// Keep Aspect
			case 2:
				upscale_224x144_to_320xXXX(actualScreen->pixels + (18 * actualScreen->w)*2, FrameBuffer+8, 204);
			break;
			#endif
		}
	}
	// Vertical
	else
	{
		switch(menu_oswan.scaling)
		{
			//Thanks Pingflood. Unscaled crap
			case 0:
				dst += (320*(224-1));	// draw from Left-Down
				dst += 90 + (240 * 4) * 2;
				for ( x = 0; x < 144/2; x++) {
					for( y = 0; y < 224; y++) {
						*(uint32_t*)dst = *src|(*(src+240)<<16);
						src++; dst -= 320;
					}
					src += (240-224)+240; dst += (320*224)+2;
				}
			break;
			case 1:
				dst += (320*(240-1));
				upscale_144x224_to_320x240_rotate(dst, src);
			break;
			case 2: // RotateWide	224x144 > 288x224	
				dst += (320*(224-1));
				dst += (320 + ( 640 * 4)) + 16;
				for ( x = 0; x < 144; x++) {
					for( y = 0; y < 224; y++) {
						*(uint32_t*)dst = *src|(*src<<16);
						src++; dst -= 320;
					}
					src += (240-224); dst += (320*224)+2;
				}
			break;
		}
	}
	*/

	#else
	/* RefreshLine sometimes draws outside the border */
	uint16_t *pp = actualScreen->pixels + (232 + 7 * 240) * sizeof(uint16_t);
	for (int y = 0 ; y < 145; y++) {
		for (int c = 0; c < 16; c++) {
			*(pp++) = 0x0;
		}
		pp += 224;
	}
	#endif
	Update_Screen();
}

void Cleanup_Screen()
{
	if (actualScreen != NULL) SDL_FreeSurface(actualScreen);
	if (menuscreen != NULL) SDL_FreeSurface(menuscreen);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);	
	SDL_Quit();
}
