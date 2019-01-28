/*----------------------------------------------------------------------*/
/*  HEADER FILES        */
/*----------------------------------------------------------------------*/
#include <allegro.h>
#include "ptask.h"
#include "init.h"
#include "draw.h"
#include "camera.h"
#include "enemy.h"
#include "ally.h"
/*----------------------------------------------------------------------*/
/*  GLOBAL CONSTRANTS   */
/*----------------------------------------------------------------------*/
#define ALLEGRO_NO_FIX_ALIASES 						// fix for the new version of glibc
/*----------------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*----------------------------------------------------------------------*/
int last_proc;										// last assigned processor
int ntasks;											// actual number of tasks

pthread_mutex_t minit;								// init mutex
/*----------------------------------------------------------------------*/
/*  FUNCTION DEFINITIONS   */
/*----------------------------------------------------------------------*/
void init(void) {
	int k;

	allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, XWIN, YWIN, 0, 0);
    ptask_init(SCHED_FIFO, GLOBAL, PRIO_INHERITANCE);

	pmux_create_pi(&minit);
	pmux_create_pi(&mcam);
	pmux_create_pi(&men);
	pmux_create_pi(&mal);
	pmux_create_pi(&mdraw);

	n_act = 0;
	for (k=0; k<MAXE; k++) tid[k] = -1;
	for (k=0; k<MAXE; k++) enemy_x[k] = -1;
	for (k=0; k<MAXE; k++) enemy_y[k] = -1;
	for (k=0; k<MAXE; k++) state[k] = -1;

	n_al_act = 0;
	for (k=0; k<MAXA; k++) tid_al[k] = -1;
	for (k=0; k<MAXA; k++) ally_x[k] = -1;
	for (k=0; k<MAXA; k++) ally_y[k] = -1;
	for (k=0; k<MAXA; k++) state_al[k] = -1;

	last_proc = 0;
	ntasks = 0;
}
/*----------------------------------------------------------------------*/
int create_task(int period, void (*task)(void)) {
	int tid;
	tpars params;

	ptask_param_init(params);
	ptask_param_period(params, period, MILLI);
	ptask_param_deadline(params, period, MILLI);
	ptask_param_priority(params, PRIO - ntasks);
	ptask_param_activation(params, NOW);
	ptask_param_measure(params);
	// a round robin assignment
	ptask_param_processor(params, last_proc);
	last_proc++;
	if (last_proc >= ptask_getnumcores())
		last_proc = 0;
	tid = ptask_create_param(task, &params);
	ntasks++;

	return tid;
}
/*----------------------------------------------------------------------*/
void make_pink_background(char *oldimg, char *newimg) {
	BITMAP		*img, *imgp;		// pointers to bitmap
	PALETTE     pal;                // color palette
	int         x, y, c;
	int         pink, white;
	
	white = makecol(255, 255, 255);
	pink = makecol(255, 0, 255);
	img = load_bitmap(oldimg, NULL);
	imgp = create_bitmap(img->w, img->h);
	
	for (x=0; x<img->w; x++) {
		for (y=0; y<img->h; y++) {
			c = getpixel(img, x, y);
			if (c == white) c = pink;
			putpixel(imgp, x, y, c);
		}
		get_palette(pal);
		save_bitmap(newimg, imgp, pal);
	}
}
/*----------------------------------------------------------------------*/
