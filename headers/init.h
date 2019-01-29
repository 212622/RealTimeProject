/*--------------------------------------------------------------*/
//  INIT.H - header file of init.c
/*--------------------------------------------------------------*/
#if !defined INIT_H
#define INIT_H

/*----------------------------------------------------------------------*/
/*  GLOBAL CONSTRANTS   */
/*----------------------------------------------------------------------*/
#define ALLEGRO_NO_FIX_ALIASES 				// fix for the new version of glibc
/*--------------------------------------------------------------*/
//  GRAPHICS CONSTANTS
/*--------------------------------------------------------------*/
#define XWIN    1024                        // window x resolution
#define YWIN    768                         // window y resolution
/*--------------------------------------------------------------*/
//  TASK CONSTANTS
/*--------------------------------------------------------------*/
#define PER 	20							// base period
#define PRIO    80							// task priority
/*--------------------------------------------------------------*/
/*  STATE CONSTRANTS   */
/*--------------------------------------------------------------*/
#define WAIT	0							// stop enemy/ally state
#define ACTIVE	1							// moving enemy/ally state
#define BOOM	2							// explosion enemy/ally state

/*--------------------------------------------------------------*/
/*  FUNCTION PROTOTYPES   */
/*--------------------------------------------------------------*/
extern void init(void);
extern int create_task(int period, void (*task)(void));

#endif // INIT_H

/*--------------------------------------------------------------*/
