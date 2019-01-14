#if !defined ALLY_H
#define ALLY_H
/*--------------------------------------------------------------*/
/*  GLOBAL CONSTRANTS   */
/*--------------------------------------------------------------*/
#define MAXA    3							    // max number of ally tasks
/*--------------------------------------------------------------*/
/*  FUNCTION PROTOTYPES   */
/*--------------------------------------------------------------*/
extern void ally(void);
/*--------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*--------------------------------------------------------------*/
extern int      ally_x[MAXA], ally_y[MAXA];        // coordinates of ally
extern int      state_al[MAXA];                    // ally state
extern int      tid_al[MAXA];					   // ally task IDs
extern int      n_al_act;		                   // number of active ally tasks
extern float    al_angle[MAXA];                    // rotation angle

extern pthread_mutex_t mal;						   // ally mutex
/*--------------------------------------------------------------*/

#endif // ALLY_H
