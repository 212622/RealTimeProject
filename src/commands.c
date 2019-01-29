/*----------------------------------------------------------------------*/
/*  HEADER FILES        */
/*----------------------------------------------------------------------*/
#include <time.h>
#include <math.h>
#include <allegro.h>
#include <pthread.h>
#include "ptask.h"
#include "commands.h"
#include "enemy.h"
#include "init.h"
#include "draw.h"
/*----------------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*----------------------------------------------------------------------*/
int  scan;									// scanning variable
int  one;									// temporary variable
int  old_s, old_ms;				  			// time of last activation
int  cam_line_view;							// camera and line visualization variable
int  command_deadline;						// deadline counter of command task

pthread_mutex_t mcom;                       // command mutex
/*----------------------------------------------------------------------*/
/*  FUNCTION DEFINITIONS   */
/*----------------------------------------------------------------------*/
// INIT_COMMAND: initialize command variables and saves initial clock.
/*----------------------------------------------------------------------*/
void init_command(void) {
	struct timespec spec;

	old_s = one = old_ms = 0;
	cam_line_view = 0;

	pthread_mutex_lock(&men);
	en_tot = en_died = 0;
	pthread_mutex_unlock(&men);

	clock_gettime(CLOCK_REALTIME, &spec);
	old_ms = round(spec.tv_nsec / 1.0e6);
	old_s = spec.tv_sec;
}
/*----------------------------------------------------------------------*/
//	CHECK_DEADLINE_MISS_COM: counts the number of deadline miss in command.
/*----------------------------------------------------------------------*/
void check_deadline_miss_com(void) {
    if (ptask_deadline_miss()) {
		pthread_mutex_lock(&mcom);
		command_deadline++;
		pthread_mutex_unlock(&mcom);
	}
}
/*----------------------------------------------------------------------*/
//	COMMAND: periodic task for commands.
/*----------------------------------------------------------------------*/
void commands(void) {
	int k;
	int now_ms =0, now_s = 0;
	struct timespec spec;

	init_command();

	while (1) {
		scan = 0;
		if (keypressed()) scan = readkey() >> 8;
		if (scan == KEY_SPACE && n_en_act < MAXE) {
			for (k=0; k<MAXE; k++) {
				if (state_en[k] == WAIT && one == 0) {
					clock_gettime(CLOCK_REALTIME, &spec);
					now_s = spec.tv_sec;
					now_ms = round(spec.tv_nsec / 1.0e6);
					if (now_s - old_s >= 1 || (now_s - old_s == 0 && now_ms - old_ms >= 500)) {
						old_s = now_s;
						old_ms = now_ms;
						pthread_mutex_lock(&men);
						en_tot++;
						n_en_act++;
						state_en[k] = ACTIVE;
						ptask_activate(tid_en[k]);
						pthread_mutex_unlock(&men);
					}
					one++;
				}
			}
			one = 0;
		}
		else if (scan == KEY_Q) {
			if (cam_line_view == 0) cam_line_view = 1;
			else cam_line_view = 0;
		}
		
		/* check for deadline miss */
        check_deadline_miss_com();
		ptask_wait_for_period();
	}
}
/*----------------------------------------------------------------------*/
