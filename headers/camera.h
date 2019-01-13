#if !defined CAMERA_H
#define CAMERA_H

/*--------------------------------------------------------------*/
/*  HEADER FILES        */
/*--------------------------------------------------------------*/
#include "pmutex.h"
/*--------------------------------------------------------------*/
/*  GLOBAL CONSTRANTS   */
/*--------------------------------------------------------------*/
#define HRES	100                         // camera height resolution
#define VRES	100                         // camera width resolution
#define CAMOV	20							// camera movements
/*--------------------------------------------------------------*/
/*  FUNCTION PROTOTYPES   */
/*--------------------------------------------------------------*/
extern void camera(void);
extern int get_image_count(int x0, int y0);
extern void get_centroid(int centroid[][2], int camera_x, int camera_y);
extern void get_image(int x0, int y0);
extern void put_image(int x0, int y0);
extern void save_image(int x0, int y0, char *path);
extern int count_pixel(char *path);
/*--------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*--------------------------------------------------------------*/
int 	image[HRES][VRES]; 				    // global image buffer
int		camera_x, camera_y;					// coordinates of camera
int     line_x1, line_x2, line_y1, line_y2;	// coordinates of predict line

pthread_mutex_t mcam;						// camera mutex
/*--------------------------------------------------------------*/

#endif // CAMERA_H
