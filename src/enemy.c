/*----------------------------------------------------------------------*/
/*  HEADER FILES        */
/*----------------------------------------------------------------------*/
#include <pthread.h>
#include "ptask.h"
#include "init.h"
#include "enemy.h"
#include "draw.h"
/*----------------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*----------------------------------------------------------------------*/
int		enemy_x[MAXE], enemy_y[MAXE];		// coordinates of enemies
int 	state[MAXE];						// enemy state
int		tid[MAXE];							// enemy task IDs
int		n_act;		                      	// number of active enemy tasks
float   en_angle[MAXE];                     // rotation angle

pthread_mutex_t men;						// enemy mutex
/*----------------------------------------------------------------------*/
/*  Periodic task for enemy   */
/*----------------------------------------------------------------------*/
void init_enemy(float *x1, float *x2, float *y1, float *y2, float *m, float *speed) {
	*x1 = rand() % (XWORLD - aereo->w);			// x di partenza
	*x2 = rand() % (XWORLD - aereo->w);			// x di arrivo
	*y1 = 0;									// y di partenza
	*y2 = YWORLD - sfondo->h - aereo->h;		// y di arrivo
	*m = ((*y2) - (*y1)) / ((*x2) - (*x1));		// coeff. angolare
	*speed = (rand() % 3) + 8;
}
/*----------------------------------------------------------------------*/
/*  Periodic task for enemy   */
/*----------------------------------------------------------------------*/
void enemy(void) {
	int tid = ptask_get_index() - 1;			// task index - 1
	float x1, x2, y1, y2, speed;
	float m;

	init_enemy(&x1, &x2, &y1, &y2, &m, &speed);

	ptask_wait_for_activation();
	while (1) {
		if (y1 <= y2) {
			pthread_mutex_lock(&men);
			enemy_x[tid] = x1;
			enemy_y[tid] = y1;
			en_angle[tid] = m;
			pthread_mutex_unlock(&men);

			x1 += (speed / (y2 - y1)) * (x2 - x1);
			y1 += speed;
		}
		else {
			pthread_mutex_lock(&men);
			state[tid] = BOOM;
			pthread_mutex_unlock(&men);

			ptask_wait_for_activation();
			init_enemy(&x1, &x2, &y1, &y2, &m, &speed);
		}
		
		ptask_wait_for_period();
	}
}
/*----------------------------------------------------------------------*/
