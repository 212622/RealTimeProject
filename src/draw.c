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
int 	crash_al[MAXA];
int 	crash_en[MAXE];

pthread_mutex_t mdraw;								// draw mutex
/*----------------------------------------------------------------------*/
/*  FUNCTION DEFINITIONS   */
/*----------------------------------------------------------------------*/
// void get_crash_en(int k) {
// 	int i;
// 	int x0_en, x1_en, y_en;
// 	int x0_al, x1_al, y_al;

// 	x0_en = enemy_x[k];
// 	x1_en = enemy_x[k] + aereo->w;
// 	y_en = enemy_y[k] + aereo->h;

// 	for(i=0; i<MAXA; i++) {
// 		if (state_al[i] == ACTIVE) {
// 			x0_al = ally_x[i];
// 			x1_al = ally_x[i] + razzo->w;
// 			y_al = ally_y[i];

// 			if ((x0_al > x0_en && x0_al < x1_en) || (x1_al > x0_en && x1_al < x1_en)) {
// 				if (y_al < y_en && y_al > enemy_y[k]) {
// 					pthread_mutex_lock(&mdraw);
// 					crash_en[k] = 1;
// 					pthread_mutex_unlock(&mdraw);
// 				}
// 			}
// 		}
// 	}
// }

void get_crash_en(int k) {
	int i;
	int en_left, en_right, en_up, en_down;
	int al_left, al_right, al_up, al_down;

	en_left = enemy_x[k];
	en_right = enemy_x[k] + aereo->w;
	en_up = enemy_y[k];
	en_down = enemy_y[k] + aereo->h;

	for(i=0; i<MAXA; i++) {
		if (state_al[i] == ACTIVE) {
			al_left = ally_x[i];
			al_right = ally_x[i] + razzo->w;
			al_up = ally_y[i];
			al_down = ally_y[i] + razzo->h;

			if (((al_left > en_left && al_left < en_right) || (al_right > en_left && al_right < en_right)) &&
				((al_up < en_down && al_up > en_up) || (al_down < en_down && al_up > en_up))) {
				pthread_mutex_lock(&mdraw);
				crash_en[k] = 1;
				pthread_mutex_unlock(&mdraw);
			}
		}
	}
}
/*----------------------------------------------------------------------*/
void get_crash_al(int k) {
	int i;
	int en_left, en_right, en_up, en_down;
	int al_left, al_right, al_up, al_down;

	al_left = ally_x[k];
	al_right = ally_x[k] + razzo->w;
	al_up = ally_y[k];
	al_down = ally_y[k] + razzo->h;
	
	for(i=0; i<MAXE; i++) {
		if (state[i] == ACTIVE) {
			en_left = enemy_x[i];
			en_right = enemy_x[i] + aereo->w;
			en_up = enemy_y[i];
			en_down = enemy_y[i] + aereo->h;

			if (((al_left > en_left && al_left < en_right) || (al_right > en_left && al_right < en_right)) &&
				((al_up < en_down && al_up > en_up) || (al_down < en_down && al_up > en_up))) {
				pthread_mutex_lock(&mdraw);
				crash_al[k] = 1;
				pthread_mutex_unlock(&mdraw);
			}
		}
	}
}

// void get_crash_al(int k) {
// 	int i;
// 	int x0_en, x1_en, y_en;
// 	int x0_al, x1_al, y_al;

// 	x0_al = ally_x[k];
// 	x1_al = ally_x[k] + razzo->w;
// 	y_al = ally_y[k];
	
// 	for(i=0; i<MAXE; i++) {
// 		if (state[i] == ACTIVE) {
// 			x0_en = enemy_x[i];
// 			x1_en = enemy_x[i] + aereo->w;
// 			y_en = enemy_y[i] + aereo->h;

// 			if ((x0_al > x0_en && x0_al < x1_en) || (x1_al > x0_en && x1_al < x1_en)) {
// 				if (y_al < y_en && y_al > enemy_y[k]) {
// 					pthread_mutex_lock(&mdraw);
// 					crash_al[k] = 1;
// 					pthread_mutex_unlock(&mdraw);
// 				}
// 			}
// 		}
// 	}
// }
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
	float angle_en, angle_al, defense_per = 0, attack_per = 0;

	pthread_mutex_lock(&mdraw);
	for (k=0; k<MAXE; k++)
		crash_en[k] = 0;
	for (k=0; k<MAXA; k++)
		crash_al[k] = 0;
	pthread_mutex_unlock(&mdraw);

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
				pthread_mutex_unlock(&mdraw);
				get_crash_en(k);
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
				get_crash_al(k);
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
		
		if ((en_arrived + en_died) != 0) {
			defense_per = (en_died / (en_died + en_arrived)) * 100;
			attack_per = (en_arrived / (en_died + en_arrived)) * 100;
		}
		
		// camera e linea traiettoria visibili
		// pthread_mutex_lock(&mdraw);
		// rect(bufw, camera_x, camera_y + HRES, camera_x + VRES, camera_y, makecol(255, 0, 0));
		// line(bufw, line_x1, line_y1, line_x2, line_y2, makecol(255, 0, 0));
		// pthread_mutex_unlock(&mdraw);

		// Menu area
		clear_to_color(bufm, makecol(0, 0, 0));
		textout_ex(bufm, font, "PATRIOT MISSILE LAUNCHER DEFENSE V1.0", (XMENU / 3), BORDER, makecol(255, 255, 255), -1);
		textout_ex(bufm, font, "Press SPACE for create enemy", (XMENU / 3) + (2 * BORDER), 4 * BORDER, makecol(255, 255, 255), -1);

		// Status window
		clear_to_color(bufs, makecol(0, 0, 0));
		textout_ex(bufs, font, "STATUS WINDOW", BORDER, 4 * BORDER, makecol(255, 255, 255), -1);
		textout_ex(bufs, font, "---------------------", BORDER, 8 * BORDER, makecol(255, 255, 255), -1);
		textprintf_ex(bufs, font, BORDER, 10 * BORDER, makecol(255, 255, 255), -1, "ACTIVE TASKS    : %d", n_act);
		textprintf_ex(bufs, font, BORDER, 12 * BORDER, makecol(255, 255, 255), -1, "TOTAL ENEMY     : %.0f", en_tot);
		textprintf_ex(bufs, font, BORDER, 14 * BORDER, makecol(255, 255, 255), -1, "ENEMY DESTROYED : %.0f", en_died);
		textprintf_ex(bufs, font, BORDER, 16 * BORDER, makecol(255, 255, 255), -1, "ENEMY ARRIVED   : %.0f", en_arrived);
		textout_ex(bufs, font, "---------------------", BORDER, 18 * BORDER, makecol(255, 255, 255), -1);
		textprintf_ex(bufs, font, BORDER, 20 * BORDER, makecol(255, 255, 255), -1, "DEFNESE         : %.0f%%", defense_per);
		textprintf_ex(bufs, font, BORDER, 22 * BORDER, makecol(255, 255, 255), -1, "ATTACK          : %.0f%%", attack_per);
		
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
