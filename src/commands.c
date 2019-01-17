/*----------------------------------------------------------------------*/
/*  HEADER FILES        */
/*----------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------*/
/*  Periodic task for commands   */
/*----------------------------------------------------------------------*/
void commands(void) {
	int scan, one = 0, k;
	cam_line_view = 0;

	pthread_mutex_lock(&men);
	en_tot = en_died = 0;
	pthread_mutex_unlock(&men);

	while (1) {
		scan = 0;
		if (keypressed()) scan = readkey() >> 8;
		if (scan == KEY_SPACE && n_act < MAXE) {
			for (k=0; k<MAXE; k++) {
				if (state[k] == WAIT && one == 0) {
					pthread_mutex_lock(&men);
					en_tot++;
					n_act++;
					state[k] = ACTIVE;
					ptask_activate(tid[k]);
					pthread_mutex_unlock(&men);
					one++;
				}
			}
			one = 0;
		}
		else if (scan == KEY_Q) {
			if (cam_line_view == 0) cam_line_view = 1;
			else cam_line_view = 0;
		}
		
		ptask_wait_for_period();
	}
}
/*----------------------------------------------------------------------*/
