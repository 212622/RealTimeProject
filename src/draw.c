/*----------------------------------------------------------------------*/
//	DRAW.C deals with screen managing, image printing and
//	collisions identification
/*----------------------------------------------------------------------*/
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "ptask.h"
#include "draw.h"
#include "init.h"
#include "enemy.h"
#include "ally.h"
#include "camera.h"
#include "commands.h"

/*----------------------------------------------------------------------*/
/*  MATH CONSTANTS   */
/*----------------------------------------------------------------------*/
#define PI		3.14159265							// PI greek
#define FLAT	180									// flat angle
#define LAP		360									// lap angle
#define	RIGHT	90									// right angle
/*----------------------------------------------------------------------*/
/*  DRAW CONSTANTS   */
/*----------------------------------------------------------------------*/
#define MAXR	256									// maximum sprite rotation

/*----------------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*----------------------------------------------------------------------*/
pthread_mutex_t mdraw;								// mutex for draw global variables
BITMAP	*bufw;										// sub-window buffer for graphic world
BITMAP	*background, *plane, *rocket;				// images
static BITMAP	*buf;						        // global buffer for double buffering
static BITMAP	*bufm, *bufs;                		// sub-windows buffer for menu and status area
static BITMAP	*boom, *patriot;					// images
static int		draw_deadline;						// flag for draw line and camera
static int		view;								// temporary variable for view explosion
static float	defense_per, attack_per;			// rate of destroyed and non-destroyed enemies

/*----------------------------------------------------------------------*/
//	GET_CRASH_EN: identifies crashing enemies.
/*----------------------------------------------------------------------*/
void get_crash_en(int k) {
	int i;											// temporary variable
	int en_left, en_right, en_up, en_down;			// enemy edges
	int al_left, al_right, al_up, al_down;			// ally edges

	en_left = enemy_x[k];
	en_right = enemy_x[k] + plane->w;
	en_up = enemy_y[k];
	en_down = enemy_y[k] + plane->h;

	for (i=0; i<MAXA; i++) {
		if (state_al[i] == ACTIVE) {
			al_left = ally_x[i];
			al_right = ally_x[i] + rocket->w;
			al_up = ally_y[i];
			al_down = ally_y[i] + rocket->h;

			if (((al_left > en_left && al_left < en_right) ||(al_right > en_left && al_right < en_right)) &&
				((al_up < en_down && al_up > en_up) || (al_down < en_down && al_up > en_up))) {
				pthread_mutex_lock(&mdraw);
				crash_en[k] = 1;
				pthread_mutex_unlock(&mdraw);
			}
		}
	}
}

/*----------------------------------------------------------------------*/
//	GET_CRASH_AL: identifies crashing allies.
/*----------------------------------------------------------------------*/
void get_crash_al(int k) {
	int i;											// temporary variable
	int en_left, en_right, en_up, en_down;			// enemy edges
	int al_left, al_right, al_up, al_down;			// ally edges

	al_left = ally_x[k];
	al_right = ally_x[k] + rocket->w;
	al_up = ally_y[k];
	al_down = ally_y[k] + rocket->h;
	
	for (i=0; i<MAXE; i++) {
		if (state_en[i] == ACTIVE) {
			en_left = enemy_x[i];
			en_right = enemy_x[i] + plane->w;
			en_up = enemy_y[i];
			en_down = enemy_y[i] + plane->h;

			if (((al_left > en_left && al_left < en_right) || (al_right > en_left && al_right < en_right)) &&
				((al_up < en_down && al_up > en_up) || (al_down < en_down && al_up > en_up))) {
				pthread_mutex_lock(&mdraw);
				crash_al[k] = 1;
				pthread_mutex_unlock(&mdraw);
			}
		}
	}
}

