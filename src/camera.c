/*----------------------------------------------------------------------*/
/*  HEADER FILES        */
/*----------------------------------------------------------------------*/
#include <pthread.h>
#include "ptask.h"
#include "camera.h"
#include "draw.h"
/*----------------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*----------------------------------------------------------------------*/
int 	image[HRES][VRES]; 				        // global image buffer
int		camera_x, camera_y;					    // coordinates of camera
int     line_x1, line_x2, line_y1, line_y2;	    // coordinates of predict line

pthread_mutex_t mcam;						    // camera mutex
/*----------------------------------------------------------------------*/
/*  FUNCTION DEFINITIONS   */
/*----------------------------------------------------------------------*/
// GET_IMAGE reads an area of the screen centered in
// (x0,y0) and stores it into image[][]
/*----------------------------------------------------------------------*/
// void get_image(int x0, int y0) {
// 	int		i, j;		// image indexes
// 	int		x, y;		// video coordinates

// 	for (i=0; i<VRES; i++) {
// 		for (j=0; j<HRES; j++) {
// 			x = x0 - (VRES / 2) + i;
// 			y = y0 - (HRES / 2) + j;
// 			image[i][j] = getpixel(bufw, x, y);
// 		}
// 	}
// }
/*----------------------------------------------------------------------*/
// PUT_IMAGE displays the image stored in image[][]
// in an area centered in (x0,y0)
/*----------------------------------------------------------------------*/
// void put_image(int x0, int y0) {
// 	int		i, j;		// image indexes
// 	int		x, y;		// video coordinates

// 	for (i=0; i<VRES; i++) {
// 		for (j=0; j<HRES; j++) {
// 			x = x0 - (VRES / 2) + i;
// 			y = y0 - (HRES / 2) + j;
// 			putpixel(bufs, x, y, image[i][j]);
// 		}
// 	}
// }
/*----------------------------------------------------------------------*/
// void save_image(int x0, int y0, char *path) {
// 	int		i, j;		// image indexes
// 	int		x, y;		// video coordinates
// 	BITMAP	*img;
	
// 	img = create_bitmap(HRES, VRES);

// 	for (i=0; i<VRES; i++) {
// 		for (j=0; j<HRES; j++) {
// 			x = x0 - (VRES / 2) + i;
// 			y = y0 - (HRES / 2) + j;
// 			putpixel(img, x, y, image[i][j]);
// 		}
// 	}

// 	save_bitmap(path, img, NULL);
// }
/*----------------------------------------------------------------------*/
// int count_pixel(char *path) {
// 	BITMAP	*img;
// 	int		count, x, y, c;
	
// 	img = load_bitmap(path, NULL);
// 	count = 0;
	
// 	for (x=0; x<img->w; x++) {
// 		for (y=0; y<img->h; y++) {
// 			c = getpixel(img, x, y);
// 			if (c != makecol(0, 0, 0) && c != makecol(255, 0, 0))
// 				count++;
// 		}
// 	}

// 	return count;
// }
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
void get_centroid(int centroid[][2], int camera_x, int camera_y) {
	int		x, y;		// video coordinates
	int		c;
	int 	min_x, max_x, min_y, max_y;

	max_x = max_y = 0;
	min_x = VRES;
	min_y = HRES;

	// elimino i vecchi valori e faccio spazio per i nuovi
	centroid[0][0] = centroid[1][0];
	centroid[0][1] = centroid[1][1];

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

	centroid[1][0] = camera_x + min_x + ((max_x - min_x) / 2);
	centroid[1][1] = camera_y + min_y + ((max_y - min_y) / 2);
}
/*----------------------------------------------------------------------*/
/*  Periodic task for camera detection   */
/*----------------------------------------------------------------------*/
void camera(void) {
	int v = 1, count = 0, old_x = 0, old_y = 0, cam_x_old = 0;
	int centroid[2][2] = {{0, 0}, {0, 0}}, tracking = 0;
	
	pthread_mutex_lock(&mcam);
	camera_x = 0;
	camera_y = 0;
	pthread_mutex_unlock(&mcam);

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

				// Centroid computation: compute the centroid of pixels with
				// light color
				get_centroid(centroid, camera_x, camera_y);

				// Camera control: control the camera axes to move to the centroid
				pthread_mutex_lock(&mcam);
				camera_x = centroid[1][0] - (VRES / 2);
				camera_y = centroid[1][1] - (HRES / 2);
				if (camera_x < 0) camera_x = 0;
				if (camera_x > XWORLD - VRES) camera_x = XWORLD - VRES;
				if (camera_y < 0) camera_y = 0;
				pthread_mutex_unlock(&mcam);

				if (tracking == CAMOV - 5) {
					old_x = centroid[0][0];
					old_y = centroid[0][1];
				}

				tracking++;
			}

			if (tracking > CAMOV) {
				// Compute line and prediction
				// old_x = centroid[0][0];
				// old_y = centroid[0][1];
				pthread_mutex_lock(&mcam);
				line_x1 = centroid[1][0];
				line_y1 = centroid[1][1];

				line_y2 = YWORLD - sfondo->h;
				if (old_y == line_y1) line_y1++;
				line_x2 = (((line_y2 - old_y) / (line_y1 - old_y)) * (line_x1 - old_x)) + old_x;
				pthread_mutex_unlock(&mcam);
				// printf("centroide1 : %d, %d\ncentroide2 : %d, %d \n\n", old_x, old_y, line_x1, line_y1);

				pthread_mutex_lock(&mcam);
				camera_x = cam_x_old + (v * VRES);
				if (camera_x > XWORLD - VRES) camera_x = XWORLD - VRES;
				if (camera_x < 0) camera_x = 0;
				camera_y = 0;
				pthread_mutex_unlock(&mcam);

				tracking = 0;
			}
		}

		ptask_wait_for_period();
	}
}
/*----------------------------------------------------------------------*/
// Task that periodically gets images from position
// (XCAM,YCAM) and displays them in position (XD,YD)
/*----------------------------------------------------------------------*/
// void cameratask() {
// 	while (1) {
// 		get_image(camera_x + 100, camera_y + 100);
// 		// get_image(XCAM, YCAM);
// 		put_image(SCREEN_W - 100, SCREEN_H - 100);
// 		// put_image(XD, YD);
// 		ptask_wait_for_period();
// 	}
// }
/*----------------------------------------------------------------------*/
