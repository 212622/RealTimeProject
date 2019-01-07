/*--------------------------------------------------------------*/
/*  HEADER FILES        */
/*--------------------------------------------------------------*/
#include "ptask.h"
#include "init.h"
/*--------------------------------------------------------------*/
/*  FUNCTION DEFINITIONS   */
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
void init() {
	allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, XWIN, YWIN, 0, 0);
    ptask_init(SCHED_RR, GLOBAL, PRIO_INHERITANCE);
	buf = create_bitmap(SCREEN_W, SCREEN_H);
}
//------------------------------------------------------
// GET_IMAGE reads an area of the screen centered in
// (x0,y0) and stores it into image[][]
//------------------------------------------------------
void get_image(int x0, int y0) {
int		i, j;		// image indexes
int		x, y;		// video coordinates

	for (i=0; i<HRES; i++)
		for (j=0; j<VRES; j++) {
			x = x0 - (HRES / 2) + i;
			y = y0 - (VRES / 2) + j;
			image[i][j] = getpixel(screen, x, y);
	}
}
//------------------------------------------------------
// PUT_IMAGE displays the image stored in image[][]
// in an area centered in (x0,y0)
//------------------------------------------------------
void put_image(int x0, int y0) {
int		i, j;		// image indexes
int		x, y;		// video coordinates

	for (i=0; i<HRES; i++)
		for (j=0; j<VRES; j++) {
			x = x0 - (HRES / 2) + i;
			y = y0 - (VRES / 2) + j;
			putpixel(buf, x, y, image[i][j]);
	}
}
/*--------------------------------------------------------------*/
void save_image(int x0, int y0, char *path) {
int		i, j;		// image indexes
int		x, y;		// video coordinates
BITMAP	*img;
	
	img = create_bitmap(HRES, VRES);

	for (i=0; i<HRES; i++)
		for (j=0; j<VRES; j++) {
			x = x0 - (HRES / 2) + i;
			y = y0 - (VRES / 2) + j;
			putpixel(img, x, y, image[i][j]);
	}

	save_bitmap(path, img, NULL);
}
/*--------------------------------------------------------------*/
