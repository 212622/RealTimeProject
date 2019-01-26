#if !defined CAMERA_H
#define CAMERA_H

/*--------------------------------------------------------------*/
/*  HEADER FILES        */
/*--------------------------------------------------------------*/
#include "pmutex.h"
/*--------------------------------------------------------------*/
/*  GLOBAL CONSTRANTS   */
/*--------------------------------------------------------------*/
#define HRES	100                                 // camera height resolution
#define VRES	100                                 // camera width resolution
#define CAMOV	30							        // camera movements
/*--------------------------------------------------------------*/
#define NCENTR  5
/*--------------------------------------------------------------*/
/*  FUNCTION PROTOTYPES   */
/*--------------------------------------------------------------*/
extern void camera(void);
extern int get_image_count(int x0, int y0);
extern void get_centroid(void);
/*--------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*--------------------------------------------------------------*/
extern int      image[HRES][VRES]; 				    // global image buffer
extern int		camera_x, camera_y;					// coordinates of camera
extern int      line_x1, line_x2, line_y1, line_y2;	// coordinates of predict line
extern float    en_speed;
extern int      cam_deadline;

extern pthread_mutex_t mcam;						// camera mutex
/*--------------------------------------------------------------*/

#endif // CAMERA_H
