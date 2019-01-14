/*----------------------------------------------------------------------*/
/*  HEADER FILES        */
/*----------------------------------------------------------------------*/
#include <math.h>
#include <pthread.h>
#include "ptask.h"
#include "init.h"
#include "draw.h"
#include "enemy.h"
#include "ally.h"
#include "camera.h"
/*----------------------------------------------------------------------*/
/*  GLOBAL CONSTRANTS   */
/*----------------------------------------------------------------------*/
#define PI 3.14159265
/*----------------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*----------------------------------------------------------------------*/
BITMAP  *buf;						        		// global buffer
BITMAP  *bufm, *bufw, *bufs;                		// double buffering
BITMAP	*sfondo, *aereo, *boom, *patriot, *razzo;	// images
int 	crash[MAXA];

pthread_mutex_t mdraw;								// draw mutex
/*----------------------------------------------------------------------*/
/*  FUNCTION DEFINITIONS   */
/*----------------------------------------------------------------------*/
// int get_crash(int x0, int y0) {
// 	int crash, crash_point, c;

// 	crash = 0;
	
// 	crash_point = y0 - 1;
// 	c = getpixel(bufw, x0, crash_point);
// 	if (c != makecol(0, 0, 0) && c != makecol(255, 0, 0))
// 		crash = 1;

