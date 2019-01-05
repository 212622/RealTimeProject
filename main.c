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
#define XCAM    200                         // camera y resolution
#define YCAM    200                         // camera y resolution
/*--------------------------------------------------------------*/
#define MAXT    5                           // max number of tasks
#define PER     30                          // base period
#define PRIO    50                          // task priority
/*--------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*--------------------------------------------------------------*/
BITMAP  *aereo, *boom;						// images
int tid[MAXT], mti, i;                      // tasks indexes
/*--------------------------------------------------------------*/
/*  Periodic task for camera detection   */
/*--------------------------------------------------------------*/
void camera(void) {
	int x = 0, y = 0, v = 1, c, k, j;
	int grey = makecol(0, 0, 0);
	
	while (1) {
		rect(screen, x, y + YCAM, XCAM + x, y, makecol(255, 0, 0));
		if (x + XCAM > 1024) v = -1;
		if (x == 0) v = 1;
		x += 2 * v;
		for (k = 0; k<XCAM; k++) {
			for (j=0; j<YCAM; j++) {
				c = getpixel(buf, x + k, y + j);
				if (c != grey) printf("TROVATO: %d %d\n", k, j);
			}
		}
		ptask_wait_for_period();
	}
}
/*--------------------------------------------------------------*/
/*  Periodic task for drawing   */
/*--------------------------------------------------------------*/
void draw(void) {
	while (1) {
		blit(buf, screen, 0, 0, 0, 0, buf->w, buf->h);
		ptask_wait_for_period();
	}
}
/*--------------------------------------------------------------*/
/*  Periodic task for background   */
/*--------------------------------------------------------------*/
void background(void) {
	BITMAP *sfondo = load_bitmap("img/sfondo.bmp", NULL);
	if (sfondo == NULL) {
		printf("file not found\n");
		exit(1);
	}
	
	while (1) {
		clear_to_color(buf, makecol(0, 0, 0));
		draw_sprite(buf, sfondo, 0, 600);
		
		for (int k=0; k<MAXT; k++)
			 if (tid[k] != 0) ptask_activate(tid[k]);
		if (mti != 0) ptask_activate(mti);

		ptask_wait_for_period();
	}
}
/*--------------------------------------------------------------*/
/*  Periodic task for enemy   */
/*--------------------------------------------------------------*/
void enemy(void) {
	float speed = 5;
	float x = rand() % 1024, y = 0, alfa = rand() % 3;
	int a;
	
	// speed = 2 + rand()%20;
	while (1) {
		// alfa usata per decidere se il razzo si sposta a destra sinistra o centrale
		if (alfa == 0) a = 1;
		else if (alfa == 1) a = -1;
		else if (alfa == 2) a = 0;
		
		// se il razzo non è arrivato alla citta scende, altrimenti scoppia
		if (x < 1024 && x > 0 && y < 600 - (aereo->h)) {
			y = y + speed;
			x = x - ((a * speed) / 5);
			draw_sprite(buf, aereo, x, y);
		}
		else {
			draw_sprite(buf, boom, x + 65 - (boom->w), y);
			i--;
			break;	// DA CAMBIARE
		}
		ptask_wait_for_period();
	}
}
/*--------------------------------------------------------------*/
/*  MAIN process   */
/*--------------------------------------------------------------*/
int main(void) {	
	int scan, k;
	tpars params;
	
	i = 0;
	for (k=0; k<MAXT; k++) tid[k] = 0;
	srand(time(NULL));
	
	init();
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
	
	ptask_create_prio(camera, PER, PRIO + 1, NOW);
	ptask_create_prio(background, PER, PRIO, NOW);
	mti = ptask_create_prio(draw, PER, PRIO - MAXT - 1, DEFERRED);
	
	do {
		scan = 0;
		if (keypressed()) scan = readkey() >> 8;
		if (scan == KEY_SPACE && i < MAXT) {
			ptask_param_init(params);
			ptask_param_period(params, PER, MILLI);
			ptask_param_priority(params, PRIO);
			ptask_param_activation(params, DEFERRED);
			tid[i] = ptask_create_param(enemy, &params);
			printf("Task: %d Priorità: %d\n", tid[i], PRIO);
			i++;
		}
	} while (!key[KEY_ESC]);
	
	allegro_exit();
	return 0;
}
/*--------------------------------------------------------------*/
