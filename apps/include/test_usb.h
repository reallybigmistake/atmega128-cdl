
#ifndef __TEST_USB_H__
#define __TEST_USB_H__
#include "test_config.h"
#include "ch372.h"

#if (CONFIG_USB_TEST == 1)

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/


/*************************************************************************************************************/
// structure definition
/*************************************************************************************************************/


/*************************************************************************************************************/
// global variable declaration
/*************************************************************************************************************/
extern cmdline_entry usb_test_menu[];

#else
#define usb_test_menu 	NULL
#endif

#endif