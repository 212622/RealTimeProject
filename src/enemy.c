/*----------------------------------------------------------------------*/
//	ENEMY.C describes the behavior of enemy planes and their attributes
/*----------------------------------------------------------------------*/
#include <pthread.h>
#include "enemy.h"
#include "ptask.h"
#include "init.h"
#include "draw.h"

/*----------------------------------------------------------------------*/
/*  GLOBAL CONSTRANTS   */
/*----------------------------------------------------------------------*/
#define SPEED 9								// fixed enemy plane speed

/*----------------------------------------------------------------------*/
/*  GLOBAL DATA STRUCTURES   */
/*----------------------------------------------------------------------*/
pthread_mutex_t men;						// mutex for enemy global variables
int		tid_en[MAXE];						// enemy task IDs
int		enemy_x[MAXE], enemy_y[MAXE];		// coordinates of enemies
int 	state_en[MAXE];						// enemy state
float   en_angle[MAXE];						// rotation angle
int		n_en_act;							// number of active enemy tasks
int     en_deadline;						// number of missed enemy deadlines
float	en_tot;								// total numer of created enemy
float	en_died;							// number of destroyed enemy plane
float	en_arrived;							// number of non-destroyed enemy plane

/*----------------------------------------------------------------------*/
//	WRITE_EN_ATT: write enemies attributes in global variables.
//	It requires 4 arguments: the x coordinate of enemy (x1),
//	its y coordinate (y1), its angular coefficient m and
//	its task identifier (tid).
/*----------------------------------------------------------------------*/
void write_en_att(float x1, float y1, float m, int tid) {
	pthread_mutex_lock(&men);
	enemy_x[tid] = x1;
	enemy_y[tid] = y1;
	en_angle[tid] = m;
	pthread_mutex_unlock(&men);
}

/*----------------------------------------------------------------------*/
//	CHECK_DEADLINE_MISS_AL: counts the number of deadline miss in enemy.
/*----------------------------------------------------------------------*/
void check_deadline_miss_en(void) {
    if (ptask_deadline_miss()) {
		pthread_mutex_lock(&men);
		en_deadline++;
		pthread_mutex_unlock(&men);
	}
}

/*----------------------------------------------------------------------*/
//	ENEMY: periodic task fors enemy
/*----------------------------------------------------------------------*/
void enemy(void) {
	int		tid = ptask_get_index() - 1;				// enemy task identifier
	float	x1 = rand() % (XWORLD - aereo->w);			// start x
	float	x2 = rand() % (XWORLD - aereo->w);			// end x
	float	y1 = 0;										// start y
	float	y2 = YWORLD - sfondo->h - aereo->h;			// end y 
	float	speed = SPEED;								// speed of enemy plane
	float	m = (y2 - y1) / (x2 - x1);					// angular coefficient

	ptask_wait_for_activation();
	while (1) {
		if (y1 <= y2 && crash_en[tid] == 0) {
			write_en_att(x1, y1, m, tid);

			x1 += (speed / (y2 - y1)) * (x2 - x1);
			y1 += speed;
		}
		else {
			pthread_mutex_lock(&men);
			state_en[tid] = BOOM;
			pthread_mutex_unlock(&men);
			
			pthread_mutex_lock(&mdraw);
			if (crash_en[tid] == 1)	en_died++;
			else en_arrived++;
			crash_en[tid] = 0;
			pthread_mutex_unlock(&mdraw);

			ptask_wait_for_activation();

			// random initializations
			x1 = rand() % (XWORLD - aereo->w);
			x2 = rand() % (XWORLD - aereo->w);
			y1 = 0;
			m = (y2 - y1) / (x2 - x1);
			write_en_att(x1, y1, m, tid);
		}

        check_deadline_miss_en();
		ptask_wait_for_period();
	}
}

/*----------------------------------------------------------------------*/
