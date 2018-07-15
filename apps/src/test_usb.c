#include "test_usb.h"
#if (CONFIG_USB_TEST == 1)
static int cmd_help(int argc, char *argv[])
{
	return cmdline_help_general(&usb_test_menu[0]);
}

static int usb_probe(int argc, char *argv[])
{
	char val1;
	char val2;
	if (argc < 2) 
	{
		info("wrong cmd_format: usb_probe val\n", argv[0]);
		return 0;
	}

	// Get the parameters
	val1 = get_arg_uint(argv[1]);

    init_gpio();
	sel_data(0);
	write_char(CHECK_EXIST);
	sel_data(1);
	write_char(val1);
	info("input val is 0x%x\r\n", (int)val1);
	while(!int_detected());
	sel_data(1);
	val2 = read_char();
	info("get value 0x%x", (int)val2);
	if(val1 != ~val2)
	{
		info("probe test fail\n");
		return -1;
	}
	info("test pass");
	return 0;
}

cmdline_entry usb_test_menu[] = {
	{"help", 			cmd_help,		"           : Display list of commands"},
	{"h",				cmd_help, 		"              : alias for help"},
	{"?",				cmd_help, 		"              : alias for help"},
	{"usb_probe",		usb_probe,	"    : ch372 probe test"},
	{0, 				0, 				0}
};


#endif

