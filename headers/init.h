#if !defined INIT_H
#define INIT_H

/*--------------------------------------------------------------*/
/*  GLOBAL CONSTRANTS   */
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
extern void make_pink_background(char *old, char *new);
/*--------------------------------------------------------------*/

#endif // INIT_H
