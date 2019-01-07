/*--------------------------------------------------------------*/
/*  Patriots																															*/
/*	Real Time Project by Tomas Torricelli and Lorenzo Cuoghi																			*/
/*																																		*/
/*	This project simulate a physical system or the behavior of active agents interacting with each other and with the environment		*/
/*	Simulate a set of Patriot defense missiles that identify enemy targets, predict their trajectories and are launched to catch them 	*/
/*--------------------------------------------------------------*/
/*  HEADER FILES        */
/*--------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <allegro.h>
#include "ptask.h"
#include "init.h"
/*--------------------------------------------------------------*/
/*  GLOBAL CONSTRANTS   */
/*--------------------------------------------------------------*/
#define MAXT    5						// max number of enemy tasks
#define PER 	30						// base period
#define PRIO    50						// task priority
/*--------------------------------------------------------------*/
#define WAIT	0
#define ACTIVE	1
#define BOOM	2
/*--------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*--------------------------------------------------------------*/
int		tid[MAXT];						// enemy task IDs
int		i;                      		// number of active enemy task
int		enemy_x[MAXT], enemy_y[MAXT];	// coordinates of enemies
int 	state[MAXT];					// enemy state
int		camera_x, camera_y;				// coordinates of camera
BITMAP	*sfondo, *aereo, *boom;			// images
//------------------------------------------------------
// Task that periodically gets images from position
// (XCAM,YCAM) and displays them in position (XD,YD)
//------------------------------------------------------
void cameratask() {
	while (1) {
		// get_image(XCAM, YCAM);
		// put_image(XD, YD);
		ptask_wait_for_period();
	}
}
/*--------------------------------------------------------------*/
/*  Periodic task for camera detection   */
/*--------------------------------------------------------------*/
void camera() {
int v, c, k, j, found;
	
	camera_x = camera_y = 0;
	v = 1;
	found = 0;

	while (1) {
		if (camera_x + VRES >= SCREEN_W) v = -1;
		if (camera_x <= 0) v = 1;

		camera_x += 2 * v;

		get_image(camera_x + 100, camera_y + 100);
		// non stampa bordino rosso
		
		for (j=1; j<HRES; j++) {
			for (k=1; k<VRES; k++) {
				c = getpixel(screen, camera_x + k, camera_y + j);
				if (c != makecol(0, 0, 0) && c != makecol(255, 0, 0)) {
					if (found ==  0) {
						save_image(100, 100, "camera/image.bmp");
						found = 1;
					}
					// printf("TROVATO: %d %d\n", k, j);
				}
			}
		}

		ptask_wait_for_period();
	}
}
/*--------------------------------------------------------------*/
/*  Periodic task for drawing   */
/*--------------------------------------------------------------*/
void draw() {
int k, view;
	
	view = 0;

	while (1) {
		//printf("\nTask draw: id %d, priority %d\n", ptask_get_index(), PRIO);

		clear_to_color(buf, makecol(0, 0, 0));
		draw_sprite(buf, sfondo, 0, 600);

		for (k=0; k<MAXT; k++) {
			if (state[k] == ACTIVE)
				draw_sprite(buf, aereo, enemy_x[k], enemy_y[k]);
			else if (state[k] == BOOM) {
				draw_sprite(buf, boom, enemy_x[k], enemy_y[k]);
				if (view == 3) {
					view = 0;
					state[k] = WAIT;
					i--;
				}
				else
					view++;
			}
		}

		rect(buf, camera_x, camera_y + HRES, camera_x + VRES, camera_y, makecol(255, 0, 0));
		put_image(SCREEN_W - 100, SCREEN_H - 100);

		blit(buf, screen, 0, 0, 0, 0, buf->w, buf->h);
		ptask_wait_for_period();
	}
}
/*--------------------------------------------------------------*/
/*  Periodic task for enemy   */
/*--------------------------------------------------------------*/
void enemy() {
float alfa, speed, angle;
int tid;
	
	tid = ptask_get_index();
	enemy_x[tid - 1] = rand() % (1024 - aereo->w);
	enemy_y[tid - 1] = 0;
	alfa = (rand() % 3) - 1;
	speed = (rand() % 3) + 8;
	angle = (rand() % 3) + 1;

	while (1) {
		//printf("Task enemy: id %d, priority %d, state %d\n", ptask_get_index(), PRIO, state[tid - 1]);

		// se il razzo non è arrivato alla citta scende, altrimenti scoppia
		if (enemy_x[tid - 1] < (1024 - aereo->w) && enemy_x[tid - 1] >= 0 && enemy_y[tid - 1] < (600 - aereo->h)) {
			enemy_y[tid - 1] += speed;
			enemy_x[tid - 1] -= (alfa * angle);
		}
		else {
			state[tid - 1] = BOOM;
			ptask_wait_for_activation();
			enemy_x[tid - 1] = rand() % (1024 - aereo->w);
			enemy_y[tid - 1] = 0;
			alfa = (rand() % 3) - 1;
			speed = (rand() % 3) + 8;
			angle = (rand() % 3) + 1;
		}
		ptask_wait_for_period();
	}
}
/*--------------------------------------------------------------*/
/*  MAIN process   */
/*--------------------------------------------------------------*/
int main() {	
int scan, k, one;
tpars params;
	
	i = 0;
	for (k=0; k<MAXT; k++) tid[k] = -1;
	for (k=0; k<MAXT; k++) enemy_x[k] = -1;
	for (k=0; k<MAXT; k++) enemy_y[k] = -1;
	for (k=0; k<MAXT; k++) state[k] = -1;
	srand(time(NULL));
	
	init();

	sfondo = load_bitmap("img/sfondo.bmp", NULL);
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
	
	ptask_create_prio(draw, PER, PRIO + 1, NOW);
	
	for (k=0; k<MAXT; k++) {
		ptask_param_init(params);
		ptask_param_period(params, PER, MILLI);
		ptask_param_priority(params, PRIO);
		ptask_param_activation(params, DEFERRED);
		tid[k] = ptask_create_param(enemy, &params);
		// printf("tid[%d] = %d\n", k, tid[k]);
		state[k] = WAIT;
	}

	ptask_create_prio(camera, PER, PRIO - 1, NOW);

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
/*--------------------------------------------------------------*/
