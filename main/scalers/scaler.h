#ifndef SCALERS_H
#define SCALERS_H
#include <stdint.h>
extern void bitmap_scale(uint32_t startx, uint32_t starty, uint32_t viswidth, uint32_t visheight, uint32_t newwidth, uint32_t newheight,uint32_t pitchsrc,uint32_t pitchdest, uint16_t* restrict src, uint16_t* restrict dst);

extern void rotate_90_ccw(uint16_t* restrict dst, uint16_t* restrict src);
extern void upscale_224x144_to_320xXXX(uint16_t* restrict dst, uint16_t* restrict src, uint32_t height);
extern void upscale_144x224_to_320x240_rotate(uint16_t * restrict dst, uint16_t * restrict src);

inline void upscale_144x224_to_144x224_rotate(uint16_t *dst, uint16_t *src)
{
	int x, y;
	// draw from Left-Down
#ifndef SHOW_LCD_ICON
	const int ymax = 224;
#else
	const int ymax = 232;
#endif
	dst += (320 * (ymax - 1 + 4)) + ((320 - 144) / 2);
	for (x = 0; x < 144 / 2; x++) {
		for (y = 0; y < ymax; y++) {
			*(uint32_t*) dst = *src | (*(src + 240) << 16);
			src++;
			dst -= 320;
		}
		src += (240 - ymax) + 240;
		dst += (320 * ymax) + 2;
	}
}

inline void upscale_144x224_to_288x224_rotate(uint16_t *dst, uint16_t *src)
{
	int x, y;
	// draw from Left-Down
#ifndef SHOW_LCD_ICON
	const int ymax = 224;
#else
	const int ymax = 232;
#endif
	dst += (320 * (ymax - 1 + 4)) * 2 + ((320 - 144) / 2);
	for (x = 0; x < 144 / 2; x++) {
		for (y = 0; y < ymax; y++) {
			uint32_t var = *src | (*(src + 240) << 16);
			*(uint32_t*) dst = var;
			dst -= 320;
			*(uint32_t*) dst = var;
			src++;
			dst -= 320;
		}
		src += (240 - ymax) + 240;
		dst += (320 * ymax) * 2 + 2;
	}
}

/* From draw.c in ReGBA */

#ifdef __GNUC__
#	define likely(x)       __builtin_expect((x),1)
#	define unlikely(x)     __builtin_expect((x),0)
#else
#	define likely(x)       (x)
#	define unlikely(x)     (x)
#endif

/* Calculates the average of two RGB565 pixels. The source of the pixels is
 * the lower 16 bits of both parameters. The result is in the lower 16 bits.
 */
#define Average(A, B) ((((A) & 0xF7DE) >> 1) + (((B) & 0xF7DE) >> 1) + ((A) & (B) & 0x0821))

/* Raises a pixel from the lower half to the upper half of a pair. */
#define Raise(N) ((N) << 16)

/* Extracts the upper pixel of a pair into the lower pixel of a pair. */
#define Hi(N) ((N) >> 16)

/* Extracts the lower pixel of a pair. */
#define Lo(N) ((N) & 0xFFFF)

static inline void upscale_244x144_to_310x432_rg99(uint16_t *to_16, uint16_t *from_16, uint16_t *fcmp_16)
{
	/* Before:
	 *    a b c d e f
	 *
	 * After (multiple letters = average):
	 *    a    ab   bc   c    d    de   ef   f
	 */

	uint32_t *to = (uint32_t *)(to_16 + 24 * 320), *to_2, *to_3;
	uint32_t *from = (uint32_t *)(from_16), *fcmp = (uint32_t *)(fcmp_16);
	const uint32_t dst_pitch = 320 * sizeof(uint16_t);

	uint32_t x, y;

	for (y = 0; y < 144; y++) {
		to_2 = (uint32_t*)((uint8_t*) to + dst_pitch * 1);
		to_3 = (uint32_t*)((uint8_t*) to + dst_pitch * 2);
		for (x = 0; x < 240 / 6; x++) {
			if (*(from + 0) == *(fcmp + 0) 
					&& *(from + 1) == *(fcmp + 1)
					&& *(from + 2) == *(fcmp + 2)) {
				from += 3, fcmp += 3;
				to += 4, to_2 += 4, to_3 += 4;
				continue;
			}
			fcmp += 3;
			uint32_t b_a = *(from++),
			         d_c = *(from++),
			         f_e = *(from++);

			// Generate ab_a from b_a.
			*(to++) = *(to_2++) = *(to_3++) = likely(Hi(b_a) == Lo(b_a))
				? b_a
				: Lo(b_a) /* 'a' verbatim to low pixel */ |
				  Raise(Average(Hi(b_a), Lo(b_a))) /* ba to high pixel */;

			// Generate c_bc from b_a and d_c.
			*(to++) = *(to_2++) = *(to_3++) = likely(Hi(b_a) == Lo(d_c))
				? Lo(d_c) | Raise(Lo(d_c))
				: Raise(Lo(d_c)) /* 'c' verbatim to high pixel */ |
				  Average(Lo(d_c), Hi(b_a)) /* bc to low pixel */;

			// Generate de_d from d_c and f_e.
			*(to++) = *(to_2++) = *(to_3++) = likely(Hi(d_c) == Lo(f_e))
				? Lo(f_e) | Raise(Lo(f_e))
				: Hi(d_c) /* 'd' verbatim to low pixel */ |
				  Raise(Average(Lo(f_e), Hi(d_c))) /* de to high pixel */;

			// Generate f_ef from f_e.
			*(to++) = *(to_2++) = *(to_3++) = likely(Hi(f_e) == Lo(f_e))
				? f_e
				: Raise(Hi(f_e)) /* 'f' verbatim to high pixel */ |
				  Average(Hi(f_e), Lo(f_e)) /* ef to low pixel */;
		}

		to = to_3;
	}
}

#endif
