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
#include "ally.h"
#include "camera.h"
#include "commands.h"
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

	n_al_act = 0;
	for (k=0; k<MAXA; k++) tid_al[k] = -1;
	for (k=0; k<MAXA; k++) ally_x[k] = -1;
	for (k=0; k<MAXA; k++) ally_y[k] = -1;
	for (k=0; k<MAXA; k++) state_al[k] = -1;
	srand(time(NULL));
	
	init();
	pmux_create_pi(&mcam);
	pmux_create_pi(&men);
	pmux_create_pi(&mal);
	pmux_create_pi(&mdraw);

	load_img();
	
	/*	
		Problemi: 
		- alcuni sbagli di direzione => Immagine salvata male
		- camera cattura piu volte lo stesso aereo, se diminuisco la velocità di enemy è più evidente
		- ogni tanto ally scoppia subito -> vattore di linee? o crash? o doppia cattura? -> conflitto indici tid
		
		Rivedere:
		- suddividere in funzioni più piccole
		- mettere a posto commenti
		- camera e linea commentata o linea solo quando stato active? vettore di linee?
		- cambiare nomi variabili enemy
		- 8 / 0.48 ? o calcolare più volte coeffang e velocità e tenerli quando sono costanti

		Features (forse):
		- velocità indipendente dal periodo?
		- controllo deadlines
		- direzione decisa con frecce
		- animazione esplosione
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
		ntasks++;
	}

	// ally task creation
	for (k=0; k<MAXA; k++) {
		ptask_param_init(params);
		ptask_param_period(params, PER, MILLI);
		ptask_param_deadline(params, PER, MILLI);
		ptask_param_priority(params, PRIO - ntasks);
		ptask_param_activation(params, NOW);
		ptask_param_processor(params, last_proc);
		last_proc++;
		if (last_proc >= max_proc)
			last_proc = 0;
		pthread_mutex_lock(&mal);
		tid_al[k] = ptask_create_param(ally, &params);
		state_al[k] = WAIT;
		pthread_mutex_unlock(&mal);
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
