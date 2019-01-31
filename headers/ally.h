/*--------------------------------------------------------------*/
//  ALLY.H - header file of ally.c
/*--------------------------------------------------------------*/
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
extern pthread_mutex_t mal;                         // mutex for ally global variables
extern int      ally_x[MAXA], ally_y[MAXA];         // coordinates of allies
extern int      state_al[MAXA];                     // ally states
extern int      tid_al[MAXA];                       // ally task IDs
extern int      n_al_act;                           // number of active ally tasks
extern float    al_angle[MAXA];                     // ally rotation angles
extern int      al_deadline;                        // number of missed ally deadlines
extern int      crash_al[MAXA];                     // defines who is crashing

#endif // ALLY_H

/*--------------------------------------------------------------*/
