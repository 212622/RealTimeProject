#if !defined INIT_H
#define INIT_H

/*--------------------------------------------------------------*/
/*  GLOBAL CONSTRANTS   */
/*--------------------------------------------------------------*/
#define WAIT	0							// stop enemy/ally state
#define ACTIVE	1							// moving enemy/ally state
#define BOOM	2							// explosion enemy/ally state
/*--------------------------------------------------------------*/
#define XWIN    1024                        // window x resolution
#define YWIN    768                         // window y resolution
/*--------------------------------------------------------------*/
#define PER 	20							// base period
#define PRIO    80							// task priority
/*--------------------------------------------------------------*/
/*  FUNCTION PROTOTYPES   */
/*--------------------------------------------------------------*/
extern void init(void);
extern void make_pink_background(char *oldimg, char *newimg);
extern int create_task(int period, void (*task)(void));
/*--------------------------------------------------------------*/

#endif // INIT_H
