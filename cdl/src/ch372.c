#include "ch372.h"
#include "gpio.h"
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

int write_endpoint(char* s, int endpoint, int len)
{
    char cmd;
    int i,j;
    switch(endpoint){
        case 0: cmd = WR_USB_DATA3;break;
        case 1: cmd = WR_USB_DATA5;break;
        case 2: cmd = WR_USB_DATA7;break;
        default:break;
    }
    sel_cmd();
    write_char(cmd);
    sel_data();
    write_char(len);
    for(i=0;i<len, j++){
        write_char(*(s+i));
    }
}


/*******************************************************/
