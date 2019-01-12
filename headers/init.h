#if !defined INIT_H
#define INIT_H

/*--------------------------------------------------------------*/
/*  HEADER FILES        */
/*--------------------------------------------------------------*/
#include <allegro.h>
/*--------------------------------------------------------------*/
/*  GLOBAL CONSTRANTS   */
/*--------------------------------------------------------------*/
#define XWIN    1024                    // window x resolution
#define YWIN    768                     // window y resolution
#define HRES	200                     // camera height resolution
#define VRES	200                     // camera width resolution
#define XWORLD  800
#define YWORLD  650
#define XMENU   800
#define YMENU   90
#define XSTATUS 195
#define YSTATUS 750
#define ALLEGRO_NO_FIX_ALIASES 				/* fix for the new version of glibc */
/*--------------------------------------------------------------*/
/*  FUNCTION PROTOTYPES   */
/*--------------------------------------------------------------*/
extern void make_pink_background(char *old, char *new);
extern void init(void);
extern void get_image(int x0, int y0);
extern void put_image(int x0, int y0);
extern void save_image(int x0, int y0, char *path);
extern int count_pixel(char *path);
extern int get_image_count(int x0, int y0);
extern void get_centroid(int centroid[][2], int camera_x, int camera_y);
/*--------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*--------------------------------------------------------------*/
BITMAP  *buf;						    // double buffering
BITMAP  *bufm, *bufw, *bufs;
int 	image[HRES][VRES]; 				// global image buffer
/*--------------------------------------------------------------*/

#endif // INIT_H
