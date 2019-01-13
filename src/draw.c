/*----------------------------------------------------------------------*/
/*  HEADER FILES        */
/*----------------------------------------------------------------------*/
#include <math.h>
#include <pthread.h>
#include "ptask.h"
#include "init.h"
#include "draw.h"
#include "enemy.h"
#include "camera.h"
/*----------------------------------------------------------------------*/
/*  GLOBAL CONSTRANTS   */
/*----------------------------------------------------------------------*/
#define PI 3.14159265
/*----------------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*----------------------------------------------------------------------*/
BITMAP  *buf;						        // global buffer
BITMAP  *bufm, *bufw, *bufs;                // double buffering
BITMAP	*sfondo, *aereo, *boom, *patriot;	// images

pthread_mutex_t mdraw;						// draw mutex
/*----------------------------------------------------------------------*/
/*  FUNCTION DEFINITIONS   */
/*----------------------------------------------------------------------*/
int get_crash(int x0, int y0) {
	int crash, crash_point, c;

	crash = 0;
	
	crash_point = y0 - 1;
	c = getpixel(bufw, x0, crash_point);
	if (c != makecol(0, 0, 0) && c != makecol(255, 0, 0))
		crash = 1;

	return crash;
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
}
/*----------------------------------------------------------------------*/
/*  Periodic task for drawing   */
/*----------------------------------------------------------------------*/
void draw(void) {
	int k, view = 0;
	float m, rad, gr360, gr256, angle;
	// float correction;

	while (1) {

		// Graphic world
		pthread_mutex_lock(&mdraw);
		clear_to_color(bufw, makecol(0, 0, 0));
		draw_sprite(bufw, sfondo, 0, YWORLD - sfondo->h);
		draw_sprite(bufw, patriot, (XWORLD / 2) - (patriot->w / 2) + 10, YWORLD - patriot->h - 20);
		pthread_mutex_unlock(&mdraw);

		for (k=0; k<MAXE; k++) {
			if (state[k] == ACTIVE) {
				m = en_angle[k];
				rad = atan(m);
				gr360 = (rad * 180) / PI;
				// correction = gr360 - 90;
				if (gr360 < 0) gr360 += 180;
				gr256 = (256 * gr360) / 360;
				angle = ftofix(gr256);
				// printf("m = %f, rad = %f, gr360 = %f, gr256 = %f, angle = %f\n", m, rad, gr360, gr256, angle);
				pthread_mutex_lock(&mdraw);
				rotate_sprite(bufw, aereo, enemy_x[k], enemy_y[k], angle);
				pthread_mutex_unlock(&mdraw);
			}
			else if (state[k] == BOOM) {
				draw_sprite(bufw, boom, enemy_x[k], enemy_y[k]);
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

		pthread_mutex_lock(&mdraw);
		rect(bufw, camera_x, camera_y + HRES, camera_x + VRES, camera_y, makecol(255, 0, 0));
		// rect(bufw, 0, YWORLD - 1, XWORLD - 1, 0, makecol(0, 0, 255));
		line(bufw, line_x1, line_y1, line_x2, line_y2, makecol(255, 0, 0));
		pthread_mutex_unlock(&mdraw);

		// Menu area
		clear_to_color(bufm, makecol(0, 0, 0));
		// rect(bufm, 0, YMENU - 1, XMENU - 1, 0, makecol(255, 255, 24));

		// Status window
		clear_to_color(bufs, makecol(0, 0, 0));
		// rect(bufs, 0, YSTATUS - 1, XSTATUS - 1, 0, makecol(128, 0, 128));

		clear_to_color(buf, makecol(0, 0, 0));
		// blit(bufm, buf, 0, 0, BORDER, BORDER, bufm->w, bufm->h);
		blit(bufm, buf, 0, 0, BORDER + 1, BORDER + 1, bufm->w, bufm->h);
		// blit(bufw, buf, 0, 0, BORDER, YMENU + (BORDER * 2), bufw->w, bufw->h);
		blit(bufw, buf, 0, 0, BORDER + 1, YMENU + (BORDER * 2) + 3, bufw->w, bufw->h);
		// blit(bufs, buf, 0, 0, XMENU + (BORDER * 2), BORDER, bufs->w, bufs->h);
		blit(bufs, buf, 0, 0, XMENU + (BORDER * 2) + 3, BORDER + 1, bufs->w, bufs->h);

		rect(buf, BORDER, SCREEN_H - BORDER, XWORLD + BORDER + 1, YMENU + (BORDER * 2) + 2, makecol(0, 0, 255));
		rect(buf, BORDER, YMENU + BORDER + 1, XMENU + BORDER + 1, BORDER, makecol(255, 255, 24));
		rect(buf, XWORLD + (BORDER * 2) + 2, SCREEN_H - BORDER, SCREEN_W - BORDER, BORDER, makecol(128, 0, 128));

		blit(buf, screen, 0, 0, 0, 0, buf->w, buf->h);

		ptask_wait_for_period();
	}
}
/*----------------------------------------------------------------------*/
