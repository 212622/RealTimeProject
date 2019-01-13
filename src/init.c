/*--------------------------------------------------------------*/
/*  HEADER FILES        */
/*--------------------------------------------------------------*/
#include <allegro.h>
#include "ptask.h"
#include "init.h"
/*--------------------------------------------------------------*/
/*  GLOBAL CONSTRANTS   */
/*--------------------------------------------------------------*/
#define ALLEGRO_NO_FIX_ALIASES 				// fix for the new version of glibc
/*--------------------------------------------------------------*/
/*  FUNCTION DEFINITIONS   */
/*--------------------------------------------------------------*/
void init(void) {
	allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, XWIN, YWIN, 0, 0);
    ptask_init(SCHED_FIFO, GLOBAL, PRIO_INHERITANCE);
}
/*--------------------------------------------------------------*/
void make_pink_background(char *old, char *new) {
	BITMAP		*img, *imgp;		// pointers to bitmap
	PALETTE     pal;                // color palette
	int         x, y, c;
	int         pink, white;
	
	white = makecol(255, 255, 255);
	pink = makecol(255, 0, 255);
	img = load_bitmap(old, NULL);
	imgp = create_bitmap(img->w, img->h);
	
	for (x=0; x<img->w; x++) {
		for (y=0; y<img->h; y++) {
			c = getpixel(img, x, y);
			if (c == white) c = pink;
			putpixel(imgp, x, y, c);
		}
		get_palette(pal);
		save_bitmap(new, imgp, pal);
	}
}
/*--------------------------------------------------------------*/
