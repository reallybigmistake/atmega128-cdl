#ifndef __CMD_COMMON_H__
#define __CMD_COMMON_H__

#include <stdarg.h>
#include <string.h>

/* command line process */
#define CMDLINE_BAD_CMD 				(-1)
#define CMDLINE_TOO_MANY_ARGS 	(-2)
#define CMDLINE_TOO_FEW_ARGS	 (-3)
#define CMDLINE_INVALID_ARG 	(-4)
#define CMDLINE_QUIT					(-5)

typedef int ( *pfn_cmdline )( int argc, char* argv[] );

typedef struct
{
	const char* cmd;
	pfn_cmdline	pfn_cmd;
	const char* help;
} cmdline_entry;

int cmdline_process( char* cmd, cmdline_entry* entry );
int cmdline_help_general( cmdline_entry* entry );

int cmdline_to_argv( char* cmd,int* argc,char** argv);
extern unsigned long get_arg_ulong(const char* buf);
extern unsigned int get_arg_uint(const char* buf);
#endif
