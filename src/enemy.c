/*----------------------------------------------------------------------*/
/*  HEADER FILES        */
/*----------------------------------------------------------------------*/
#include <pthread.h>
#include "ptask.h"
#include "init.h"
#include "enemy.h"
#include "draw.h"
/*--------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*--------------------------------------------------------------*/
int		enemy_x[MAXE], enemy_y[MAXE];		// coordinates of enemies
int 	state[MAXE];						// enemy state
int		tid[MAXE];							// enemy task IDs
int		n_act;		                      	// number of active enemy tasks
float   en_angle[MAXE];                     // rotation angle
// float   alfa[MAXE];                         // direction

pthread_mutex_t men;						// enemy mutex
/*----------------------------------------------------------------------*/
/*  Periodic task for enemy   */
/*----------------------------------------------------------------------*/
// void enemy2(void) {
// 	int tid = ptask_get_index();					// task index
// 	float speed = (rand() % 3) + 8;
	
// 	pthread_mutex_lock(&men);
// 	enemy_x[tid - 1] = rand() % (XWORLD - aereo->w);
// 	enemy_y[tid - 1] = 0;
// 	alfa[tid - 1] = (rand() % 3) - 1;
// 	angle[tid - 1] = (rand() % 3) + 1;
// 	pthread_mutex_unlock(&men);

// 	ptask_wait_for_activation();
// 	while (1) {
// 		//printf("Task enemy: id %d, priority %d, state %d\n", ptask_get_index(), PRIO, state[tid - 1]);

// 		// se il razzo non è arrivato alla citta scende, altrimenti scoppia
// 		if (enemy_x[tid - 1] < (XWORLD - aereo->w) && enemy_x[tid - 1] >= 0 && enemy_y[tid - 1] < (YWORLD - sfondo->h - aereo->h)) {
// 			pthread_mutex_lock(&men);
// 			enemy_y[tid - 1] += speed;
// 			enemy_x[tid - 1] += (alfa[tid - 1] * angle[tid - 1]);
// 			pthread_mutex_unlock(&men);
// 		}
// 		else {
// 			pthread_mutex_lock(&men);
// 			state[tid - 1] = BOOM;
// 			pthread_mutex_unlock(&men);

// 			ptask_wait_for_activation();
// 			pthread_mutex_lock(&men);
// 			enemy_x[tid - 1] = rand() % (XWORLD - aereo->w);
// 			enemy_y[tid - 1] = 0;
// 			alfa[tid - 1] = (rand() % 3) - 1;
// 			angle[tid - 1] = (rand() % 3) + 1;
// 			pthread_mutex_unlock(&men);
// 			speed = (rand() % 3) + 8;
// 		}
// 		ptask_wait_for_period();
// 	}
// }
// /*----------------------------------------------------------------------*/
void init_enemy(float *x1, float *x2, float *y1, float *y2, float *m, float *speed) {
	*x1 = rand() % (XWORLD - aereo->w);			// x di partenza
	*x2 = rand() % (XWORLD - aereo->w);			// x di arrivo
	*y1 = 0;									// y di partenza
	*y2 = YWORLD - sfondo->h - aereo->h;		// y di arrivo
	*m = ((*y2) - (*y1)) / ((*x2) - (*x1));		// coeff. angolare
	*speed = (rand() % 3) + 8;
}
/*----------------------------------------------------------------------*/
// prova di enemy con traiettoria
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
			// printf("x1 = %f, x2 = %f, y1 = %f, y2 = %f, m = %f, speed = %f\n", x1, x2, y1, y2, m, speed);
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
