/*----------------------------------------------------------------------*/
/*  HEADER FILES        */
/*----------------------------------------------------------------------*/
#include <time.h>
#include <math.h>
#include <pthread.h>
#include "ptask.h"
#include "camera.h"
#include "draw.h"
#include "ally.h"
#include "init.h"
/*----------------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*----------------------------------------------------------------------*/
int 	image[HRES][VRES]; 				        // global image buffer
int		camera_x, camera_y;					    // coordinates of camera
int     line_x1, line_x2, line_y1, line_y2;	    // coordinates of predict line
int		en_time[NCENTR];
float 	en_speed;
int     cam_deadline;

int centroid[NCENTR][2];
int n_speed;
float speed, tot_speed, new_speed;
int n_x2, tot_x2, new_x2;
int x1, my_y1, x2, y2;
int old_x, old_y;

pthread_mutex_t mcam;						    // camera mutex
/*----------------------------------------------------------------------*/
/*  FUNCTION DEFINITIONS   */
/*----------------------------------------------------------------------*/
void init_camera(void) {
	int i;

	for (i=0; i<3; ++i) {
		centroid[i][0] = 0;
		centroid[i][1] = 0;
	}
	en_time[0] = en_time[1] = en_time[2] = 0;
	n_speed = speed = tot_speed = new_speed = 0;
	n_x2 = x2 = tot_x2 = new_x2 = 0;
	old_x = old_y = 0;
	x1 = my_y1 = x2 = y2 = 0;
}
/*----------------------------------------------------------------------*/
// GET_IMAGE_COUNT reads an area of the screen centered in
// (x0,y0) and stores it into image[][] and also return
// the number of non-black pixels
/*----------------------------------------------------------------------*/
int get_image_count(int x0, int y0) {
	int		i, j;		// image indexes
	int		x, y;		// video coordinates
	int 	c, count = 0;

	for (i=0; i<VRES; i++) {
		for (j=0; j<HRES; j++) {
			x = x0 - (VRES / 2) + i;
			y = y0 - (HRES / 2) + j;
			c = getpixel(bufw, x, y);
			if (c == makecol(255, 0, 0))
				c = makecol(0, 0, 0);
			pthread_mutex_lock(&mcam);
			image[i][j] = c;
			pthread_mutex_unlock(&mcam);
			if(image[i][j] != makecol(0, 0, 0))
				count++;
		}
	}

	return count;
}
/*----------------------------------------------------------------------*/
// GET_CENTROID do the centroid computation:
// compute the centroid of pixels with light color
/*----------------------------------------------------------------------*/
void get_centroid(void) {
	int		x, y;		// video coordinates
	int		c;
	int		i;
	int 	min_x, max_x, min_y, max_y;

	max_x = max_y = 0;
	min_x = VRES;
	min_y = HRES;

	// elimino i vecchi valori e faccio spazio per i nuovi
	for (i=0; i<NCENTR - 1; i++) {
		centroid[i][0] = centroid[i + 1][0];
		centroid[i][1] = centroid[i + 1][1];
	}

	for (x=0; x<VRES; x++) {
		for (y=0; y<HRES; y++) {
			c = image[x][y];
			if (c != makecol(0, 0, 0)) {
				if (x < min_x) min_x = x;
				else if (x > max_x) max_x = x;

				if (y < min_y) min_y = y;
				else if (y > max_y) max_y = y;
			}	
		}
	}

	centroid[NCENTR - 1][0] = camera_x + min_x + ((max_x - min_x) / 2);
	centroid[NCENTR - 1][1] = camera_y + min_y + ((max_y - min_y) / 2);
}
/*----------------------------------------------------------------------*/
void get_time(void) {
	int ms = 0, i = 0;
	struct timespec spec;

	clock_gettime(CLOCK_REALTIME, &spec);
	ms = round(spec.tv_nsec / 1.0e6);

	for (i=0; i< NCENTR - 1; i++)
		en_time[i] = en_time[i + 1];
	
	en_time[NCENTR - 1] = ms;
}
/*----------------------------------------------------------------------*/
void compute(void) {

	// Compute line and prediction
	old_x = centroid[0][0];
	old_y = centroid[0][1];
	x1 = centroid[NCENTR - 1][0];
	my_y1 = centroid[NCENTR - 1][1];
	y2 = YWORLD - sfondo->h;
	if (old_y == my_y1) my_y1++;
	
	x2 = (((y2 - old_y) / (my_y1 - old_y)) * (x1 - old_x)) + old_x;
	if (x2 >= 0 && x2 <= XWORLD && old_y != 0) {
		tot_x2 += x2;
		n_x2++;
		new_x2 = tot_x2 / n_x2;
	}
	// printf("new_x2 = %d, n_x2 = %d\n", new_x2, n_x2);

	speed = ((float)my_y1 - (float)old_y) / (en_time[NCENTR - 1] - en_time[0]);
	if (speed >= 0 && old_y != 0) {
		tot_speed += speed;
		n_speed++;
		new_speed = tot_speed / n_speed;
	}
	// printf("new_speed = %f\n", new_speed);

	pthread_mutex_lock(&mcam);
	line_x1 = x1;
	line_y1 = my_y1;
	line_x2 = new_x2;
	line_y2 = y2;
	en_speed = new_speed;
	pthread_mutex_unlock(&mcam);
}
/*----------------------------------------------------------------------*/
/*  Periodic task for camera detection   */
/*----------------------------------------------------------------------*/
void camera(void) {
	int v = 1, count = 0, cam_x_old = 0, k, one = 0;
	int tracking = 0;

	pthread_mutex_lock(&mcam);
	camera_x = 0;
	camera_y = 0;
	en_speed = 0;
	line_x1 = line_x2 = line_y1 = line_y2 = 0;
	pthread_mutex_unlock(&mcam);

	init_camera();

	while (1) {
		
		// Image scanning & thresholding: read the pixel color in a given
		// window and discard the pixel with dark color
		count = get_image_count(camera_x + (VRES / 2), camera_y + (HRES /  2));

		if (tracking == 0) {
			if (count > 0) {
				tracking = 1;
				cam_x_old = camera_x;
			}
			else {
				if (camera_x <= 0) v = 1;
				else if (camera_x + VRES >= XWORLD) v = -1;

				pthread_mutex_lock(&mcam);
				camera_x += v * VRES;
				if (camera_x < 0) camera_x = 0;
				if (camera_x > XWORLD - VRES) camera_x = XWORLD - VRES;
				pthread_mutex_unlock(&mcam);
			}
		}

		if (tracking >= 1) {
			if (tracking <= CAMOV) {
				
				// Centroid computation: compute the centroid of pixels with light color
				get_centroid();
				get_time();
				
				compute();

				// Camera control: control the camera axes to move to the centroid
				pthread_mutex_lock(&mcam);
				camera_x = centroid[NCENTR - 1][0] - (VRES / 2);
				camera_y = centroid[NCENTR - 1][1] - (HRES / 2);
				if (camera_x < 0) camera_x = 0;
				if (camera_x > XWORLD - VRES) camera_x = XWORLD - VRES;
				if (camera_y < 0) camera_y = 0;
				pthread_mutex_unlock(&mcam);

				tracking++;
			}

			if (tracking > CAMOV) {
				// printf("en_speed = %f\n", en_speed);
				// printf("line_x2 = %d\n", line_x2);

				// activate ally
				for(k=0; k<MAXA; k++) {
					if (state_al[k] == WAIT && one == 0) {
						pthread_mutex_lock(&mal);
						n_al_act++;
						state_al[k] = ACTIVE;
						ptask_activate(tid_al[k]);
						pthread_mutex_unlock(&mal);
						one++;
					}
				}
				one = 0;

				// initialize camera
				pthread_mutex_lock(&mcam);
				camera_x = cam_x_old + (v * VRES);
				if (camera_x > XWORLD - VRES) camera_x = XWORLD - VRES;
				if (camera_x < 0) camera_x = 0;
				camera_y = 0;
				pthread_mutex_unlock(&mcam);
				init_camera();

				tracking = 0;
			}
		}

		/* check for deadline miss */
        if (ptask_deadline_miss()) cam_deadline++;

		ptask_wait_for_period();
	}
}
/*----------------------------------------------------------------------*/
