#if !defined ENEMY_H
#define ENEMY_H

/*--------------------------------------------------------------*/
/*  HEADER FILES        */
/*--------------------------------------------------------------*/
#include "pmutex.h"
/*--------------------------------------------------------------*/
/*  GLOBAL CONSTRANTS   */
/*--------------------------------------------------------------*/
#define MAXE    3							    // max number of enemy tasks
/*--------------------------------------------------------------*/
/*  FUNCTION PROTOTYPES   */
/*--------------------------------------------------------------*/
extern void enemy(void);
extern void init_enemy(float *x1, float *x2, float *y1, float *y2, float *m, float *speed);
extern void write_en_att(float x1, float y1, float m, int tid);
extern void check_deadline_miss_en(void);
/*--------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*--------------------------------------------------------------*/
extern int      enemy_x[MAXE], enemy_y[MAXE];   // coordinates of enemies
extern int      state[MAXE];                    // enemy state
extern int      tid[MAXE];						// enemy task IDs
extern int      n_act;		                    // number of active enemy tasks
extern float    en_tot, en_died, en_arrived;
extern float    en_angle[MAXE];                 // rotation angle
extern int      en_deadline;

extern pthread_mutex_t men;						// enemy mutex
/*--------------------------------------------------------------*/

#endif // ENEMY_H
