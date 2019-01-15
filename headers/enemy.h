#if !defined ENEMY_H
#define ENEMY_H

/*--------------------------------------------------------------*/
/*  HEADER FILES        */
/*--------------------------------------------------------------*/
#include "pmutex.h"
/*--------------------------------------------------------------*/
/*  GLOBAL CONSTRANTS   */
/*--------------------------------------------------------------*/
#define MAXE    2							    // max number of enemy tasks
/*--------------------------------------------------------------*/
/*  FUNCTION PROTOTYPES   */
/*--------------------------------------------------------------*/
extern void enemy(void);
/*--------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*--------------------------------------------------------------*/
extern int      enemy_x[MAXE], enemy_y[MAXE];   // coordinates of enemies
extern int      state[MAXE];                    // enemy state
extern int      tid[MAXE];						// enemy task IDs
extern int      n_act;		                    // number of active enemy tasks
extern float    en_tot, en_died, en_arrived;
extern float    en_angle[MAXE];                 // rotation angle

extern pthread_mutex_t men;						// enemy mutex
/*--------------------------------------------------------------*/

#endif // ENEMY_H
