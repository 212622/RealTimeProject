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
int  cam_line_view;
int  command_deadline;
/*----------------------------------------------------------------------*/
/*  Periodic task for commands   */
/*----------------------------------------------------------------------*/
void commands(void) {
	int scan, one = 0, k;
	struct timespec spec;
	int now_s = 0, old_s = 0, now_ms = 0, old_ms = 0;
	cam_line_view = 0;

	pthread_mutex_lock(&men);
	en_tot = en_died = 0;
	pthread_mutex_unlock(&men);

	clock_gettime(CLOCK_REALTIME, &spec);
	old_ms = round(spec.tv_nsec / 1.0e6);
	old_s = spec.tv_sec;

	while (1) {
		scan = 0;
		if (keypressed()) scan = readkey() >> 8;
		if (scan == KEY_SPACE && n_act < MAXE) {
			for (k=0; k<MAXE; k++) {
				if (state[k] == WAIT && one == 0) {
					clock_gettime(CLOCK_REALTIME, &spec);
					now_s = spec.tv_sec;
					now_ms = round(spec.tv_nsec / 1.0e6);
					//printf("now = %d, old = %d, spec.tv_nsec %ld\n", now, old, spec.tv_nsec);
					if (now_s - old_s >= 1 || (now_s - old_s == 0 && now_ms - old_ms >= 500)) {
						old_s = now_s;
						old_ms = now_ms;
						pthread_mutex_lock(&men);
						en_tot++;
						n_act++;
						state[k] = ACTIVE;
						ptask_activate(tid[k]);
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
        if (ptask_deadline_miss()) command_deadline++;

		ptask_wait_for_period();
	}
}
/*----------------------------------------------------------------------*/