/*----------------------------------------------------------------------*/
//	LOAD_IMAGE: creates buffers and a bitmap for each image.
/*----------------------------------------------------------------------*/
void load_img(void) {
	buf = create_bitmap(SCREEN_W, SCREEN_H);		// global buffer
	bufm = create_bitmap(XMENU, YMENU);				// menu area buffer
	bufw = create_bitmap(XWORLD, YWORLD);			// graphic world buffer
	bufs = create_bitmap(XSTATUS, YSTATUS);			// status window buffer

    background = load_bitmap("img/background.bmp", NULL);
	if (background == NULL) {
		printf("ERROR: file not found\n");
		exit(1);
	}
	plane = load_bitmap("img/plane.bmp", NULL);
	if (plane == NULL) {
		printf("ERROR: file not found\n");
		exit(1);
	}
	boom = load_bitmap("img/boom.bmp", NULL);
	if (boom == NULL) {
		printf("ERROR: file not found\n");
		exit(1);
	}
	patriot = load_bitmap("img/patriot.bmp", NULL);
	if (patriot == NULL) {
		printf("ERROR: file not found\n");
		exit(1);
	}
	rocket = load_bitmap("img/rocket.bmp", NULL);
	if (rocket == NULL) {
		printf("ERROR: file not found\n");
		exit(1);
	}
}

/*----------------------------------------------------------------------*/
//	INIT_DRAW: initializes draw parameters.
/*----------------------------------------------------------------------*/
void init_draw(void) {
	int k;									// temporary variable

	view = 0;
	defense_per = 100;
	attack_per = 0;
	draw_deadline = 0;

	pthread_mutex_lock(&men);
	en_deadline = 0;
	for (k=0; k<MAXE; k++) crash_en[k] = 0;
	pthread_mutex_unlock(&men);

	pthread_mutex_lock(&mal);
	al_deadline = 0;
	for (k=0; k<MAXA; k++) crash_al[k] = 0;
	pthread_mutex_unlock(&mal);

	pthread_mutex_lock(&mcam);
	cam_deadline = 0;
	pthread_mutex_unlock(&mcam);
	
	pthread_mutex_lock(&mcom);
	command_deadline = 0;
	pthread_mutex_unlock(&mcom);
}

/*----------------------------------------------------------------------*/
//	PRINT_WORLD: prints static parameters in graphic world buffer.
/*----------------------------------------------------------------------*/
void print_graphic_world(void) {
	// print background
	pthread_mutex_lock(&mdraw);
	clear_to_color(bufw, makecol(0, 0, 0));
	draw_sprite(bufw, background, 0, YWORLD - background->h);
	draw_sprite(bufw, patriot, (XWORLD / 2) - (patriot->w / 2) + BORDER, YWORLD - patriot->h - (BORDER * 2));
	pthread_mutex_unlock(&mdraw);

	draw_enemy();
	draw_ally();

	view_cam_line();
}

/*----------------------------------------------------------------------*/
//	PRINT_MENU_AREA: prints menu area parameters, title and border.
/*----------------------------------------------------------------------*/
void print_menu_area(void) {
	clear_to_color(bufm, makecol(0, 0, 0));

	textout_ex(bufm, font, "-------------------------------------", (XMENU / 3), 
				BORDER, makecol(255, 255, 255), -1);
	textout_ex(bufm, font, "PATRIOT MISSILE LAUNCHER DEFENSE V1.0", (XMENU / 3),
				 2 * BORDER, makecol(255, 255, 255), -1);
	textout_ex(bufm, font, "-------------------------------------", (XMENU / 3),
				 3 * BORDER, makecol(255, 255, 255), -1);
	textout_ex(bufm, font, "Press SPACE for create enemy", (XMENU / 3) + (3 * BORDER),
				 4.5 * BORDER, makecol(255, 255, 255), -1);
	textout_ex(bufm, font, "Press Q for camera and line", (XMENU / 3) + (3 * BORDER),
				 5.5 * BORDER, makecol(255, 255, 255), -1);
	textout_ex(bufm, font, "Press ESC for quit", (XMENU / 3) + (3 * BORDER),
				 6.5 * BORDER, makecol(255, 255, 255), -1);
}

