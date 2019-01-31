/*--------------------------------------------------------------*/
//  COMMANDS.H - header file of commands.c
/*--------------------------------------------------------------*/
#if !defined COMMANDS_H
#define COMMANDS_H

/*--------------------------------------------------------------*/
/*  FUNCTION PROTOTYPES   */
/*--------------------------------------------------------------*/
extern void commands(void);
extern void init_command(void);
extern void check_deadline_miss_com(void);

/*--------------------------------------------------------------*/
/*  GLOBAL VARIABLES   */
/*--------------------------------------------------------------*/
extern pthread_mutex_t mcom;                    // mutex for command global variables
extern int  command_deadline;                   // number of missed command deadlines

#endif // COMMANDS_H

/*--------------------------------------------------------------*/
