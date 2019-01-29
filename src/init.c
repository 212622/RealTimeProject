/*----------------------------------------------------------------------*/
/*  HEADER FILES        */
/*----------------------------------------------------------------------*/
#include <allegro.h>
#include "ptask.h"
#include "init.h"
#include "draw.h"
#include "camera.h"
#include "enemy.h"
#include "ally.h"
/*----------------------------------------------------------------------*/
/*  GLOBAL CONSTRANTS   */
/*----------------------------------------------------------------------*/
#define ALLEGRO_NO_FIX_ALIASES 						// fix for the new version of glibc
/*----------------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*----------------------------------------------------------------------*/
int last_proc;										// last assigned processor
int ntasks;											// actual number of tasks
/*----------------------------------------------------------------------*/
/*  FUNCTION DEFINITIONS   */
/*----------------------------------------------------------------------*/
// INIT: Initialization of global variables, mutexes and allegro parameters
/*----------------------------------------------------------------------*/
void init(void) {
	int k;											// temporary variable

	allegro_init();									
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, XWIN, YWIN, 0, 0);
    ptask_init(SCHED_FIFO, GLOBAL, PRIO_INHERITANCE);

	pmux_create_pi(&mcam);
	pmux_create_pi(&men);
	pmux_create_pi(&mal);
	pmux_create_pi(&mdraw);

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

	last_proc = 0;
	ntasks = 0;

	load_img();

	srand(time(NULL));
}
/*----------------------------------------------------------------------*/
// CREATE_TASK: Initialize and create tasks with specified attributes, returning task id.
/*----------------------------------------------------------------------*/
int create_task(int period, void (*task)(void)) {
	int tid;
	tpars params;

	ptask_param_init(params);
	ptask_param_period(params, period, MILLI);
	ptask_param_deadline(params, period, MILLI);
	ptask_param_priority(params, PRIO - ntasks);
	ptask_param_activation(params, NOW);
	ptask_param_measure(params);
	// a round robin assignment
	ptask_param_processor(params, last_proc);
	last_proc++;
	if (last_proc >= ptask_getnumcores())
		last_proc = 0;
	tid = ptask_create_param(task, &params);
	ntasks++;

	return tid;
}
/*----------------------------------------------------------------------*/
