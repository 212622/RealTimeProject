/*----------------------------------------------------------------------*/
/*  HEADER FILES        */
/*----------------------------------------------------------------------*/
#include "ptask.h"
#include "init.h"
#include "enemy.h"
#include "draw.h"
/*----------------------------------------------------------------------*/
/*  Periodic task for enemy   */
/*----------------------------------------------------------------------*/
void enemy(void) {
	int tid = ptask_get_index();					// task index
	float speed = (rand() % 3) + 8;
	
	pthread_mutex_lock(&men);
	enemy_x[tid - 1] = rand() % (XWORLD - aereo->w);
	enemy_y[tid - 1] = 0;
	alfa[tid - 1] = (rand() % 3) - 1;
	angle[tid - 1] = (rand() % 3) + 1;
	pthread_mutex_unlock(&men);

	ptask_wait_for_activation();
	while (1) {
		//printf("Task enemy: id %d, priority %d, state %d\n", ptask_get_index(), PRIO, state[tid - 1]);

		// se il razzo non è arrivato alla citta scende, altrimenti scoppia
		if (enemy_x[tid - 1] < (XWORLD - aereo->w) && enemy_x[tid - 1] >= 0 && enemy_y[tid - 1] < (YWORLD - sfondo->h - aereo->h)) {
			pthread_mutex_lock(&men);
			enemy_y[tid - 1] += speed;
			enemy_x[tid - 1] -= (alfa[tid - 1] * angle[tid - 1]);
			pthread_mutex_unlock(&men);
		}
		else {
			pthread_mutex_lock(&men);
			state[tid - 1] = BOOM;
			pthread_mutex_unlock(&men);

			ptask_wait_for_activation();
			pthread_mutex_lock(&men);
			enemy_x[tid - 1] = rand() % (XWORLD - aereo->w);
			enemy_y[tid - 1] = 0;
			pthread_mutex_unlock(&men);
			alfa[tid - 1] = (rand() % 3) - 1;
			speed = (rand() % 3) + 8;
			angle[tid - 1] = (rand() % 3) + 1;
		}
		ptask_wait_for_period();
	}
}
/*----------------------------------------------------------------------*/
