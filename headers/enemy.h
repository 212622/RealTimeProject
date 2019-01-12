#if !defined ENEMY_H
#define ENEMY_H

/*--------------------------------------------------------------*/
/*  HEADER FILES        */
/*--------------------------------------------------------------*/
#include "pmutex.h"
/*--------------------------------------------------------------*/
/*  GLOBAL CONSTRANTS   */
/*--------------------------------------------------------------*/
#define WAIT	0							// stop enemy state
#define ACTIVE	1							// moving enemy state
#define BOOM	2							// explosion enemy state
/*--------------------------------------------------------------*/
#define MAXE    3							// max number of enemy tasks
/*--------------------------------------------------------------*/
/*  FUNCTION PROTOTYPES   */
/*--------------------------------------------------------------*/
extern void enemy(void);
/*--------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*--------------------------------------------------------------*/
int		enemy_x[MAXE], enemy_y[MAXE];		// coordinates of enemies
int 	state[MAXE];						// enemy state
int		tid[MAXE];							// enemy task IDs
int		n_act;		                      	// number of active enemy tasks
float   angle[MAXE];                        // rotation angle
float   alfa[MAXE];                         // direction

pthread_mutex_t men;						// enemy mutex
/*--------------------------------------------------------------*/

#endif // ENEMY_H
