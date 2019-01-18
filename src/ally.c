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
/*  GLOBAL CONSTRANTS   */
/*----------------------------------------------------------------------*/
#define CONVERSION	17
/*----------------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*----------------------------------------------------------------------*/
int		ally_x[MAXA], ally_y[MAXA];			// coordinates of ally
int 	state_al[MAXA];						// ally state
int		tid_al[MAXA];						// ally task IDs
int		n_al_act;		                    // number of active ally tasks
float   al_angle[MAXA];                     // rotation angle
int     al_deadline;

pthread_mutex_t mal;						// ally mutex
/*----------------------------------------------------------------------*/
/*  Periodic task for enemy   */
/*----------------------------------------------------------------------*/
void ally(void) {

	int tid = ptask_get_index() - (MAXE + 1);
	float x1, x2, y1, y2, speed = 8;
	float m;

	x1 = (XWORLD / 2) - (razzo->w / 2) + (BORDER * 3);
	y1 = YWORLD - sfondo->h - razzo->h;
	y2 = YWORLD / 2;

	ptask_wait_for_activation();
	x2 = (((y2 - line_y1) / (line_y2 - line_y1)) * (line_x2 - line_x1)) + line_x1;
	m = (y2 - y1) / (x2 - x1);
	speed =  ((y1 - y2) / ((y2 - line_y1) / en_speed)) * CONVERSION;
	while(1) {
		if (y1 > HRES + 1 && crash_al[tid] == 0) {
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

			pthread_mutex_lock(&mdraw);
			crash_al[tid] = 0;
			pthread_mutex_unlock(&mdraw);

			ptask_wait_for_activation();
			x1 = (XWORLD / 2) - (razzo->w / 2) + (BORDER * 3);
			y1 = YWORLD - sfondo->h - razzo->h;
			y2 = YWORLD / 2;
			x2 = (((y2 - line_y1) / (line_y2 - line_y1)) * (line_x2 - line_x1)) + line_x1;
			m = (y2 - y1) / (x2 - x1);
			pthread_mutex_lock(&mal);
			ally_x[tid] = x1;
			ally_y[tid] = y1;
			al_angle[tid] = m;
			pthread_mutex_unlock(&mal);
		}

		/* check for deadline miss */
        if (ptask_deadline_miss()) al_deadline++;

		ptask_wait_for_period();
	}
}
/*----------------------------------------------------------------------*/
