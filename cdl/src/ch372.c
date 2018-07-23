#include "ch372.h"
#include "gpio.h"
#include "usb_ch9.h"
#include "usb_hid.h"
#include "cdl_types.h"
#include <util/delay.h>

/*******************************************************/
// struct usb_ctrlrequest request;
struct usb_ctrlrequest request;
struct usb_ctrlrequest* p_request=&request;
char read_buffer[EP_MAX_SIZE];
char* p_send;
/*
*SendCount>0,continue transfer; 
*
* or ,transfer done
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
    return !pin_get_input(PORTINT, PIN_INT);

}
void sel_data()
{
    pin_set_output(PORTCTRL, PIN_A0, SEL_DATA);
}
void sel_cmd()
{
    pin_set_output(PORTCTRL, PIN_A0, SEL_CMD);
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
    //set ddr as 0x00 before start read
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
    // pullup_disable(1);

    pin_set_output(PORTINT, PIN_INT, HIGH);
    pin_set_output(PORTCTRL, PIN_A0, HIGH);
    pin_set_output(PORTCTRL, PIN_RD, HIGH);
    pin_set_output(PORTCTRL, PIN_WR, HIGH);

    pin_set_direction(PORTINT, PIN_INT, IN);
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
    // _delay_us(2);
}

char read_char()
{
    char val;
    enable_read(1);
    val = get_char();
    enable_read(0);
    // _delay_us(2);
    return val;
}




/*********************************************/

int check_exist(char val1)
{
    char val2;
    sel_cmd();
	write_char(CHECK_EXIST);
	sel_data();
	write_char(val1);
    sel_data();
	val2 = read_char();
    info("input val is 0x%x, outut val is 0x%x\n", val1&0xff, val2&0xff);
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
    info("unlock\n");  
}
void set_usb_mode(E_USB_MODE mode)
{
    sel_cmd();
	write_char(SET_USB_MODE);
	sel_data();
	write_char(mode);
    _delay_us(10);
}

char get_status()
{
    sel_cmd();
	write_char(GET_STATUS);
	sel_data();
	return read_char();
}
char get_toggle()
{
    sel_cmd();
	write_char(GET_TOGGLE);
	sel_data();
    write_char(0x1A);
	return read_char();
}
void set_address(char addr)
{
    sel_cmd();
	write_char(SET_USB_ADDR);
	sel_data();
	write_char(addr);
}

int read_ep_buffer(char* buffer)
{   
    int i, len;
    sel_cmd();
	write_char(RD_USB_DATA0);
	sel_data();
	len = read_char();
    // info("get len %d :", len);
    for(i=0;i<len;i++){
        buffer[i]=read_char();
        // info("0x%x,", buffer[i]&0xff);
    }
    // info("\n");
    return len;
}

void write_chars(char* s, int len)
{
    int i;
    sel_data();
    write_char(len);
    // info("write len %d :", len);
    for(i=0;i<len;i++){
        write_char(*(s+i));
        // info("0x%x,", *(s+i)&0xff);
    }
    // info("\n");
}

void write_cmd(char cmd)
{
    sel_cmd();
    write_char(cmd);
}
/******************************************************************
**setup trasition
*******************************************************************/

