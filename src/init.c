/*----------------------------------------------------------------------*/
//  INIT.C takes care of initializations and standard task creation.
/*----------------------------------------------------------------------*/
#include <stdlib.h>
#include <time.h>
#include <sched.h>
#include <allegro.h>
#include "ptask.h"
#include "pmutex.h"
#include "init.h"
#include "draw.h"
#include "camera.h"
#include "enemy.h"
#include "ally.h"

/*----------------------------------------------------------------------*/
/*  GLOBAL CONSTANTS   */
/*----------------------------------------------------------------------*/
#define CDEPTH  32                          // color depth

/*----------------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*----------------------------------------------------------------------*/
static int last_proc;						// last assigned processor
static int ntasks;							// actual number of tasks

/*----------------------------------------------------------------------*/
// INIT: initialization of global variables, mutexes and allegro parameters
/*----------------------------------------------------------------------*/
void init(void) {
	int k;									// temporary variable

	// allegro and ptask libraries initialization
	allegro_init();
    install_keyboard();
    set_color_depth(CDEPTH);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, XWIN, YWIN, 0, 0);
    ptask_init(SCHED_FIFO, GLOBAL, PRIO_INHERITANCE);

	// mutexes initialization
	pmux_create_pi(&mcam);
	pmux_create_pi(&men);
	pmux_create_pi(&mal);
	pmux_create_pi(&mdraw);

	// enemy variables initialization
	n_en_act = 0;
	for (k=0; k<MAXE; k++) tid_en[k] = NONE;
	for (k=0; k<MAXE; k++) enemy_x[k] = 0;
	for (k=0; k<MAXE; k++) enemy_y[k] = 0;
	for (k=0; k<MAXE; k++) state_en[k] = NONE;

	// ally variables initialization
	n_al_act = 0;
	for (k=0; k<MAXA; k++) tid_al[k] = NONE;
	for (k=0; k<MAXA; k++) ally_x[k] = 0;
	for (k=0; k<MAXA; k++) ally_y[k] = 0;
	for (k=0; k<MAXA; k++) state_al[k] = NONE;

	last_proc = 0;
	ntasks = 0;

	load_img();

	srand(time(NULL));
}

/*----------------------------------------------------------------------*/
//	CREATE_TASK: initializes and creates task with specified attributes.
//	It requires a period and the task pointer and return the task id of
//	the created task.
/*----------------------------------------------------------------------*/
int create_task(int period, void (*task)(void)) {
	int tid;								// task identifier
	tpars params;							// parameter for task creation

	ptask_param_init(params);
	ptask_param_period(params, period, MILLI);
	ptask_param_deadline(params, period, MILLI);
	ptask_param_priority(params, PRIO - ntasks);
	ptask_param_activation(params, NOW);
	ptask_param_measure(params);

	// round robin assignment of processors
	ptask_param_processor(params, last_proc);
	last_proc++;
	if (last_proc >= ptask_getnumcores())
		last_proc = 0;
	
	tid = ptask_create_param(task, &params);
	ntasks++;

	return tid;
}

/*----------------------------------------------------------------------*/