// 	return crash;
// }
/*----------------------------------------------------------------------*/
void get_crash(int k) {
	int x0_en, y0_en, x1_en, x0_al, y0_al, x1_al, j;
	// int y1_al, y1_en;
	crash[k] = 0;

	x0_en = enemy_x[k];
	y0_en = enemy_y[k] - aereo->h;
	x1_en = enemy_x[k] + aereo->w;
	// y1_en = y0_en;
	for(j=0; j<MAXA; j++) {
		x0_al = ally_x[j];
		y0_al = ally_y[j];
		x1_al = ally_x[j] + razzo->w;
		// y1_al = y0_al;

		if (x0_al > x0_en && x0_al < x1_en) {
			if (y0_al > y0_en) crash[k] = 1;
		}
		else if (x1_al > x0_en && x1_al < x1_en) {
			if (y0_al > y0_en) crash[k] = 1;
		}
	}
}
/*----------------------------------------------------------------------*/
void load_img(void) {
	buf = create_bitmap(SCREEN_W, SCREEN_H);
	bufm = create_bitmap(XMENU, YMENU);
	bufw = create_bitmap(XWORLD, YWORLD);
	bufs = create_bitmap(XSTATUS, YSTATUS);

    sfondo = load_bitmap("img/sfondo3.bmp", NULL);
	if (sfondo == NULL) {
		printf("file not found\n");
		exit(1);
	}
	aereo = load_bitmap("img/aereo.bmp", NULL);
	if (aereo == NULL) {
		printf("file not found\n");
		exit(1);
	}
	boom = load_bitmap("img/boom.bmp", NULL);
	if (boom == NULL) {
		printf("file not found\n");
		exit(1);
	}
	patriot = load_bitmap("img/patriot.bmp", NULL);
	if (patriot == NULL) {
		printf("file not found\n");
		exit(1);
	}
	razzo = load_bitmap("img/razzo.bmp", NULL);
	if (razzo == NULL) {
		printf("file not found\n");
		exit(1);
	}
}
/*----------------------------------------------------------------------*/
/*  Periodic task for drawing   */
/*----------------------------------------------------------------------*/
void draw(void) {
	int k, view = 0;
	float angle_en, angle_al;

	while (1) {

		// Graphic world
		pthread_mutex_lock(&mdraw);
		clear_to_color(bufw, makecol(0, 0, 0));
		draw_sprite(bufw, sfondo, 0, YWORLD - sfondo->h);
		draw_sprite(bufw, patriot, (XWORLD / 2) - (patriot->w / 2) + BORDER, YWORLD - patriot->h - (BORDER * 2));
		pthread_mutex_unlock(&mdraw);
		
		for (k=0; k<MAXE; k++) {
			if (state[k] == ACTIVE) {
				angle_en = ((atan(en_angle[k]) * 180) / PI);
				if (angle_en < 0) angle_en += 180;
				angle_en = ftofix((256 * angle_en) / 360);
				pthread_mutex_lock(&mdraw);
				rotate_sprite(bufw, aereo, enemy_x[k], enemy_y[k], angle_en);
				get_crash(k);
				pthread_mutex_unlock(&mdraw);
				printf("crash = %d %d %d\n", crash[0], crash[1], crash[2]);
			}
			else if (state[k] == BOOM) {
				pthread_mutex_lock(&mdraw);
				draw_sprite(bufw, boom, enemy_x[k], enemy_y[k]);
				pthread_mutex_unlock(&mdraw);
				if (view >= TEXP) {
					view = 0;
					pthread_mutex_lock(&men);
					state[k] = WAIT;
					n_act--;
					pthread_mutex_unlock(&men);
				}
				else
					view++;
			}
		}

		for(k=0; k<MAXA; k++) {
			if (state_al[k] == ACTIVE) {
				angle_al = ((atan(al_angle[k]) * 180) / PI) + 90;
				if (angle_al > 90) angle_al -= 180;
				angle_al = ftofix((256 * angle_al) / 360);
				pthread_mutex_lock(&mdraw);
				rotate_sprite(bufw, razzo, ally_x[k], ally_y[k], angle_al);
				pthread_mutex_unlock(&mdraw);
			}
			else if (state_al[k] == BOOM) {
				pthread_mutex_lock(&mdraw);
				draw_sprite(bufw, boom, ally_x[k], ally_y[k]);
				pthread_mutex_unlock(&mdraw);
				if (view >= TEXP) {
					view = 0;
					pthread_mutex_lock(&mal);
					state_al[k] = WAIT;
					n_al_act--;
					pthread_mutex_unlock(&mal);
				}
				else
					view++;
			}
		}

		pthread_mutex_lock(&mdraw);
		rect(bufw, camera_x, camera_y + HRES, camera_x + VRES, camera_y, makecol(255, 0, 0));
		line(bufw, line_x1, line_y1, line_x2, line_y2, makecol(255, 0, 0));
		pthread_mutex_unlock(&mdraw);

		// Menu area
		clear_to_color(bufm, makecol(0, 0, 0));
		textout_ex(bufm, font, "PATRIOT MISSILE LAUNCHER DEFENSE", (XMENU / 3) + BORDER, BORDER, makecol(255, 255, 255), -1);

		// Status window
		clear_to_color(bufs, makecol(0, 0, 0));
		textprintf_ex(bufs, font, BORDER, BORDER, makecol(255, 255, 255), -1, "ACTIVE TASK: %d", n_act);

		clear_to_color(buf, makecol(0, 0, 0));
		blit(bufm, buf, 0, 0, BORDER + 1, BORDER + 1, bufm->w, bufm->h);
		blit(bufw, buf, 0, 0, BORDER + 1, YMENU + (BORDER * 2) + 3, bufw->w, bufw->h);
		blit(bufs, buf, 0, 0, XMENU + (BORDER * 2) + 3, BORDER + 1, bufs->w, bufs->h);

		rect(buf, BORDER, SCREEN_H - BORDER, XWORLD + BORDER + 1, YMENU + (BORDER * 2) + 2, makecol(0, 0, 255));
		rect(buf, BORDER, YMENU + BORDER + 1, XMENU + BORDER + 1, BORDER, makecol(255, 255, 24));
		rect(buf, XWORLD + (BORDER * 2) + 2, SCREEN_H - BORDER, SCREEN_W - BORDER, BORDER, makecol(128, 0, 128));

		blit(buf, screen, 0, 0, 0, 0, buf->w, buf->h);

		ptask_wait_for_period();
	}
}
/*----------------------------------------------------------------------*/
