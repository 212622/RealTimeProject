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
/*  Periodic task for enemy   */
/*----------------------------------------------------------------------*/
void init_enemy(float *x1, float *x2, float *y1, float *y2, float *m, float *speed) {
	*x1 = rand() % (XWORLD - aereo->w);			// x di partenza
	*x2 = rand() % (XWORLD - aereo->w);			// x di arrivo
	*y1 = 0;									// y di partenza
	*y2 = YWORLD - sfondo->h - aereo->h;		// y di arrivo
	*m = ((*y2) - (*y1)) / ((*x2) - (*x1));		// coeff. angolare
	// *speed = (rand() % 5) + 10;
	*speed = 9;
}
/*----------------------------------------------------------------------*/
/*  Periodic task for enemy   */
/*----------------------------------------------------------------------*/
void enemy(void) {
	int tid = ptask_get_index() - 1;			// task index - 1
	float x1, x2, y1, y2, speed;
	float m;
	// float r = 0, tot_r = 0, n_r = 0, new_r = 0;
	
	init_enemy(&x1, &x2, &y1, &y2, &m, &speed);
	
	ptask_wait_for_activation();
	while (1) {
		
		// r = speed / en_speed;
		// if (r > 0 && r < 100) {
		// 	tot_r += r;
		// 	n_r++;
		// 	new_r = tot_r / n_r;
		// }
		// printf("new_r : %f\n", new_r);

		if (y1 <= y2 && crash_en[tid] == 0) {
			pthread_mutex_lock(&men);
			enemy_x[tid] = x1;
			enemy_y[tid] = y1;
			en_angle[tid] = m;
			pthread_mutex_unlock(&men);

			x1 += (speed / (y2 - y1)) * (x2 - x1);
			y1 += speed;
		}
		else {
			if (crash_en[tid] == 1) {
				pthread_mutex_lock(&men);
				en_died++;
				pthread_mutex_unlock(&men);
			}
			else{
				pthread_mutex_lock(&men);
				en_arrived++;
				pthread_mutex_unlock(&men);
			}

			pthread_mutex_lock(&men);
			state[tid] = BOOM;
			pthread_mutex_unlock(&men);

			pthread_mutex_lock(&mdraw);
			crash_en[tid] = 0;
			pthread_mutex_unlock(&mdraw);

			ptask_wait_for_activation();
			init_enemy(&x1, &x2, &y1, &y2, &m, &speed);
			pthread_mutex_lock(&men);
			enemy_x[tid] = x1;
			enemy_y[tid] = y1;
			en_angle[tid] = m;
			pthread_mutex_unlock(&men);
		}

		/* check for deadline miss */
        if (ptask_deadline_miss()) en_deadline++;
		
		ptask_wait_for_period();
	}
}
/*----------------------------------------------------------------------*/