// void write_endpoint0()
// {
//     char ep_send_cmd;
//     int buf_size;
//     ep_send_cmd = WR_USB_DATA3;
//     buf_size=EP0_BUF_SIZE;
//     if(SendCount>0){
//         write_cmd(ep_send_cmd);
//         if(SendCount<buf_size)  //if SendCount less than buffer size, send len=SendCount
//         {
//             write_chars(p_send, SendCount);
//             SendCount = -1;     //transfer done
//         }else               //if SendCount bigger than buffer size, send len=buffer size
//         {
//             write_chars(p_send, buf_size);
//             if(!Address){
//                 SendCount = -1;
//             }else{
//                 SendCount -= buf_size;
//             }
//             p_send += buf_size;
//         }
//     }else if (SendCount == 0){
//         write_chars(p_send, 0);
//     }
// }
void write_endpoint0()
{
    char ep_send_cmd;
    int buf_size;
    ep_send_cmd = WR_USB_DATA3;
    buf_size=EP0_BUF_SIZE;
    if(SendCount>0){
        write_cmd(ep_send_cmd);
        if(SendCount<buf_size)  //if SendCount less than buffer size, send len=SendCount
        {
            write_chars(p_send, SendCount);
            SendCount = 0;     //transfer done
        }else               //if SendCount bigger than buffer size, send len=buffer size
        {
            write_chars(p_send, buf_size);
            p_send += buf_size;
            SendCount -= buf_size;
        }
    }
}
void setup_get_descriptor()
{
    switch((p_request->wValue)>>8){
    case USB_DT_DEVICE:
        info("USB_DT_DEVICE\n");
        p_send = (char*)p_dev_descriptor;
        SendCount = p_dev_descriptor->bLength;
        info("request size : %d\n", SendCount);
        break;
    case USB_DT_CONFIG:
        info("USB_DT_CONFIG\n");
        p_send = (char*)p_cfg_descriptor;
        SendCount = p_dev_descriptor->bLength;
        info("request size : %d\n", SendCount);
        break;
    case USB_DT_STRING:
        info("USB_DT_STRING\n");
        p_send = (char*)p_string_descriptor;
        SendCount = p_dev_descriptor->bLength;
        info("request size : %d\n", SendCount);
        break;
    default:info("Unknown USB_DT 0x%x", (p_request->wValue)>>8);break;
    }
}



/*********************************************************
 * interrupt handlers
 * 
 * ******************************************************/
void usb_ep0_setup()
{
    info("usb_ep0_setup\n");
    int len;
    p_request = &request;
    char* req = (char*)p_request;
    InCtrlTransfer = 1;     //set ctrl transfer flag
    len = read_ep_buffer(req);
    info("request len %d:", len);
    for(int i=0; i<len;i++){
        info("0x%x,",*(req+i)&0xff);
        }
    if(((p_request->bRequestType)&0x80) == USB_DIR_IN)
    { 
        info("DIR_IN_REQUEST\n");
        switch(p_request->bRequestType&USB_TYPE_MASK){
        case USB_TYPE_STANDARD:
            info("USB_TYPE_STANDARD\n");
            switch(p_request->bRequest){
            case USB_REQ_GET_DESCRIPTOR:
                info("USB_REQ_GET_DESCRIPTOR\n");
                setup_get_descriptor();
                write_endpoint0();
                break;                    
            case USB_REQ_GET_CONFIGURATION:
                info("USB_REQ_GET_CONFIGURATION\n");    break;
            default:
                info("UNKNOWN REQUEST 0x%x\n", p_request->bRequest);    break;
            }
            break;
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
        switch((p_request->bRequestType)&USB_TYPE_MASK){
        case USB_TYPE_STANDARD:
            info("USB_TYPE_STANDARD\n");
            switch(p_request->bRequest){
            case USB_REQ_SET_ADDRESS:
                info("USB_REQ_SET_ADDRESS\n");
                Address = p_request->wValue;
                info("get new address 0x%x\n", Address);
                // set_address(Address);
                break;
            default:
                info("UNKNOWN REQUEST 0x%x\n", p_request->bRequest);    break;
            }
            break;
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
    info("toggle:0x%x\n", get_toggle());
    unlock_buffer();
}
void usb_ep0_in()
{
    info("usb_ep0_in\n");
    switch(p_request->bRequest){
    case USB_REQ_GET_DESCRIPTOR:
        info("USB_REQ_GET_DESCRIPTOR\n");
        write_endpoint0();
        break;
    case USB_REQ_SET_ADDRESS:
        info("USB_REQ_SET_ADDRESS\n");
        set_address(Address);
        info("set new address 0x%x\n", Address);
        SendCount=0;
        // write_endpoint0();
        break;
    }
    unlock_buffer();
}
void usb_ep1_out()
{
    info("usb_ep1_out\n");
    unlock_buffer();
}
void usb_ep1_in()
{
    info("usb_ep1_in\n");
    unlock_buffer();
}
void usb_ep2_out()
{
    info("usb_ep2_out\n");
    unlock_buffer();
}
void usb_ep2_in()
{
    info("usb_ep2_in\n");
    unlock_buffer();
}
void usb_suspend()
{
    info("usb_suspend\n");
    unlock_buffer();
}
void usb_wakeup()
{
    info("usb_wakeup\n");
    unlock_buffer();
}
void usb_bus_reset()
{
    info("ch372_reset\n");
    ch372_reset();
    SendCount = 0;
    Address = 0;
    unlock_buffer();
}

#if 0
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
#endif