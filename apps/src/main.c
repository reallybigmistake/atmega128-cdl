/*----------------------------------------------------------------------------------------------------------*/
/*                                                                                               											 		 */
/*           Copyright (C) 2016 Brite Semiconductor Co., Ltd. All rights reserved.                									 */
/*                                                                                                													 */
/*-----------------------------------------------------------------------------------------------------------*/
#include "test_config.h"
#include "test_timer.h"
#include "test_gpio.h"
#include "test_usb.h"

/*************************************************************************************************************/
// macro definition
/*************************************************************************************************************/
#define CMD_BUF_SIZE		64

typedef struct test_func_menu_s {
	cmdline_entry* menu;
	int (*prepare)(int);
	int dev_sel;
	const char* desc;
	int (*cleanup)(int);
} test_func_menu;

/*************************************************************************************************************/
// global variable definition
/*************************************************************************************************************/
static char g_cmd_buf[CMD_BUF_SIZE];
static char *g_cmd_buf_endp;

/*************************************************************************************************************/
// function prototype
/*************************************************************************************************************/


// device list for test fun
const test_func_menu menu[] = {
	{timer_test_menu,	timer_test_prepare,	0, "timer", 			timer_test_cleanup },
	{gpio_test_menu, NULL, 0, "gpio", NULL},
	{usb_test_menu, NULL, 0, "usb", NULL},
};

/*************************************************************************************************************/
// function implementation
/*************************************************************************************************************/
int one_device_test(int idx)
{
	int ret;

	/*display the help,list all support test command for the device*/
	info("device: %s test case\n", menu[idx].desc);
	ret = cmdline_process("help", menu[idx].menu);

	while (1) 
	{
		/* Print a prompt to the console.  Show the CWD.*/
		info("\n%s> ", "cmd:");

		/*Get a line of text from the user.*/
		console_gets(g_cmd_buf, sizeof(g_cmd_buf));

		/* Pass the line from the user to the command processor.*/
		/* It will be parsed and valid commands executed.*/
		ret = cmdline_process(g_cmd_buf, menu[idx].menu);

		/* Handle the case of bad command.*/
		if (ret == CMDLINE_QUIT) 
		{
			info("Quit %s test\n", menu[idx].desc);				
			break;
		} 
		if (ret == CMDLINE_BAD_CMD)
			info("Bad command!\n");
		else if (ret == CMDLINE_TOO_MANY_ARGS) 
			info("Too many arguments for command processor!\n");
		else if (ret != 0) 
			info("Command returned error code %d\n", ret);
	}

	if (menu[idx].cleanup != NULL) 
		(menu[idx].cleanup)(menu[idx].dev_sel);
	
	return 0;
}


int main(void)
{
	int i;
	int j;
	int ret;
	unsigned long usel = 0;
	console_init(0, 38400);
	debug("atmega cdl test\n");

	while (1) 
	{
		info("\nColumbus CDL test,please select device first\n");

		/*display test device list*/
		for (i = 0; i < ARRAY_SIZE(menu); i++) {
			if (menu[i].menu == 0) 
				info("%d: %s  %s\r\n", i, menu[i].desc, "!!! not implemented");
			else 
				info("%d: %s  %s\r\n", i, menu[i].desc, "");
		}

		/*get user selection and run the test*/
		info("\r\nSelect the device to test : ");
		console_gets(g_cmd_buf, sizeof(g_cmd_buf));
		usel = (unsigned long)simple_strtoul((const char*)g_cmd_buf, &g_cmd_buf_endp, 10);
		if(g_cmd_buf == g_cmd_buf_endp) {
			usel = -1;
			for(j=0; j<ARRAY_SIZE(menu); j++) {
				if(menu[j].menu && !strcmp(menu[j].desc, g_cmd_buf)) {
					usel = j;
					break;
				}
			}
		}

		if (usel >= 0 && usel < ARRAY_SIZE(menu)) {
			if (menu[usel].menu == NULL) {
				info("%s test not imlpemented now\n", menu[usel].desc);
			} else {
				if (menu[usel].prepare != NULL) 
					ret = (menu[usel].prepare)(menu[usel].dev_sel);
				else 
					ret = 0;
				
				if (!ret) 
					one_device_test(usel);
			}
		}
		
		info("\r\n");
	}
	
	while (1);
	return 0;
}


