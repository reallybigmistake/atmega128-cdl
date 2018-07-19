#include "test_usb.h"
#include <util/delay.h>
#if (CONFIG_USB_TEST == 1)
static int cmd_help(int argc, char *argv[])
{
	return cmdline_help_general(&usb_test_menu[0]);
}

static int usb_probe(int argc, char *argv[])
{
	char val;
	if (argc < 2) 
	{
		info("wrong cmd_format: usb_probe val\n", argv[0]);
		return 0;
	}
	init_gpio();
	// Get the parameters
	val = get_arg_uint(argv[1]);
	if(!check_exist(val))
	{
		info("probe test fail\n");
		return -1;
	}
	info("test pass\n");
	return 0;
}

static int usb_hid(int argc, char *argv[])
{	char* param[2] = {"usb_probe", "0x55"};
	char int_status;
	int in_reset=0;
	init_gpio();
	set_usb_mode(MODE_CUSTOM_FIRMWARE);
	_delay_us(20);
	check_exist(0x5a);
	while(1){
		if(!int_detected())continue; 
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
			case USB_INT_BUS_RESET4:usb_reset();break;
			default:info("unknown interrupt %0x\n", int_status&0xff);break;
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

