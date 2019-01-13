#if !defined DRAW_H
#define DRAW_H

/*--------------------------------------------------------------*/
/*  HEADER FILES        */
/*--------------------------------------------------------------*/
#include <allegro.h>
#include "pmutex.h"
/*--------------------------------------------------------------*/
/*  GLOBAL CONSTRANTS   */
/*--------------------------------------------------------------*/
#define XWORLD  800
#define YWORLD  650
#define XMENU   800
// #define YMENU   90
#define YMENU   84
// #define XSTATUS 195
#define XSTATUS 190
// #define YSTATUS 750
#define YSTATUS 746
/*--------------------------------------------------------------*/
#define BORDER  10
/*--------------------------------------------------------------*/
#define TEXP	10							// explosion time
/*--------------------------------------------------------------*/
#define ALLEGRO_NO_FIX_ALIASES 				// fix for the new version of glibc
/*--------------------------------------------------------------*/
/*  FUNCTION PROTOTYPES   */
/*--------------------------------------------------------------*/
extern void draw(void);
extern int get_crash(int x0, int y0);
extern void load_img(void);
/*--------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*--------------------------------------------------------------*/
extern BITMAP   *sfondo, *aereo;            // images
extern BITMAP   *bufw;                      // double buffering

pthread_mutex_t mdraw;                      // draw mutex
/*--------------------------------------------------------------*/

#endif // DRAW_H
