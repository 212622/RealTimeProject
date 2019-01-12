#if !defined DRAW_H
#define DRAW_H

/*--------------------------------------------------------------*/
/*  HEADER FILES        */
/*--------------------------------------------------------------*/
#include <allegro.h>
/*--------------------------------------------------------------*/
/*  GLOBAL CONSTRANTS   */
/*--------------------------------------------------------------*/
#define XWORLD  800
#define YWORLD  650
#define XMENU   800
#define YMENU   90
#define XSTATUS 195
#define YSTATUS 750
/*--------------------------------------------------------------*/
#define TEXP	10							// explosion time
/*--------------------------------------------------------------*/
/*  FUNCTION PROTOTYPES   */
/*--------------------------------------------------------------*/
extern void draw(void);
extern int get_crash(int x0, int y0);
extern void load_img(void);
/*--------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*--------------------------------------------------------------*/
BITMAP	*sfondo, *aereo, *boom, *patriot;	// images
BITMAP  *buf;						        // global buffer
BITMAP  *bufm, *bufw, *bufs;                // double buffering
/*--------------------------------------------------------------*/

#endif // DRAW_H
