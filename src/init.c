/*--------------------------------------------------------------*/
/*  HEADER FILES        */
/*--------------------------------------------------------------*/
#include "ptask.h"
#include "init.h"
/*--------------------------------------------------------------*/
/*  FUNCTION DEFINITIONS   */
/*--------------------------------------------------------------*/
void init() {
	allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, XWIN, YWIN, 0, 0);
    ptask_init(SCHED_RR, GLOBAL, PRIO_INHERITANCE);
	buf = create_bitmap(SCREEN_W, SCREEN_H);
	bufm = create_bitmap(XMENU, YMENU);
	bufw = create_bitmap(XWORLD, YWORLD);
	bufs = create_bitmap(XSTATUS, YSTATUS);
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
//------------------------------------------------------
// GET_IMAGE reads an area of the screen centered in
// (x0,y0) and stores it into image[][]
//------------------------------------------------------
void get_image(int x0, int y0) {
int		i, j;		// image indexes
int		x, y;		// video coordinates

	for (i=0; i<VRES; i++)
		for (j=0; j<HRES; j++) {
			x = x0 - (VRES / 2) + i;
			y = y0 - (HRES / 2) + j;
			image[i][j] = getpixel(buf, x, y);
	}
}
//------------------------------------------------------
// PUT_IMAGE displays the image stored in image[][]
// in an area centered in (x0,y0)
//------------------------------------------------------
void put_image(int x0, int y0) {
int		i, j;		// image indexes
int		x, y;		// video coordinates

	for (i=0; i<VRES; i++)
		for (j=0; j<HRES; j++) {
			x = x0 - (VRES / 2) + i;
			y = y0 - (HRES / 2) + j;
			putpixel(bufs, x, y, image[i][j]);
	}
}
/*--------------------------------------------------------------*/
void save_image(int x0, int y0, char *path) {
int		i, j;		// image indexes
int		x, y;		// video coordinates
BITMAP	*img;
	
	img = create_bitmap(HRES, VRES);

	for (i=1; i<VRES; i++)
		for (j=1; j<HRES; j++) {
			x = x0 - (VRES / 2) + i;
			y = y0 - (HRES / 2) + j;
			putpixel(img, x, y, image[i][j]);
	}

	save_bitmap(path, img, NULL);
}
/*--------------------------------------------------------------*/
int count_pixel(char *path) {
BITMAP	*img;
int		count, x, y, c;
	
	img = load_bitmap(path, NULL);
	count = 0;
	
	for (x=0; x<img->w; x++) {
		for (y=0; y<img->h; y++) {
			c = getpixel(img, x, y);
			if (c != makecol(0, 0, 0) && c != makecol(255, 0, 0))
				count++;
		}
	}

	return count;
}
//------------------------------------------------------
// Like GET_IMAGE but also return the number of
// non-black and non-red pixels
//------------------------------------------------------
int get_image_count(int x0, int y0) {
int		i, j;		// image indexes
int		x, y;		// video coordinates
int 	count = 0;

	for (i=0; i<VRES; i++)
		for (j=0; j<HRES; j++) {
			x = x0 - (VRES / 2) + i;
			y = y0 - (HRES / 2) + j;
			image[i][j] = getpixel(bufw, x, y);
			if(image[i][j] != makecol(0,0,0) && image[i][j] != makecol(255,0,0))
				count++;
		}

	return count;
}
//------------------------------------------------------
// Task that periodically gets images from position
// (XCAM,YCAM) and displays them in position (XD,YD)
//------------------------------------------------------
// void cameratask() {
// 	while (1) {
// 		get_image(camera_x + 100, camera_y + 100);
// 		// get_image(XCAM, YCAM);
// 		put_image(SCREEN_W - 100, SCREEN_H - 100);
// 		// put_image(XD, YD);
// 		ptask_wait_for_period();
// 	}
// }
/*--------------------------------------------------------------*/
