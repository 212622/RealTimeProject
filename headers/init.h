#if !defined INIT_H
#define INIT_H

/*--------------------------------------------------------------*/
/*  HEADER FILES        */
/*--------------------------------------------------------------*/
#include <allegro.h>
/*--------------------------------------------------------------*/
/*  GLOBAL CONSTRANTS   */
/*--------------------------------------------------------------*/
#define XWIN    1024                        // window x resolution
#define YWIN    768                         // window y resolution
#define HRES	100                         // camera height resolution
#define VRES	100                         // camera width resolution
#define ALLEGRO_NO_FIX_ALIASES 				// fix for the new version of glibc
/*--------------------------------------------------------------*/
#define PER 	20							// base period
#define PRIO    80							// task priority
/*--------------------------------------------------------------*/
#define CAMOV	20							// camera movements
/*--------------------------------------------------------------*/
/*  FUNCTION PROTOTYPES   */
/*--------------------------------------------------------------*/
extern void make_pink_background(char *old, char *new);
extern void init(void);
// extern void get_image(int x0, int y0);
// extern void put_image(int x0, int y0);
extern void save_image(int x0, int y0, char *path);
extern int count_pixel(char *path);
extern void get_centroid(int centroid[][2], int camera_x, int camera_y);
/*--------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*--------------------------------------------------------------*/
int 	image[HRES][VRES]; 				    // global image buffer
int		camera_x, camera_y;					// coordinates of camera
int     line_x1, line_x2, line_y1, line_y2;	// coordinates of predict line

pthread_mutex_t mcam;						// camera mutex
/*--------------------------------------------------------------*/

#endif // INIT_H
