/*--------------------------------------------------------------*/
/*  HEADER FILES        */
/*--------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <allegro.h>
#include "ptask.h"
/*--------------------------------------------------------------*/
/*  GLOBAL CONSTRANTS   */
/*--------------------------------------------------------------*/
#define XWIN    1024                        // window x resolution
#define YWIN    768                         // window y resolution
#define XCAM    200                         // camera y resolution
#define YCAM    200                         // camera y resolution
#define MAXT    5                           // max number of tasks
#define PER     30                          // base period
#define PRIO    50                          // task priority
/*--------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*--------------------------------------------------------------*/
BITMAP  *buf, *aereo, *boom;                // double buffering 
int tid[MAXT], mti, i;                      // tasks indexes
/*--------------------------------------------------------------*/
