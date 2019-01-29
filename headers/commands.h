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
extern int  cam_line_view;
extern int  command_deadline;

extern pthread_mutex_t mcom;                         // command mutex
/*--------------------------------------------------------------*/

#endif // COMMANDS_H
