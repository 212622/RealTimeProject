/*--------------------------------------------------------------*/
/*HEADER FILES*/
/*--------------------------------------------------------------*/
#include <allegro.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include "ptask.h"
/*--------------------------------------------------------------*/
/*GLOBAL DATA*/
/*--------------------------------------------------------------*/
#define XWIN    1024                        // window x resolution
#define YWIN    768                         // window y resolution
#define XBASE   40                          // X start for the message
#define YBASE   50                          // Y level for the first task
#define YINC    30                          // Y increment for the other tasks
#define BKG     0                           // background color
//-------------------------------------------------------------
#define MAXT    5                           // max number of tasks
#define LEN     80                          // max message length
#define PER     30                          // base period
#define PINC    20                          // period increment
#define PRIO    50                          // priorità 
//-------------------------------------------------------------
// GLOBAL VARIABLES
//-------------------------------------------------------------
int     end = 0;                            // end flag
char    mes[MAXT][LEN+1];                   // buf for MAXT mes of length LEN
float 	speed;	                            //speed
BITMAP  *buf;

