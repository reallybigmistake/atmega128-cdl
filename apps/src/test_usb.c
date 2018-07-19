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
	sel_cmd();
	write_char(CHECK_EXIST);
	sel_data();
	write_char(val1);
	info("input val is 0x%x\r\n", val1);
	sel_data();
	val2 = read_char();
	info("get value 0x%x\n", val2&0xff);
	if(val1 != ~val2)
	{
		info("probe test fail\n");
		return -1;
	}
	info("test pass\n");
	return 0;
}

static int usb_hid(int argc, char *argv[])
{	
	char int_status;
	set_usb_mode(MODE_CUSTOM_FIRMWARE);//custom firmware mode
	while(!int_detected()){
		int_status = get_status();
		switch(int_status){
			case USB_INT_EP0_SETUP:	usb_ep0_setup();	break;    	//端点 0 的接收器接收到数据，SETUP 成功
			case USB_INT_EP0_OUT:	usb_ep0_out();		break;   	//端点 0 的接收器接收到数据，OUT 成功
			case CMD_RET_SUCCESS:	usb_ep0_in();		break;    	//端点 0 的发送器发送完数据，IN 成功
			case USB_INT_EP1_OUT:	usb_ep1_out();		break;    	//辅助端点/端点 1 接收到数据，OUT 成功
			case USB_INT_EP1_IN:	usb_ep1_in();		break;  		//中断端点/端点 1 发送完数据，IN 成功
			case USB_INT_EP2_OUT:	usb_ep2_out();		break;     	//批量端点/端点 2 接收到数据，OUT 成功
			case USB_INT_EP2_IN:	usb_ep2_in();		break;    	//批量端点/端点 2 发送完数据，IN 成功
			case USB_INT_USB_SUSPEND:usb_suspend();		break;	//USB 总线挂起事件（如果已 CHK_SUSPEND）
			case USB_INT_WAKE_UP:	usb_wakeup();		break;   	//从睡眠中被唤醒事件（如果已 ENTER_SLEEP）
			case USB_INT_BUS_RESET1:
			case USB_INT_BUS_RESET2:
			case USB_INT_BUS_RESET3:
			case USB_INT_BUS_RESET4:usb_reset();		break;
			default:info("unknown interrupt %d\n", int_status);break;
		}
	}

	
}

cmdline_entry usb_test_menu[] = {
	{"help", 			cmd_help,		"           : Display list of commands"},
	{"h",				cmd_help, 		"              : alias for help"},
	{"?",				cmd_help, 		"              : alias for help"},
	{"usb_probe",		usb_probe,		"    : ch372 probe test"},
	{"usb_hid",			usb_hid,		"    : ch372 hid test"},
	{0, 				0, 				0}
};


#endif

