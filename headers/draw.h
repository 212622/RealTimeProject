/*--------------------------------------------------------------*/
//  DRAW.H - header file of draw.c
/*--------------------------------------------------------------*/
#if !defined DRAW_H
#define DRAW_H

/*--------------------------------------------------------------*/
/*  HEADER FILES        */
/*--------------------------------------------------------------*/
#include <allegro.h>
#include "pmutex.h"

/*--------------------------------------------------------------*/
/*  GRAPHICS CONSTANTS   */
/*--------------------------------------------------------------*/
#define XWORLD  800                             // graphic world x resolution
#define YWORLD  650                             // graphic world y resolution
#define XMENU   800                             // menu area x resolution
#define YMENU   84                              // menu area y resolution
#define XSTATUS 190                             // status window x resolution
#define YSTATUS 746                             // status window y resolution

/*--------------------------------------------------------------*/
/*  FUNCTION PROTOTYPES   */
/*--------------------------------------------------------------*/
extern void draw(void);
extern void load_img(void);
extern void init_draw(void);
extern void get_crash_en(int k);
extern void get_crash_al(int k);
extern void print_screen(void);
extern void print_graphic_world(void);
extern void print_menu_area(void);
extern void print_status_window(void);
extern void draw_enemy(void);
extern void draw_ally(void);
extern void view_cam_line(void);
extern void check_deadline_miss_draw(void);

/*--------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*--------------------------------------------------------------*/
extern pthread_mutex_t mdraw;                   // mutex for draw global variables
extern BITMAP   *background, *plane, *rocket;   // images
extern BITMAP   *bufw;                          // sub-window buffer for graphic world

#endif // DRAW_H

/*--------------------------------------------------------------*/
