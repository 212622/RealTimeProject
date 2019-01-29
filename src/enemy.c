/*----------------------------------------------------------------------*/
/*  HEADER FILES        */
/*----------------------------------------------------------------------*/
#include <pthread.h>
#include "ptask.h"
#include "init.h"
#include "enemy.h"
#include "draw.h"
// #include "camera.h"
/*----------------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*----------------------------------------------------------------------*/
int		enemy_x[MAXE], enemy_y[MAXE];		// coordinates of enemies
int 	state[MAXE];						// enemy state
int		tid[MAXE];							// enemy task IDs
int		n_act;		                      	// number of active enemy tasks
float	en_tot, en_died, en_arrived;
float   en_angle[MAXE];                     // rotation angle
int     en_deadline;

pthread_mutex_t men;						// enemy mutex
/*----------------------------------------------------------------------*/
/*  FUNCTION DEFINITIONS   */
/*----------------------------------------------------------------------*/
//	INIT_ENEMY: initialize enemy parameters and variables
/*----------------------------------------------------------------------*/
void init_enemy(float *x1, float *x2, float *y1, float *y2, float *m, float *speed) {
	*x1 = rand() % (XWORLD - aereo->w);			// x di partenza
	*x2 = rand() % (XWORLD - aereo->w);			// x di arrivo
	*y1 = 0;									// y di partenza
	*y2 = YWORLD - sfondo->h - aereo->h;		// y di arrivo
	*m = ((*y2) - (*y1)) / ((*x2) - (*x1));		// coeff. angolare
	*speed = 9;
}
/*----------------------------------------------------------------------*/
//	WRITE_EN_ATT: write enemies attributes in global variables.
/*----------------------------------------------------------------------*/
void write_en_att(float x1, float y1, float m, int tid) {
	pthread_mutex_lock(&men);
	enemy_x[tid] = x1;
	enemy_y[tid] = y1;
	en_angle[tid] = m;
	pthread_mutex_unlock(&men);

}
/*----------------------------------------------------------------------*/
//	CHECK_DEADLINE_MISS_AL: counts the number of deadline miss in ally.
/*----------------------------------------------------------------------*/
void check_deadline_miss_en(void) {
    if (ptask_deadline_miss()) {
		pthread_mutex_lock(&men);
		en_deadline++;
		pthread_mutex_unlock(&men);
	}
}
/*----------------------------------------------------------------------*/
// ENEMY: periodic task fors enemy
/*----------------------------------------------------------------------*/
void enemy(void) {
	int tid = ptask_get_index() - 1;			// task index - 1
	float x1, x2, y1, y2, speed;
	float m;
	
	init_enemy(&x1, &x2, &y1, &y2, &m, &speed);

	ptask_wait_for_activation();
	while (1) {

		if (y1 <= y2 && crash_en[tid] == 0) {
			write_en_att(x1, y1, m, tid);

			x1 += (speed / (y2 - y1)) * (x2 - x1);
			y1 += speed;
		}
		else {

			pthread_mutex_lock(&mdraw);
			if (crash_en[tid] == 1)	en_died++;
			else en_arrived++;
			crash_en[tid] = 0;
			pthread_mutex_unlock(&mdraw);

			pthread_mutex_lock(&men);
			state[tid] = BOOM;
			pthread_mutex_unlock(&men);

			ptask_wait_for_activation();
			init_enemy(&x1, &x2, &y1, &y2, &m, &speed);
			write_en_att(x1, y1, m, tid);
		}

		/* check for deadline miss */
        check_deadline_miss_en();
		
		ptask_wait_for_period();
	}
}
/*----------------------------------------------------------------------*/