/*----------------------------------------------------------------------*/
//	PRINT_STATUS_WINDOW: prints statistics and credits.
/*----------------------------------------------------------------------*/
void print_status_window(void) {
	if (en_arrived + en_died != 0) {
		defense_per = (en_died / (en_died + en_arrived)) * 100;
		attack_per = (en_arrived / (en_died + en_arrived)) * 100;
	}

	clear_to_color(bufs, makecol(0, 0, 0));

	textout_ex(bufs, font, "    STATUS WINDOW    ", BORDER, 5 * BORDER, makecol(255, 255, 255), -1);
	textout_ex(bufs, font, "---------------------", BORDER, 10 * BORDER, makecol(255, 255, 255), -1);
	textprintf_ex(bufs, font, BORDER, 12 * BORDER, 
					makecol(255, 255, 255), -1, "ACTIVE ENEMY    : %d", n_en_act);
	textprintf_ex(bufs, font, BORDER, 14 * BORDER,
					makecol(255, 255, 255), -1, "TOTAL ENEMY     : %.0f", en_tot);
	textprintf_ex(bufs, font, BORDER, 16 * BORDER,
					makecol(255, 255, 255), -1, "ENEMY DESTROYED : %.0f", en_died);
	textprintf_ex(bufs, font, BORDER, 18 * BORDER, 
					makecol(255, 255, 255), -1, "ENEMY ARRIVED   : %.0f", en_arrived);
	textout_ex(bufs, font, "---------------------", BORDER, 20 * BORDER, makecol(255, 255, 255), -1);
	textprintf_ex(bufs, font, BORDER, 22 * BORDER, 
					makecol(255, 255, 255), -1, "DEFENSE         : %.0f%%", defense_per);
	textprintf_ex(bufs, font, BORDER, 24 * BORDER, 
					makecol(255, 255, 255), -1, "ATTACK          : %.0f%%", attack_per);
	textout_ex(bufs, font, "---------------------", BORDER, 26 * BORDER, makecol(255, 255, 255), -1);
	textprintf_ex(bufs, font, BORDER, 28 * BORDER, 
					makecol(255, 255, 255), -1, "EN  DEADLINE MISS: %d", en_deadline);
	textprintf_ex(bufs, font, BORDER, 30 * BORDER, 
					makecol(255, 255, 255), -1, "AL  DEADLINE MISS: %d", al_deadline);
	textprintf_ex(bufs, font, BORDER, 32 * BORDER, 
					makecol(255, 255, 255), -1, "CAM DEADLINE MISS: %d", cam_deadline);
	textprintf_ex(bufs, font, BORDER, 34 * BORDER, 
					makecol(255, 255, 255), -1, "GRA DEADLINE MISS: %d", draw_deadline);
	textprintf_ex(bufs, font, BORDER, 36 * BORDER, 
					makecol(255, 255, 255), -1, "COM DEADLINE MISS: %d", command_deadline);
	textout_ex(bufs, font, "---------------------", BORDER, 38 * BORDER, makecol(255, 255, 255), -1);
	textout_ex(bufs, font, "#####################", BORDER, 56 * BORDER, makecol(255, 255, 255), -1);
	textout_ex(bufs, font, "     CREATED BY      ", BORDER, 60 * BORDER, makecol(255, 255, 255), -1);
	textout_ex(bufs, font, "  TORRICELLI TOMAS   ", BORDER, 64 * BORDER, makecol(255, 255, 255), -1);
	textout_ex(bufs, font, "   CUOGHI LORENZO    ", BORDER, 66 * BORDER, makecol(255, 255, 255), -1);
	textout_ex(bufs, font, "#####################", BORDER, 70 * BORDER, makecol(255, 255, 255), -1);
}

