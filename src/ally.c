/*----------------------------------------------------------------------*/
/*  HEADER FILES        */
/*----------------------------------------------------------------------*/
#include <pthread.h>
#include "ptask.h"
#include "init.h"
#include "ally.h"
#include "draw.h"
#include "camera.h"
/*----------------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*----------------------------------------------------------------------*/
int		ally_x[MAXA], ally_y[MAXA];			// coordinates of ally
int 	state_al[MAXA];						// ally state
int		tid_al[MAXA];						// ally task IDs
int		n_al_act;		                    // number of active ally tasks
float   al_angle[MAXA];                     // rotation angle

pthread_mutex_t mal;						// ally mutex
/*----------------------------------------------------------------------*/
/*  Periodic task for enemy   */
/*----------------------------------------------------------------------*/
void ally(void) {

	int tid = ptask_get_index() - 4;
	float x1, x2, y1, y2, speed = 5;
	float m;

	x1 = (XWORLD / 2) - (razzo->w / 2);
	y1 = YWORLD - sfondo->h - razzo->h;

	ptask_wait_for_activation();
	x2 = line_x2;
	y2 = line_y2;
	m = (y2 - y1) / (x2 - x1);
	while(1) {
		// printf("crash : %d\n",get_crash(ally_x[tid] + (razzo->w / 2), ally_y[tid]));
		// printf("x: %d, y: %d\n",ally_x[tid] + (razzo->w / 2), ally_y[tid]);
		if (y1 > y2) {
			pthread_mutex_lock(&mal);
			ally_x[tid] = x1;
			ally_y[tid] = y1;
			al_angle[tid] = m;
			pthread_mutex_unlock(&mal);
			
			x1 += (-speed / (y2 - y1)) * (x2 - x1);
			y1 -= speed;
		}
		else {
			pthread_mutex_lock(&mal);
			state_al[tid] = BOOM;
			pthread_mutex_unlock(&mal);

			ptask_wait_for_activation();
			x1 = (XWORLD / 2) - (razzo->w / 2);
			y1 = YWORLD - sfondo->h - razzo->h;
			x2 = line_x2;
			y2 = line_y2;
			m = (y2 - y1) / (x2 - x1);
		}
		ptask_wait_for_period();
	}
	

}
/*----------------------------------------------------------------------*/