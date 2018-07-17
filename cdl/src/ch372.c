#include "ch372.h"
#include "gpio.h"
#include "usb_ch9.h"
#include "usb_hid.h"
#include <stdarg.h>
int int_detected()
{
    return !pin_get_input(PORTCTRL, PIN_INT);

}
void sel_data()
{
    pin_set_output(PORTCTRL, PIN_INT, SEL_DATA);
}
void sel_cmd()
{
    pin_set_output(PORTCTRL, PIN_INT, SEL_DATA);
}

void enable_write(int en)
{
    if(en){
        pin_set_output(PORTCTRL, PIN_WR, LOW);
    }else{
        pin_set_output(PORTCTRL, PIN_WR, HIGH);
    }
}

void enable_read(int en)
{
    //set ddr as 0xff before start read
    if(en){
        port_set_direction(PORTDATA, 0x00);
        pin_set_output(PORTCTRL, PIN_RD, LOW);
    }else{
        pin_set_output(PORTCTRL, PIN_RD, HIGH);
    }
}

void set_char(char c)
{
    port_set_direction(PORTDATA, 0xff);
    port_set_output(PORTDATA, c);
}


char get_char()
{
    return port_get_input(PORTDATA);
}

void init_gpio()
{
    pullup_disable(1);

    pin_set_output(PORTCTRL, PIN_INT, HIGH);
    pin_set_output(PORTCTRL, PIN_A0, HIGH);
    pin_set_output(PORTCTRL, PIN_RD, HIGH);
    pin_set_output(PORTCTRL, PIN_WR, HIGH);

    pin_set_direction(PORTCTRL, PIN_INT, IN);
    pin_set_direction(PORTCTRL, PIN_A0, OUT);
    pin_set_direction(PORTCTRL, PIN_RD, OUT);
    pin_set_direction(PORTCTRL, PIN_WR, OUT);
    
    port_set_direction(PORTDATA, 0x00);
    port_set_output(PORTDATA, 0x00);

}
void write_char(char c)
{
    set_char(c);
    enable_write(1);   
    enable_write(0);
}

char read_char()
{
    char val;
    enable_read(1);
    val = get_char();
    enable_read(0);
    return val;
}




/*********************************************/

int check_exist(char val1)
{
    char val2;
    sel_cmd();
	write_char(CHECK_EXIST);
	sel_data();
	write_char(val2);
	read_char();
	if(val1 == ~val2)
	{
		return 1;
	}
	return 0;
}

void ch372_reset()
{
    sel_cmd();
	write_char(RESET_ALL);
}

void unlock_buffer()
{
    sel_cmd();
    write_char(UNLOCK_USB);   
}
void set_usb_mode(E_USB_MODE mode)
{
    sel_cmd();
	write_char(SET_USB_MODE);
	sel_data();
	write_char(mode);
}

char get_status()
{
    sel_cmd();
	write_char(GET_STATUS);
	sel_data();
	return read_char();
}

void set_address(char addr)
{
    sel_cmd();
	write_char(SET_USB_ADDR);
	sel_data();
	write_char(addr);
}

int read_data(char* buffer, int len)
{   
    int i, j;
    sel_cmd();
	write_char(RD_USB_DATA);
	sel_data();
	j = read_char();
    if(len<j)
        j=len;
    for(i=0;i<j;i++){
        buffer[i]=read_char();
    }
    len = j;
    buffer[j]=0;
    return len;
}

int write_chars(char* s, int len)
{
    sel_data();
    write_char(len);
    for(i=0;i<len;i++){
        write_char(*(s+i));
    }
}

int write_endpoint(int endpoint)
{
    char cmd;
    int buf_size;
    int i=0;
    switch(endpoint)
    {
        case 0: cmd = WR_USB_DATA3; buf_size=EP0_BUF_SIZE; break;
        case 1: cmd = WR_USB_DATA5; buf_size=EP1_BUF_SIZE; break;
        case 2: cmd = WR_USB_DATA7; buf_size=EP1_BUF_SIZE; break;
        default:break;
    }
    if(count>0)
    {
        sel_cmd();
        write_char(cmd);
        if(count<buf_size) //if count less than buffer size, send len=count
        {
            write_chars(p_wr, count)
        }else //if count bigger than buffer size, send len=buffer size
        {
            write_chars(p_wr, buf_size)
            count -= buf_size;
        }
    }else if(count == 0)    //if count is zero, send 0 len empty package
    {
        sel_cmd();
        write_char(cmd);
        write_chars(p_wr, 0);
    }
    else return -1
}

/*******************************************************/
extern int console_printf(const char* fmt, ...);
#define info(...)	console_printf(__VA_ARGS__)
struct usb_ctrlrequest request;
struct usb_ctrlrequest* p_request = &request;
char read_buffer[EP_MAX_SIZE];
char* p_send;
int count;
void usb_ep0_setup()
{
    int len;
    char* req = (char*)p_request;
    len = read_data(ep_buffer);
    read_data(request, len);
    if((p_request->bRequestType)&0x80 ==USB_DIR_IN)
    {
        info("DIR_IN_REQUEST\n");
        switch((p_request->bRequestType)&USB_TYPE_MASK)
        {
            case USB_TYPE_STANDARD:
                info("USB_TYPE_STANDARD\n")
                switch(p_request->bRequest)
                {
                    case USB_REQ_GET_DESCRIPTOR:
                        info("USB_REQ_GET_DESCRIPTOR\n");
                        switch((p_request->wValue)>>8)
                        {
                            case USB_DT_DEVICE:
                                p_send = (char*)p_dev_descriptor;
                                count = p_dev_descriptor->bLength;
                                info("requeset size : %d\n", count);
                                break;
                            case USB_DT_CONFIG:
                                p_send = (char*)p_cfg_descriptor;
                                count = p_dev_descriptor->bLength;
                                info("requeset size : %d\n", count);
                                break;
                            case USB_DT_STRING:
                                p_send = (char*)p_string_descriptor;
                                count = p_dev_descriptor->bLength;
                                info("requeset size : %d\n", count);
                                break;
                            write_endpoint(0);
                        }
                        
                    case USB_REQ_GET_CONFIGURATION:
                        info("USB_REQ_GET_CONFIGURATION\n");    break;
                    case USB_REQ_SET_ADDRESS:
                        info("USB_REQ_SET_ADDRESS\n");   break;
                    default:
                        info("UNKNOWN REQUEST\n");    break;
                }
            case USB_TYPE_CLASS:
                info("USB_TYPE_CLASS\n");    break;
            case USB_TYPE_VENDOR:
                info("USB_TYPE_VENDOR\n");    break;
            case USB_TYPE_RESERVED:
                info("USB_TYPE_VENDOR\n");    break;
            default:
                info("UNKNOWN USB REQ TYPE\n"); break;
        }
        // switch((p_request->bRequestType)&USB_RECIP_MASK)

    }
    else
    {
        info("DIR_OUT_REQUEST\n")
    }


	// read_data(ep_buffer, 8);

}
void usb_ep0_out()
{
    write_endpoint(0);
    unlock_buffer();
}
void usb_ep0_in()
{

}
void usb_ep1_out()
{

}
void usb_ep1_in()
{
    
}
void usb_ep2_out()
{

}
void usb_ep2_in()
{
    
}
void usb_suspend()
{

}
void usb_wakeup()
{

}