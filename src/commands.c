/*----------------------------------------------------------------------*/
/*  HEADER FILES        */
/*----------------------------------------------------------------------*/
#include <allegro.h>
#include <pthread.h>
#include "ptask.h"
#include "commands.h"
#include "enemy.h"
/*----------------------------------------------------------------------*/
/*  Periodic task for commands   */
/*----------------------------------------------------------------------*/
void commands(void) {
	int scan, one = 0, k;

	while (1) {
		scan = 0;
		if (keypressed()) scan = readkey() >> 8;
		if (scan == KEY_SPACE && n_act < MAXE) {
			for (k=0; k<MAXE; k++) {
				if (state[k] == WAIT && one == 0) {
					ptask_activate(tid[k]);
					pthread_mutex_lock(&men);
					n_act++;
					state[k] = ACTIVE;
					pthread_mutex_unlock(&men);
					one++;
				}
			}
			one = 0;
		}

		ptask_wait_for_period();
	}
}
/*----------------------------------------------------------------------*/
