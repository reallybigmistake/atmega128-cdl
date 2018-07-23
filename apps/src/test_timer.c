#include "test_timer.h"
#if (CONFIG_TIMER_TEST == 1)
/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
#define TIMER_REG_LEN_IN_DWORD 10

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
static int cmd_help(int argc, char *argv[])
{	
	return cmdline_help_general(&timer_test_menu[0]);
}
static int timer_start_stop(int argc, char *argv[])
{
	info("timer_start_stop");
    return 0;
}
int timer_test_cleanup(int sel)
{
	info("timer_clean");
    return 0;
}
//*****************************************************************************
//
// This is the table that holds the command names, implementing functions,
// and brief description.
//
//*****************************************************************************
cmdline_entry timer_test_menu[] = {
	{"help", 				cmd_help,  			" : Display list of commands"},
	{"h",					cmd_help,			"    : alias for help"},
	{"?",					cmd_help, 			"    : alias for help"},
	{"timer_start_stop",	timer_start_stop,	": timer start/stop test"},	
	{0, 					0, 					0}
};

int timer_test_prepare(int sel)
{
    info("timer_test_prepare");
	return 0;
}

#endif