/*----------------------------------------------------------------------*/
//	PRINT_SCREEN: prints everything to the screen from the buffers.
/*----------------------------------------------------------------------*/
void print_screen(void) {
	clear_to_color(buf, makecol(0, 0, 0));

	// print sub-window buffers into global buffer
	blit(bufm, buf, 0, 0, BORDER + 1, BORDER + 1, bufm->w, bufm->h);
	blit(bufw, buf, 0, 0, BORDER + 1, YMENU + (BORDER * 2) + 3, bufw->w, bufw->h);
	blit(bufs, buf, 0, 0, XMENU + (BORDER * 2) + 3, BORDER + 1, bufs->w, bufs->h);

	// print borders of windows into global buffer
	rect(buf, BORDER, SCREEN_H - BORDER, XWORLD + BORDER + 1, YMENU + (BORDER * 2) + 2, makecol(0, 0, 255));
	rect(buf, BORDER, YMENU + BORDER + 1, XMENU + BORDER + 1, BORDER, makecol(255, 255, 24));
	rect(buf, XWORLD + (BORDER * 2) + 2, SCREEN_H - BORDER, SCREEN_W - BORDER, BORDER, makecol(128, 0, 128));

	// print global buffer on screen
	blit(buf, screen, 0, 0, 0, 0, buf->w, buf->h);
}

/*----------------------------------------------------------------------*/
//	DRAW_ENEMY: computes movement and state of enemies.
/*----------------------------------------------------------------------*/
void draw_enemy(void) {
	int		k;					// temporary variable
	float	angle_en = 0;		// gradient of enemy to be drawn
	
	for (k=0; k<MAXE; k++) {
		if (state_en[k] == ACTIVE) {
			angle_en = ((atan(en_angle[k]) * FLAT) / PI);
			if (angle_en < 0) angle_en += FLAT;
			angle_en = ftofix((MAXR * angle_en) / LAP);
			pthread_mutex_lock(&mdraw);
			rotate_sprite(bufw, plane, enemy_x[k], enemy_y[k], angle_en);
			pthread_mutex_unlock(&mdraw);
			get_crash_en(k);
		}
		else if (state_en[k] == BOOM) {
			pthread_mutex_lock(&mdraw);
			draw_sprite(bufw, boom, enemy_x[k], enemy_y[k]);
			pthread_mutex_unlock(&mdraw);
			if (view >= TEXP) {
				view = 0;
				pthread_mutex_lock(&men);
				state_en[k] = WAIT;
				n_en_act--;
				pthread_mutex_unlock(&men);
			}
			else
				view++;
		}
	}
}

/*----------------------------------------------------------------------*/
//	DRAW_ALLY: computes movement and state of allies.
/*----------------------------------------------------------------------*/
void draw_ally(void) {
	int 	k;					// temporary variable
	float 	angle_al = 0;		// gradient of ally to be drawn

	for(k=0; k<MAXA; k++) {
		if (state_al[k] == ACTIVE) {
			angle_al = ((atan(al_angle[k]) * FLAT) / PI) + RIGHT;
			if (angle_al > RIGHT) angle_al -= FLAT;
			angle_al = ftofix((MAXR * angle_al) / LAP);
			pthread_mutex_lock(&mdraw);
			rotate_sprite(bufw, rocket, ally_x[k], ally_y[k], angle_al);
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
}

/*----------------------------------------------------------------------*/
//	VIEW_CAM_LINE: visualizes camera and trajectory lines
//	if Q key is pressed.
/*----------------------------------------------------------------------*/
void view_cam_line(void) {
	if (cam_line_view == 1) {	
		pthread_mutex_lock(&mdraw);
		rect(bufw, camera_x, camera_y + HRES, camera_x + VRES, camera_y, makecol(255, 0, 0));
		line(bufw, line_x1, line_y1, line_x2, line_y2, makecol(255, 0, 0));
		pthread_mutex_unlock(&mdraw);
	}
}

/*----------------------------------------------------------------------*/
//	CHECK_DEADLINE_MISS_DRAW: counts the number of deadline miss in ally.
/*----------------------------------------------------------------------*/
void check_deadline_miss_draw(void) {
	if (ptask_deadline_miss()) {
		pthread_mutex_lock(&mdraw);
		draw_deadline++;
		pthread_mutex_unlock(&mdraw);
	}
}

/*----------------------------------------------------------------------*/
//	DRAW: periodic task for drawing.
/*----------------------------------------------------------------------*/
void draw(void) {
	init_draw();

	while (1) {
		print_graphic_world();

		print_menu_area();

		print_status_window();
		
		print_screen();

		check_deadline_miss_draw();
		ptask_wait_for_period();
	}
}

/*----------------------------------------------------------------------*/
