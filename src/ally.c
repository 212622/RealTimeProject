/*----------------------------------------------------------------------*/
//	ALLY.C describes the behavior of allied rockets and their attributes.
/*----------------------------------------------------------------------*/
#include <pthread.h>
#include "ptask.h"
#include "ally.h"
#include "init.h"
#include "draw.h"
#include "camera.h"
#include "enemy.h"

/*----------------------------------------------------------------------*/
//  SPEED CONSTANT: speed conversion coefficient between 20 and 23.xx
//	from pixel/sec to pixel based motion value.
/*----------------------------------------------------------------------*/
#define CONVERSION	23.25

/*----------------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*----------------------------------------------------------------------*/
pthread_mutex_t mal;						// mutex for ally global variables
int		ally_x[MAXA], ally_y[MAXA];			// coordinates of allies
int 	state_al[MAXA];						// ally states
int		tid_al[MAXA];						// ally task IDs
int		n_al_act;		                    // number of active ally tasks
float   al_angle[MAXA];                     // ally rotation angles
int     al_deadline;						// number of missed ally deadlines
int 	crash_al[MAXA];						// defines who is crashing

/*----------------------------------------------------------------------*/
//	WRITE_AL_ATT: writes allies attributes in global variables.
/*----------------------------------------------------------------------*/
void write_al_att(float x1, float y1, float m, int tid) {
	pthread_mutex_lock(&mal);
	ally_x[tid] = x1;
	ally_y[tid] = y1;
	al_angle[tid] = m;
	pthread_mutex_unlock(&mal);
}

/*----------------------------------------------------------------------*/
//	CHECK_DEADLINE_MISS_AL: counts the number of deadline miss in ally.
/*----------------------------------------------------------------------*/
void check_deadline_miss_al(void) {
    if (ptask_deadline_miss()) {
		pthread_mutex_lock(&mal);
		al_deadline++;
		pthread_mutex_unlock(&mal);
	}
}

/*----------------------------------------------------------------------*/
//	ALLY: periodic task for ally.
/*----------------------------------------------------------------------*/
void ally(void) {
	int tid = ptask_get_index() - (MAXE + 1);	// task array index
	float m, x1, x2, y1, y2, speed;				// ally position and speed variables

	x1 = (XWORLD / 2) - (rocket->w / 2) + (BORDER * 3);
	y1 = YWORLD - background->h - rocket->h;
	y2 = YWORLD / 2;

	ptask_wait_for_activation();
	x2 = (((y2 - line_y1) / (line_y2 - line_y1)) * (line_x2 - line_x1)) + line_x1;
	m = (y2 - y1) / (x2 - x1);
	speed =  ((y1 - y2) / ((y2 - line_y1) / en_speed)) * CONVERSION;
	while(1) {
		if (y1 > HRES + 1 && crash_al[tid] == 0) {
			write_al_att(x1, y1, m , tid);
			
			x1 += (-speed / (y2 - y1)) * (x2 - x1);
			y1 -= speed;
		}
		else {
			pthread_mutex_lock(&mal);
			state_al[tid] = BOOM;
			pthread_mutex_unlock(&mal);

			pthread_mutex_lock(&mal);
			crash_al[tid] = 0;
			pthread_mutex_unlock(&mal);

			ptask_wait_for_activation();
			x1 = (XWORLD / 2) - (rocket->w / 2) + (BORDER * 3);
			y1 = YWORLD - background->h - rocket->h;
			y2 = YWORLD / 2;
			x2 = (((y2 - line_y1) / (line_y2 - line_y1)) * (line_x2 - line_x1)) + line_x1;
			m = (y2 - y1) / (x2 - x1);
			write_al_att(x1, y1, m, tid);
		}

		check_deadline_miss_al();
		ptask_wait_for_period();
	}
}

/*----------------------------------------------------------------------*/
