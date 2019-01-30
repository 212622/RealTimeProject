/*----------------------------------------------------------------------*/
/*  Patriots																										*/
/*	Real Time Project by Tomas Torricelli and Lorenzo Cuoghi														*/
/*																													*/
/*	This project simulate a physical system or the behavior of active agents										*/
/* 	interacting with each other and with the environment.															*/
/*	Simulate a set of Patriot defense missiles that identify enemy targets, predict their							*/
/* 	trajectories and are launched to catch them. 																	*/
/*----------------------------------------------------------------------*/
#include <pthread.h>
#include <allegro.h>
#include "ptask.h"
#include "init.h"
#include "draw.h"
#include "enemy.h"
#include "ally.h"
#include "camera.h"
#include "commands.h"

/*----------------------------------------------------------------------*/
/*  MAIN process   */
/*----------------------------------------------------------------------*/
int main(void) {	
	int k, i;				// temporary variables
	
	init();

	// graphic task creation
	create_task(DPER, draw);
	
	// enemy task creation
	for (k=0; k<MAXE; k++) {
		i = create_task(PER, enemy);
		pthread_mutex_lock(&men);
		tid_en[k] = i;
		state_en[k] = WAIT;
		pthread_mutex_unlock(&men);
	}

	// ally task creation
	for (k=0; k<MAXA; k++) {
		i = create_task(PER, ally);
		pthread_mutex_lock(&mal);
		tid_al[k] = i;
		state_al[k] = WAIT;
		pthread_mutex_unlock(&mal);
	}

	// camera task creation
	create_task(DPER, camera);

	// commands task creation
	create_task(PER, commands);

	while (!key[KEY_ESC]);

	allegro_exit();
	return 0;
}

/*----------------------------------------------------------------------*/
/*
	Problemi:
	- un aereo di troppo quando si tiene premuto

	Rivedere:
	- mettere a posto commenti e ordinamento/spazi
	- struct?
	- #define anche in .c o solo .h? ordine include
	- usa #define per numeri draw
*/
