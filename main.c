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
#define PER 	20						// base period
#define PRIO    50						// task priority
/*--------------------------------------------------------------*/
#define WAIT	0
#define ACTIVE	1
#define BOOM	2
#define CAMOV	5						// camera movements
/*--------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*--------------------------------------------------------------*/
int		tid[MAXT];						// enemy task IDs
int		tid_al[MAXT];					// ally task IDs
int		i;                      		// number of active enemy task
int		enemy_x[MAXT], enemy_y[MAXT];	// coordinates of enemies
int		ally_x[MAXT], ally_y[MAXT];	// coordinates of enemies
int 	state[MAXT];					// enemy state
int 	state_al[MAXT];					// ally state
int		camera_x, camera_y;				// coordinates of camera
BITMAP	*sfondo, *aereo, *boom, *razzo, *patriot;	// images
int     line_x1, line_x2, line_y1, line_y2;
int 	crash;
int centroid[CAMOV][2];
/*--------------------------------------------------------------*/
/*  Periodic task for camera detection   */
/*--------------------------------------------------------------*/
void camera() {
int v, count, old_x, old_y, k;
// int centroid[5][2], x1, x2, y1, y2, found, name;
int tracking;
char *img[CAMOV] = {"camera/image1.bmp", "camera/image2.bmp", "camera/image3.bmp", "camera/image4.bmp", "camera/image5.bmp"};
// int c, k, j, npixel;
	
	camera_x = 0;
	camera_y = 2;
	v = 1;
	// found = name = 0;
	count = 0;
	k = 0;

	tracking = 0;
	old_x = old_y = 0;

	while (1) {
		// if (camera_x + VRES >= XWORLD) v = -1;
		// if (camera_x <= 0) v = 1;

		// centroide ok (no pixel non neri non aereo, tolte righe rosse)
		// rifare calcolo linea. spostamento camera. tipo 3
		// no found o invio o altro. riavviare automaticamente

		// Image scanning & thresholding: read the pixel color in a given window and discard the pixel with dark color
		count = get_image_count(camera_x + (VRES / 2), camera_y + (HRES /  2));

		if (tracking == 0) {
			if (count >= 1000) {
				tracking = 1;
			}
			else {
				if (camera_x <= 0) v = 1;
				else if (camera_x + VRES >= XWORLD) v = -1;

				camera_x += 5 * v;
			}
		}
		if (tracking >= 1) {
			if (tracking <= CAMOV) {
				save_image(VRES / 2, HRES / 2, img[tracking - 1]);

				// Centroid computation: compute the centroid of pixels with light color
				get_centroid(centroid, tracking - 1, camera_x, camera_y);

				// Camera control: control the camera axes to move to the centroid
				camera_x = centroid[tracking - 1][0] - (VRES / 2);
				camera_y = centroid[tracking - 1][1] - (HRES / 2);
				if (camera_x < 0) camera_x = 0;
				if (camera_x > XWORLD - VRES) camera_x = XWORLD - VRES;
				if (camera_y < 0) camera_y = 2;

				tracking++;
			}

			if (tracking > CAMOV) {
				// Compute line and prediction
				old_x = centroid[CAMOV - 2][0];
				old_y = centroid[CAMOV - 2][1];
				line_x1 = centroid[CAMOV - 1][0];
				line_y1 = centroid[CAMOV - 1][1];

				line_y2 = 300;
				if (old_x == line_x1) line_x1++;
				if (old_y == line_y1) line_y1++;
				line_x2 = (((line_y2 - old_y) / (line_y1 - old_y)) * (line_x1 - old_x)) + old_x;

				if (k < MAXT) {
					state_al[k] = ACTIVE;
					ptask_activate(tid_al[k]);
					i++;
					k++;
				}
				else k = 0;

				tracking = 0;

				camera_x = 0;
				camera_y = 2;
			}
		}

		// if (found ==  0 && count > 1000) {
		// 	save_image(VRES / 2, HRES /  2, img[name]);

			// get_centroid(centroid, name, camera_x, camera_y);

			// camera_x += 5 * v;

			// printf("Image : %s Centroide : %d %d\n", img[name], centroid[name][0], centroid[name][1]);
		// 	if (name == 4) {
		// 		found = 1;
		// 		printf("FOUND: COUNT = %d\n", count);
		// 		x1 = centroid[0][0];
		// 		y1 = centroid[0][1];
		// 		x2 = centroid[4][0];
		// 		y2 = centroid[4][1];
		// 		line_x1 = x1;
		// 		line_y1 = y1;
		// 		line_y2 = YWORLD - sfondo->h;
		// 		if(y1 == y2) y2++; // no. riga orizzontale, non bisogna disegnare
		// 		line_x2 = (((line_y2 - y1) / (y2 - y1)) * (x2 - x1)) + x1;
		// 		name = 0;
		// 	}
		// 	else name++;
		// }
		// if(key[KEY_ENTER]) found = 0;

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

		// Graphic world
		clear_to_color(bufw, makecol(0, 0, 0));
		draw_sprite(bufw, sfondo, 0, YWORLD - sfondo->h);
		draw_sprite(bufw, patriot, (XWORLD / 2) - (patriot->w / 2), YWORLD - patriot->h);

		for (k=0; k<MAXT; k++) {
			if (state[k] == ACTIVE)
				draw_sprite(bufw, aereo, enemy_x[k], enemy_y[k]);
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
			if (state_al[k] == ACTIVE)
				draw_sprite(bufw, razzo, ally_x[k], ally_y[k]);
			else if (state_al[k] == BOOM) {
				draw_sprite(bufw, boom, ally_x[k], ally_y[k]);
				if (view == 3) {
					view = 0;
					state_al[k] = WAIT;
					i--;
				}
				else
					view++;
			}
		}

		rect(bufw, camera_x, camera_y + HRES, camera_x + VRES, camera_y, makecol(255, 0, 0));
		rect(bufw, 0, YWORLD - 1, XWORLD, 0, makecol(0, 0, 255));
		// printf("LINEA : %d %d %d %d\n", line_x1, line_y1, line_x2, line_y2);
		line(bufw, line_x1, line_y1, line_x2, line_y2, makecol(255, 0, 0));

		// Menu area
		clear_to_color(bufm, makecol(0, 0, 0));
		rect(bufm, 0, YMENU, XMENU, 0, makecol(0, 0, 255));

		// Status window
		clear_to_color(bufs, makecol(0, 0, 0));
		put_image(XSTATUS - (VRES / 2) - 12, YSTATUS - (HRES / 2) - 12);
		rect(bufs, 12, YSTATUS - 12, XSTATUS - 12, YSTATUS - HRES - 12, makecol(255, 0, 0));
		rect(bufs, 0, YSTATUS - 1, XSTATUS - 1, 0, makecol(0, 0, 255));

		blit(bufm, buf, 0, 0, 0, 0, bufm->w, bufm->h);
		blit(bufw, buf, 0, 0, 0, YMENU, bufw->w, bufw->h);
		blit(bufs, buf, 0, 0, XMENU, 0, bufs->w, bufs->h);

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
	enemy_x[tid - 1] = rand() % (XWORLD - aereo->w);
	enemy_y[tid - 1] = 0;
	alfa = (rand() % 3) - 1;
	speed = 4;
	angle = (rand() % 3) + 1;
	crash = 0;
	while (1) {
		//printf("Task enemy: id %d, priority %d, state %d\n", ptask_get_index(), PRIO, state[tid - 1]);

		// se il razzo non è arrivato alla citta scende, altrimenti scoppia
		if (crash == 0 && enemy_x[tid - 1] < (XWORLD - aereo->w) && enemy_x[tid - 1] >= 0 && enemy_y[tid - 1] < (YWORLD - sfondo->h - aereo->h)) {
			enemy_y[tid - 1] += speed;
			enemy_x[tid - 1] -= (alfa * angle);
		}
		else {
			state[tid - 1] = BOOM;
			ptask_wait_for_activation();
			enemy_x[tid - 1] = rand() % (XWORLD - aereo->w);
			enemy_y[tid - 1] = 0;
			alfa = (rand() % 3) - 1;
			speed = 4;
			angle = (rand() % 3) + 1;
		}
		ptask_wait_for_period();
	}
}
/*--------------------------------------------------------------*/
/*  Periodic task for ally   */
/*--------------------------------------------------------------*/
void ally() {
float alfa, speed, stop;
int tid_al;
	
	tid_al = ptask_get_index();
	ally_x[tid_al - 6] = (XWORLD / 2);
	ally_y[tid_al - 6] = YWORLD - patriot->h;
	//alfa = (line_y2 - ally_y[tid_al - 6]) / (line_x2 - ally_x[tid_al - 6]);
	alfa = 0;
	if (ally_x[tid_al - 6] < line_x2) alfa = 5;
	if (ally_x[tid_al - 6] > line_x2) alfa = -5;
	printf("line_x2: %d x: %d line_y2: %d y: %d alfa: %f\n",line_x2, ally_x[tid_al - 6], line_y2,ally_y[tid_al - 6], alfa);	
	speed = (5 * (411 - centroid[0][1])) / 516;
	stop = 0;
	crash = 0;

	while (1) {
		//printf("Task ally: id %d, priority %d, state %d\n", ptask_get_index(), PRIO, state_al[tid_al - 1]);

		if (alfa < 0){
			if (ally_x[tid_al - 6] < line_x2) stop = 1;
		}
		else{
			if (ally_x[tid_al - 6] > line_x2) stop = 1;
		}

		if (ally_y[tid_al - 6] < 300) stop = 1;

		if (stop == 0) ally_x[tid_al - 6] += alfa;

		ally_y[tid_al - 6] -= speed/2;

		crash = get_crash(ally_x[tid_al - 6], ally_y[tid_al - 6]);
		printf("crash : %d\n", crash);

		if (crash == 1 || ally_y[tid_al - 6] < 300){
			state_al[tid_al - 6] = BOOM;
			ptask_wait_for_activation();
			ally_x[tid_al - 6] = (XWORLD / 2);
			ally_y[tid_al - 6] = YWORLD - patriot->h;
			alfa = 0;
			if (ally_x[tid_al - 6] < line_x2) alfa = 1;
			if (ally_x[tid_al - 6] > line_x2) alfa = -1;
			speed = (5 * (411 - centroid[0][1])) / 516;
			crash = 0;
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
	for (k=0; k<MAXT; k++) tid_al[k] = -1;
	for (k=0; k<MAXT; k++) enemy_x[k] = -1;
	for (k=0; k<MAXT; k++) enemy_y[k] = -1;
	for (k=0; k<MAXT; k++) ally_x[k] = -1;
	for (k=0; k<MAXT; k++) ally_y[k] = -1;
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
	razzo = load_bitmap("img/razzo.bmp", NULL);
	if (razzo == NULL) {
		printf("file not found\n");
		exit(1);
	}
	patriot = load_bitmap("img/patriot.bmp", NULL);
	if (patriot == NULL) {
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
	
	for (k=0; k<MAXT; k++) {
		ptask_param_init(params);
		ptask_param_period(params, PER, MILLI);
		ptask_param_priority(params, PRIO);
		ptask_param_activation(params, DEFERRED);
		tid_al[k] = ptask_create_param(ally, &params);
		// printf("tid[%d] = %d\n", k, tid[k]);
		state_al[k] = WAIT;
	}

	ptask_create_prio(camera, PER, PRIO, NOW);

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
