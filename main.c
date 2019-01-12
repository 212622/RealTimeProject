/*----------------------------------------------------------------------*/
/*  Patriots																										*/
/*	Real Time Project by Tomas Torricelli and Lorenzo Cuoghi														*/
/*																													*/
/*	This project simulate a physical system or the behavior of active agents										*/
/* 	interacting with each other and with the environment.															*/
/*	Simulate a set of Patriot defense missiles that identify enemy targets, predict their							*/
/* 	trajectories and are launched to catch them. 																	*/
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
#include "draw.h"
#include "enemy.h"
/*--------------------------------------------------------------*/
/*  FUNCTION PROTOTYPES   */
/*--------------------------------------------------------------*/
int get_image_count(int x0, int y0);
//------------------------------------------------------
// Like GET_IMAGE but also return the number of
// non-black and non-red pixels
//------------------------------------------------------
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
				//old_x = centroid[0][0];
				//old_y = centroid[0][1];
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
/*  Periodic task for commands   */
/*----------------------------------------------------------------------*/
void commands(void) {
	int scan, one = 0, k;

	while (1) {
		scan = 0;
		if (keypressed()) scan = readkey() >> 8;
		if (scan == KEY_SPACE && n_act < MAXE) {
			for (k=0; k<MAXE; k++) {
				if (state[k] == WAIT && one == 0) {
					// printf("attivo task %d\n", tid[k]);
					one++;
					pthread_mutex_lock(&men);
					state[k] = ACTIVE;
					n_act++;
					pthread_mutex_unlock(&men);
					ptask_activate(tid[k]);
				}
			}
			one = 0;
		}

		ptask_wait_for_period();
	}
}
/*----------------------------------------------------------------------*/
/*  MAIN process   */
/*----------------------------------------------------------------------*/
int main(void) {	
	int k, ntasks = 0;						// temporary variable
	int last_proc = 0;                  	// last assigned processor
	int max_proc = ptask_getnumcores();		// max number of procs
	tpars params;
	
	n_act = 0;
	for (k=0; k<MAXE; k++) tid[k] = -1;
	for (k=0; k<MAXE; k++) enemy_x[k] = -1;
	for (k=0; k<MAXE; k++) enemy_y[k] = -1;
	for (k=0; k<MAXE; k++) state[k] = -1;
	srand(time(NULL));
	
	init();
	pmux_create_pi(&mcam);
	pmux_create_pi(&men);

	load_img();
	
	/*	
		Problemi: 
		- alcuni sbagli di direzione => Immagine salvata male
		- camera cattura piu volte lo stesso aereo

		- un file c per ogni task
		- struct per parametri enemy, unico mutex per la struct?
		- suddividere in funzioni più piccole
		- makefile unico obj per i task
		- ricontrollare posizione patriot

		- lunghezza commenti divisiorie
		- variabili globali extern?
		- confronta con ball
		- direzione decisa con frecce?
		- vedi slide_per_progetto
	*/

	// graphic task creation
	ptask_param_init(params);
	ptask_param_period(params, PER / 3, MILLI);
	ptask_param_deadline(params, PER / 3, MILLI);
	ptask_param_priority(params, PRIO);
	ptask_param_activation(params, NOW);
	// a round robin assignment
	ptask_param_processor(params, last_proc);
	last_proc++;
	if (last_proc >= max_proc)
		last_proc = 0;
	ptask_create_param(draw, &params);
	ntasks++;
	
	// enemy task creation
	for (k=0; k<MAXE; k++) {
		ptask_param_init(params);
		ptask_param_period(params, PER, MILLI);
		ptask_param_deadline(params, PER, MILLI);
		ptask_param_priority(params, PRIO - ntasks);
		ptask_param_activation(params, NOW);
		ptask_param_processor(params, last_proc);
		last_proc++;
		if (last_proc >= max_proc)
			last_proc = 0;
		pthread_mutex_lock(&men);
		tid[k] = ptask_create_param(enemy, &params);
		state[k] = WAIT;
		pthread_mutex_unlock(&men);
		// printf("tid[%d] = %d\n", k, tid[k]);
		ntasks++;
	}

	// camera task creation
	ptask_param_init(params);
	ptask_param_period(params, PER / 2, MILLI);
	ptask_param_deadline(params, PER / 2, MILLI);
	ptask_param_priority(params, PRIO - ntasks);
	ptask_param_activation(params, NOW);
	ptask_param_processor(params, last_proc);
	last_proc++;
	if (last_proc >= max_proc)
		last_proc = 0;
	ptask_create_param(camera, &params);
	ntasks++;

	// commands task creation
	ptask_param_init(params);
	ptask_param_period(params, PER * 2, MILLI);
	ptask_param_deadline(params, PER * 2, MILLI);
	ptask_param_priority(params, PRIO - ntasks);
	ptask_param_activation(params, NOW);
	ptask_param_processor(params, last_proc);
	last_proc++;
	if (last_proc >= max_proc)
		last_proc = 0;
	ptask_create_param(commands, &params);
	ntasks++;

	while (!key[KEY_ESC]);

	allegro_exit();
	return 0;
}
/*----------------------------------------------------------------------*/
