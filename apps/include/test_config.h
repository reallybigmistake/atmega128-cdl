#ifndef __TEST_CONFIG_H__
#define __TEST_CONFIG_H__

#include "test_types.h"
#include "cmd_common.h"
#include "console.h"
#include "strto.h"

#define DEBUG_OUTPUT

#ifdef 	DEBUG_OUTPUT
#define debug(...) console_printf (__VA_ARGS__)
#else
#define debug(...)
#endif

#define info(...)	console_printf(__VA_ARGS__)

/*console uart select*/
#define CONSOLE_UART_NUM				0

/* include test device list*/
#define CONFIG_TIMER_TEST	0
#define CONFIG_IO_TEST      0
#define CONFIG_USB_TEST     1
#endif

