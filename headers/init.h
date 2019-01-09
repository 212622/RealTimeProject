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
#define YWORLD  668
#define XMENU   800
#define YMENU   100
#define XSTATUS 224
#define YSTATUS 768
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
extern int get_crash(int x0, int y0);
extern void get_centroid(int centroide[][2] , int name, int camera_x, int camera_y);
/*--------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*--------------------------------------------------------------*/
BITMAP  *buf;						    // double buffering
BITMAP  *bufm, *bufw, *bufs;
int 	image[HRES][VRES]; 				// global image buffer
/*--------------------------------------------------------------*/

#endif // INIT_H
