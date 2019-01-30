/*--------------------------------------------------------------*/
//  ENEMY.H - header file of enemy.c
/*--------------------------------------------------------------*/
#if !defined ENEMY_H
#define ENEMY_H

/*--------------------------------------------------------------*/
/*  HEADER FILES        */
/*--------------------------------------------------------------*/
#include "pmutex.h"

/*--------------------------------------------------------------*/
/*  GLOBAL CONSTANTS   */
/*--------------------------------------------------------------*/
#define MAXE    3							        // max number of enemy tasks

/*--------------------------------------------------------------*/
/*  FUNCTION PROTOTYPES   */
/*--------------------------------------------------------------*/
extern void enemy(void);
extern void write_en_att(float x1, float y1, float m, int tid);
extern void check_deadline_miss_en(void);

/*--------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*--------------------------------------------------------------*/
extern pthread_mutex_t men;                         // enemy mutex
extern int		tid_en[MAXE];						// enemy task IDs
extern int		enemy_x[MAXE], enemy_y[MAXE];		// coordinates of enemies
extern int      state_en[MAXE];						// enemy state
extern int      crash_en[MAXE];                     // defines which enemy is crashing
extern float    en_angle[MAXE];						// rotation angle
extern int		n_en_act;							// number of active enemy tasks
extern int      en_deadline;						// number of missed enemy deadlines
extern float	en_tot;								// total numer of created enemy
extern float	en_died;							// number of destroyed enemy plane
extern float	en_arrived;							// number of non-destroyed enemy plane

#endif // ENEMY_H

/*--------------------------------------------------------------*/
