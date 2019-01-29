#if !defined DRAW_H
#define DRAW_H

/*--------------------------------------------------------------*/
/*  HEADER FILES        */
/*--------------------------------------------------------------*/
#include <allegro.h>
#include "pmutex.h"
#include "enemy.h"
#include "ally.h"
/*--------------------------------------------------------------*/
/*  GLOBAL CONSTRANTS   */
/*--------------------------------------------------------------*/
#define XWORLD  800
#define YWORLD  650
#define XMENU   800
#define YMENU   84
#define XSTATUS 190
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
extern void get_crash_en(int k);
extern void get_crash_al(int k);
extern void draw(void);
extern void load_img(void);
extern void print_menu_area(void);
extern void print_status_window(void);
extern void print_screen(void);
extern void init_draw(void);
extern void view_cam_line(void);
extern void per_stats_calc(void);
extern void print_world(void);
extern void en_movement_draw(void);
extern void al_movement_draw(void);
/*--------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*--------------------------------------------------------------*/
extern BITMAP   *sfondo, *aereo, *razzo;    // images
extern BITMAP   *bufw;                      // double buffering
extern int      crash_al[MAXA];
extern int      crash_en[MAXE];
extern int      draw_deadline;

extern pthread_mutex_t mdraw;               // draw mutex
/*--------------------------------------------------------------*/

#endif // DRAW_H
