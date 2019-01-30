#if !defined ALLY_H
#define ALLY_H
/*--------------------------------------------------------------*/
/*  GLOBAL CONSTRANTS   */
/*--------------------------------------------------------------*/
#define MAXA    3                                   // max number of ally tasks
/*--------------------------------------------------------------*/
/*  FUNCTION PROTOTYPES   */
/*--------------------------------------------------------------*/
extern void ally(void);
extern void write_al_att(float x1, float y1, float m, int tid);
extern void check_deadline_miss_al(void);
/*--------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*--------------------------------------------------------------*/
extern int      ally_x[MAXA], ally_y[MAXA];         // coordinates of ally
extern int      state_al[MAXA];                     // ally state
extern int      tid_al[MAXA];                       // ally task IDs
extern int      n_al_act;                           // number of active ally tasks
extern float    al_angle[MAXA];                     // rotation angle
extern int      al_deadline;
extern int      crash_al[MAXA];                     // defines who is crashing

extern pthread_mutex_t mal;                         // ally mutex
/*--------------------------------------------------------------*/

#endif // ALLY_H
