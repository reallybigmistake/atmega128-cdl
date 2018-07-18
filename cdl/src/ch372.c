#include "ch372.h"
#include "gpio.h"
#include "usb_ch9.h"
#include "usb_hid.h"
// #include <stdarg.h>

/*******************************************************/
// extern int console_printf(const char* fmt, ...);
// #define info(...)	console_printf(__VA_ARGS__)
struct usb_ctrlrequest request;
struct usb_ctrlrequest* p_request = &request;
char read_buffer[EP_MAX_SIZE];
char* p_send;
/*
*SendCount>0,continue transfer; 
*SendCount=0,send 0 length package; 
*SendCount<0,transfer done
*/
int SendCount;
/*
*inCtrlTransfer = 0, not in contrl transfer
*other in contrl transfer
*/ 
int InCtrlTransfer;
char Address = 0;
/******************************************************/



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
    Address = 0;
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

void write_chars(char* s, int len)
{
    sel_data();
    write_char(len);
    for(i=0;i<len;i++){
        write_char(*(s+i));
    }
}

void write_cmd(char cmd)
{
    sel_cmd();
    write_char(cmd);
}

int write_endpoint(int endpoint)
{
    char ep_send_cmd;
    int buf_size;
    switch(endpoint)
    {
        case 0: cmd = WR_USB_DATA3; buf_size=EP0_BUF_SIZE; break;
        case 1: cmd = WR_USB_DATA5; buf_size=EP1_BUF_SIZE; break;
        case 2: cmd = WR_USB_DATA7; buf_size=EP1_BUF_SIZE; break;
        default:break;
    }
    if(SendCount>0)
    {
        write_cmd(ep_send_cmd);
        if(SendCount<buf_size)  //if SendCount less than buffer size, send len=SendCount
        {
            write_chars(p_wr, SendCount)
            SendCount = -1;     //transfer done
            InCtrlTransfer = 0; //InCtrlTransfer only affect control transfer
        }else               //if SendCount bigger than buffer size, send len=buffer size
        {
            write_chars(p_wr, buf_size)
            SendCount -= buf_size;
        }
    }
    else if(SendCount == 0)    //when SendCount reach zero, send 0 package if InctrlTransfer
    {                       //else just ignor the SendCount=0
        if(InCtrlTransfer)  //anyway ,set the SendCount=-1, to finish the send
        {
            if(!Address)break;  //when first time get device descriptor, no zero package
            write_cmd(ep_send_cmd);
            write_chars(p_wr, 0);  
            InCtrlTransfer = 0; //contrl transfer finish after the 0 package
        }
        SendCount = -1;         
    }
    // else unlock_buffer();
}

int write_endpoint0()
{
    char ep_send_cmd;
    int buf_size;
    cmd = WR_USB_DATA3;
    buf_size=EP0_BUF_SIZE;
    if(SendCount>0)
    {
        write_cmd(ep_send_cmd);
        if(SendCount<buf_size)  //if SendCount less than buffer size, send len=SendCount
        {
            write_chars(p_wr, SendCount)
            SendCount = -1;     //transfer done
            InCtrlTransfer = 0; //InCtrlTransfer only affect control transfer
        }else               //if SendCount bigger than buffer size, send len=buffer size
        {
            write_chars(p_wr, buf_size)
            SendCount -= buf_size;
        }
    }
    else if(SendCount == 0)    //when SendCount reach zero, send 0 package if InctrlTransfer
    {                       //else just ignor the SendCount=0
        if(InCtrlTransfer)  //anyway ,set the SendCount=-1, to finish the send
        {
            if(!Address)break;  //when first time get device descriptor, no zero package
            write_cmd(ep_send_cmd);
            write_chars(p_wr, 0);  
            InCtrlTransfer = 0; //contrl transfer finish after the 0 package
        }
        SendCount = -1;         
    }
    // else unlock_buffer();
}

/*********************************************************
 * interrupt handlers
 * 
 * ******************************************************/
void usb_ep0_setup()
{
    int len;
    char* req = (char*)p_request;
    InCtrlTransfer = 1;     //set ctrl transfer flag
    len = read_data(ep_buffer);
    read_data(request, len);
    if((p_request->bRequestType)&0x80 == USB_DIR_IN)
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
                                if(!Address) //first time request device descriptor
                                {
                                    SendCount = EP0_BUF_SIZE;
                                }else
                                {
                                    SendCount = p_dev_descriptor->bLength;
                                }
                                info("requeset size : %d\n", SendCount);
                                break;
                            case USB_DT_CONFIG:
                                p_send = (char*)p_cfg_descriptor;
                                SendCount = p_dev_descriptor->bLength;
                                info("requeset size : %d\n", SendCount);
                                break;
                            case USB_DT_STRING:
                                p_send = (char*)p_string_descriptor;
                                SendCount = p_dev_descriptor->bLength;
                                info("requeset size : %d\n", SendCount);
                                break;   
                        }
                         write_endpoint(0);  //send descriptor
                         break;                    
                    case USB_REQ_GET_CONFIGURATION:
                        info("USB_REQ_GET_CONFIGURATION\n");    break;
                    default:
                        info("UNKNOWN REQUEST 0x%x\n", p_request->bRequest);    break;
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

    }
    else
    {
        info("DIR_OUT_REQUEST\n");
        switch((p_request->bRequestType)&USB_TYPE_MASK)
        {
            case USB_TYPE_STANDARD:
                info("USB_TYPE_STANDARD\n");
                switch(p_request->bRequest)
                {
                    case USB_REQ_SET_Address:
                        info("USB_REQ_SET_Address\n");    
                        SendCount = 0;  //send 0 package
                        write_endpoint(0);
                        info("get new address 0x%x\n", p_request->wValue);
                        break;
                    default:
                        info("UNKNOWN REQUEST 0x%x\n", p_request->bRequest);    break;
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
    }
    unlock_buffer();
}
void usb_ep0_out()
{
    info("usb_ep0_out\n");
}
void usb_ep0_in()
{
    info("usb_ep0_in\n");
    write_endpoint(0);
    if(p_request->bRequest == USB_REQ_SET_Address)
    {
        Address = p_request->wValue;
        set_address(Address);
    }
    unlock_buffer();
}
void usb_ep1_out()
{
    info("usb_ep1_out\n");
}
void usb_ep1_in()
{
    info("usb_ep1_in\n");
}
void usb_ep2_out()
{
    info("usb_ep2_out\n");
}
void usb_ep2_in()
{
    info("usb_ep2_in\n");
}
void usb_suspend()
{
    info("usb_suspend\n");
}
void usb_wakeup()
{
    info("usb_wakeup\n");
}
void usb_reset()
{
    info("usb_reset\n");
    write_cmd(RESET_ALL);
    SendCount = 0;
    p_request = 0;
}