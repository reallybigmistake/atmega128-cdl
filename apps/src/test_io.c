#include "test_io.h"
#define CONFIG_IO_TEST 1
#if (CONFIG_IO_TEST == 1)
// cmdline_entry gpio_test_menu[];

static int cmd_help(int argc, char *argv[])
{
	return cmdline_help_general(&gpio_test_menu[0]);
}

E_IO_ID io_get_port(char *argv)
{
	E_IO_ID port;

	if ((!strcmp(argv, "a")) || (!strcmp(argv, "A")))
	{
		port = IOA;
	}
	else if ((!strcmp(argv, "b")) || (!strcmp(argv, "B")))
	{
		port = IOB;
	}
	else if ((!strcmp(argv, "c")) || (!strcmp(argv, "C")))
	{
		port = IOC;
	}
	else if ((!strcmp(argv, "d")) || (!strcmp(argv, "D")))
	{
		port = IOD;
	}
    else if ((!strcmp(argv, "e")) || (!strcmp(argv, "E")))
	{
		port = IOE;
	}
    else if ((!strcmp(argv, "f")) || (!strcmp(argv, "F")))
	{
		port = IOF;
	}
    else if ((!strcmp(argv, "g")) || (!strcmp(argv, "G")))
	{
		port = IOG;
	}
	else
	{
		port = (char)(get_arg_uint(argv)&0xff);
	}

	if (port >7)
	{
		info("invalid, default to porta\n");
		port = IOA;
	}

	return port;
}

static int pin_output(int argc, char *argv[])
{
	uint8_t val;
	E_IO_ID port;
	uint8_t pin;

	info("%s:output test\n", argv[0]);

	// Check the validity of parameter count
	if (argc < 4) 
	{
		info("wrong cmd_format: %s port pin val\n", argv[0]);
		return 0;
	}

	// Get the parameters
	port = io_get_port(argv[1]); 
	pin = get_arg_uint(argv[2]);
	val = get_arg_uint(argv[3]);
	info("port:%s, pin: %s, val:%s\n", argv[1], argv[2], argv[3]);
	info("port:%d, pin: %d, val:%d\n", port, pin, val);

    pin_set_direction(port, pin, OUT);

	pin_set_output(port, pin, val);
	info("%s: set gpio port(%x), pin(%x) to level '%s'\n", argv[0], port, pin, val ? "high" : "low");
	info("%s: view the oscilloscope or multimeter result for pass or fail\n", argv[0]);

	return 0;
}

static int pin_input(int argc, char *argv[])
{
	uint8_t gpio_val;
	E_IO_ID port;
	uint8_t pin;

	info("%s:input test\n", argv[0]);

	// Check the validity of parameter count
	if (argc < 4) 
	{
		info("wrong cmd_format: %s port pin val\n", argv[0]);
		return 0;
	}
	
	// Get the parameters
	port = io_get_port(argv[1]);
	pin = (char)(get_arg_uint(argv[2]));

    pin_set_direction(port, pin, IN);

	gpio_val = pin_get_input(port, pin);
	debug("gpio input: get %d\n", gpio_val);
	
	return 0;
}

static int port_output(int argc, char *argv[])
{
	uint8_t val;
	E_IO_ID port;

	info("%s:output test\n", argv[0]);

	// Check the validity of parameter count
	if (argc < 3) 
	{
		info("wrong cmd_format: %s port val\n", argv[0]);
		return 0;
	}

	// Get the parameters
	port = io_get_port(argv[1]); 
	val = (char)(get_arg_uint(argv[2]));
	info("port:%d, val:%d\n", port, val);

    port_set_direction(port, OUT);

	port_set_output(port, val);
	info("%s: set gpio port(%x) to level '%s'\n", argv[0], port, val ? "high" : "low");
	info("%s: view the oscilloscope or multimeter result for pass or fail\n", argv[0]);

	return 0;
}

static int port_input(int argc, char *argv[])
{
	uint8_t gpio_val;
	E_IO_ID port;

	info("%s:input test\n", argv[0]);

	// Check the validity of parameter count
	if (argc < 2) 
	{
		info("wrong cmd_format: %s port\n", argv[0]);
		return 0;
	}
	
	// Get the parameters
	port = io_get_port(argv[1]);
    port_set_direction(port, IN);

	gpio_val = port_get_input(port);
	debug("gpio input: get %x\n", gpio_val);
	
	return 0;
}

cmdline_entry gpio_test_menu[] = {
	{"help", 			cmd_help,		"           : Display list of commands"},
	{"h",				cmd_help, 		"              : alias for help"},
	{"?",				cmd_help, 		"              : alias for help"},
	{"pin_output",		pin_output,	"    : gpio output test"},
	{"pin_input",		pin_input,		"     : gpio input test"},
	{"port_output",	    port_output,	"  : gpio interrupt type test"},
	{"port_input",	    port_input,	"  : gpio debounce test"},
	{0, 				0, 				0}
};


#endif

