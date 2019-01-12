/*----------------------------------------------------------------------*/
/*  Patriots															*/															
/*	Real Time Project by Tomas Torricelli and Lorenzo Cuoghi			*/																
/*																		*/																
/*	This project simulate a physical system or the behavior of active 	*/
/*	agents interacting with each other and with the environment			*/
/*	Simulate a set of Patriot defense missiles that identify enemy 		*/
/*	targets, predict their trajectories and are launched to catch them 	*/
/*----------------------------------------------------------------------*/
/*  HEADER FILES        */
/*----------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <allegro.h>
#include "ptask.h"
#include "pmutex.h"
#include "init.h"
/*----------------------------------------------------------------------*/
/*  GLOBAL CONSTRANTS   */
/*----------------------------------------------------------------------*/
#define MAXT    5							// max number of enemy tasks
#define PER 	20							// base period
#define PRIO    80							// task priority
/*----------------------------------------------------------------------*/
#define WAIT	0							// stop enemy state
#define ACTIVE	1							// moving enemy state
#define BOOM	2							// explosion enemy state
/*----------------------------------------------------------------------*/
#define CAMOV	50							// camera movements
/*----------------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*----------------------------------------------------------------------*/
int		tid[MAXT];							// enemy task IDs
int		i;                      			// number of active enemy task
int		enemy_x[MAXT], enemy_y[MAXT];		// coordinates of enemies
int 	state[MAXT];						// enemy state
int		camera_x, camera_y;					// coordinates of camera
int     line_x1, line_x2, line_y1, line_y2;	// coordinates of predict line
BITMAP	*sfondo, *aereo, *boom, *patriot;	// images
float angle[MAXT], alfa[MAXT];
pthread_mutex_t mcam;						// camera mutex
/*----------------------------------------------------------------------*/
/*  Periodic task for camera detection   */
/*----------------------------------------------------------------------*/
void camera() {
int v = 1, count = 0, old_x = 0, old_y = 0, n = 0, cam_x_old = 0;
int centroid[2][2] = {{0, 0}, {0, 0}}, tracking = 0;
char *img[2] = {"camera/image1.bmp", "camera/image2.bmp"};
	
	camera_x = 0;
	camera_y = 2;

	while (1) {

		// Image scanning & thresholding: read the pixel color in a given
		// window and discard the pixel with dark color
		pthread_mutex_lock(&mcam);
		count = get_image_count(camera_x + (VRES / 2), camera_y + (HRES /  2));
		pthread_mutex_unlock(&mcam);
		
		if (tracking == 0) {
			if (count > 0) {
				// printf("camera_x = %d\ncamera_y = %d\ntracking = %d\n", camera_x, camera_y, tracking);
				tracking = 1;
				cam_x_old = camera_x;
			}
			else {
				if (camera_x <= 0) v = 1;
				else if (camera_x + VRES >= XWORLD) v = -1;

				camera_x += 5 * v;
			}
		}
		if (tracking >= 1) {
			if (tracking <= CAMOV) {
				save_image(VRES / 2, HRES / 2, img[n]);
				if (n == 0) n++;
				else n--;

				// Centroid computation: compute the centroid of pixels with
				// light color
				pthread_mutex_lock(&mcam);
				get_centroid(centroid, camera_x, camera_y);
				pthread_mutex_unlock(&mcam);

				// Camera control: control the camera axes to move to the centroid
				pthread_mutex_lock(&mcam);
				camera_x = centroid[1][0] - (VRES / 2);
				camera_y = centroid[1][1] - (HRES / 2);
				if (camera_x < 0) camera_x = 0;
				if (camera_x > XWORLD - VRES) camera_x = XWORLD - VRES;
				if (camera_y < 0) camera_y = 2;
				pthread_mutex_unlock(&mcam);

				if (tracking == 30) {
					old_x = centroid[0][0];
					old_y = centroid[0][1];
				}

				tracking++;
			}

			if (tracking > CAMOV) {
				// Compute line and prediction
				//old_x = centroid[0][0];
				//old_y = centroid[0][1];
				pthread_mutex_lock(&mcam);
				line_x1 = centroid[1][0];
				line_y1 = centroid[1][1];

				line_y2 = YWORLD - sfondo->h;
				//if (old_x == line_x1) line_x1++;
				if (old_y == line_y1) line_y1++;
				line_x2 = (((line_y2 - old_y) / (line_y1 - old_y)) * (line_x1 - old_x)) + old_x;
				pthread_mutex_unlock(&mcam);
				printf("centroide1 : %d, %d\ncentroide2 : %d, %d \n\n", old_x, old_y, line_x1, line_y1);

				pthread_mutex_lock(&mcam);
				camera_x = cam_x_old;
				camera_y = 2;
				pthread_mutex_unlock(&mcam);

				tracking = 0;
			}
		}

		ptask_wait_for_period();
	}
}
/*----------------------------------------------------------------------*/
/*  Periodic task for drawing   */
/*----------------------------------------------------------------------*/
void draw() {
int k, view;
	
	view = 0;

	while (1) {
		//printf("\nTask draw: id %d, priority %d\n", ptask_get_index(), PRIO);

		clear_to_color(buf, makecol(0, 0, 0));

		// Graphic world
		clear_to_color(bufw, makecol(0, 0, 0));
		draw_sprite(bufw, sfondo, 0, YWORLD - sfondo->h);
		draw_sprite(bufw, patriot, (XWORLD / 2) - (patriot->w / 2) + 10, YWORLD - patriot->h - 20);

		for (k=0; k<MAXT; k++) {
			if (state[k] == ACTIVE)
				//draw_sprite(bufw, aereo, enemy_x[k], enemy_y[k]);
				rotate_sprite(bufw, aereo, enemy_x[k], enemy_y[k], ftofix(alfa[k] * angle[k]));
			else if (state[k] == BOOM) {
				draw_sprite(bufw, boom, enemy_x[k], enemy_y[k]);
				if (view == 3) {
					view = 0;
					state[k] = WAIT;
					i--;
				}
				else
					view++;
			}
		}

		rect(bufw, camera_x, camera_y + HRES, camera_x + VRES, camera_y, makecol(255, 0, 0));
		rect(bufw, 0, YWORLD - 1, XWORLD - 1, 0, makecol(0, 0, 255));
		line(bufw, line_x1, line_y1, line_x2, line_y2, makecol(255, 0, 0));

		// Menu area
		clear_to_color(bufm, makecol(0, 0, 0));
		rect(bufm, 0, YMENU - 1, XMENU - 1, 0, makecol(0, 0, 255));

		// Status window
		clear_to_color(bufs, makecol(0, 0, 0));
		put_image(12 + (VRES / 2), YMENU + (HRES / 2) + 2);
		//rect(bufs, 12, YMENU + HRES + 2, XSTATUS - 12, YMENU + 2, makecol(255, 0, 0));
		rect(bufs, 0, YSTATUS - 1, XSTATUS - 1, 0, makecol(0, 0, 255));

		blit(bufm, buf, 0, 0, 10, 10, bufm->w, bufm->h);
		blit(bufw, buf, 0, 0, 10, YMENU + 20, bufw->w, bufw->h);
		blit(bufs, buf, 0, 0, XMENU + 20, 10, bufs->w, bufs->h);

		blit(buf, screen, 0, 0, 0, 0, buf->w, buf->h);

		ptask_wait_for_period();
	}
}
/*----------------------------------------------------------------------*/
/*  Periodic task for enemy   */
/*----------------------------------------------------------------------*/
void enemy() {
float speed;//, angle, alfa;
int tid;
	
	tid = ptask_get_index();
	enemy_x[tid - 1] = rand() % (XWORLD - aereo->w);
	enemy_y[tid - 1] = 0;
	alfa[tid - 1] = (rand() % 3) - 1;
	speed = (rand() % 3) + 8;
	angle[tid - 1] = (rand() % 3) + 1;

	while (1) {
		//printf("Task enemy: id %d, priority %d, state %d\n", ptask_get_index(), PRIO, state[tid - 1]);

		// se il razzo non è arrivato alla citta scende, altrimenti scoppia
		if (enemy_x[tid - 1] < (XWORLD - aereo->w) && enemy_x[tid - 1] >= 0 && enemy_y[tid - 1] < (YWORLD - sfondo->h - aereo->h)) {
			enemy_y[tid - 1] += speed;
			enemy_x[tid - 1] -= (alfa[tid - 1] * angle[tid - 1]);
		}
		else {
			state[tid - 1] = BOOM;
			ptask_wait_for_activation();
			enemy_x[tid - 1] = rand() % (XWORLD - aereo->w);
			enemy_y[tid - 1] = 0;
			alfa[tid - 1] = (rand() % 3) - 1;
			speed = (rand() % 3) + 8;
			angle[tid - 1] = (rand() % 3) + 1;
		}
		ptask_wait_for_period();
	}
}
/*----------------------------------------------------------------------*/
/*  MAIN process   */
/*----------------------------------------------------------------------*/
int main() {	
int scan, k, one, ntasks = 0;
tpars params;
int last_proc = 0;                  /* last assigned processor      */
int max_proc = ptask_getnumcores(); /* max number of procs  */
	
	i = 0;
	for (k=0; k<MAXT; k++) tid[k] = -1;
	for (k=0; k<MAXT; k++) enemy_x[k] = -1;
	for (k=0; k<MAXT; k++) enemy_y[k] = -1;
	for (k=0; k<MAXT; k++) state[k] = -1;
	srand(time(NULL));
	
	init();
	pmux_create_pi(&mcam);

	sfondo = load_bitmap("img/sfondo3.bmp", NULL);
	if (sfondo == NULL) {
		printf("file not found\n");
		exit(1);
	}
	aereo = load_bitmap("img/aereo.bmp", NULL);
	if (aereo == NULL) {
		printf("file not found\n");
		exit(1);
	}
	boom = load_bitmap("img/boom.bmp", NULL);
	if (boom == NULL) {
		printf("file not found\n");
		exit(1);
	}
	patriot = load_bitmap("img/patriot.bmp", NULL);
	if (patriot == NULL) {
		printf("file not found\n");
		exit(1);
	}
	
	ptask_create_prio(draw, PER / 3, PRIO, NOW);
	ntasks++;
	
	for (k=0; k<MAXT; k++) {
		ptask_param_init(params);
		ptask_param_period(params, PER, MILLI);
		ptask_param_priority(params, PRIO - ntasks);
		ptask_param_activation(params, DEFERRED);
		/* a round robin assignment */
		ptask_param_processor(params, last_proc);
		last_proc++;
		if (last_proc >= max_proc)
			last_proc = 0;
		tid[k] = ptask_create_param(enemy, &params);
		// printf("tid[%d] = %d\n", k, tid[k]);
		state[k] = WAIT;
		ntasks++;
	}

	ptask_create_prio(camera, PER / 2, PRIO - ntasks, NOW);
	ntasks++;

	do {
		scan = 0;
		if (keypressed()) scan = readkey() >> 8;
		if (scan == KEY_SPACE && i < MAXT) {
			for (k=0; k<MAXT; k++) {
				if (state[k] == WAIT && one == 0) {
					// printf("attivo task %d\n", tid[k]);
					one++;
					state[k] = ACTIVE;
					ptask_activate(tid[k]);
					i++;
				}
			}
			one = 0;
		}
	} while (!key[KEY_ESC]);
	
	allegro_exit();
	return 0;
}
/*----------------------------------------------------------------------*/
