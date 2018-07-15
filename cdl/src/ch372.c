#include "ch372.h"
#include "gpio.h"
int int_detected()
{
    return !pin_get_input(PORTCTRL, PIN_INT);

}
void sel_data(uint8_t en)
{
    if(en){
        pin_set_output(PORTCTRL, PIN_INT, SEL_DATA);
    }else{
        pin_set_output(PORTCTRL, PIN_INT, SEL_CMD);
    }       
}

void enable_write(uint8_t en)
{
    if(en){
        pin_set_output(PORTCTRL, PIN_WR, LOW);
    }else{
        pin_set_output(PORTCTRL, PIN_WR, HIGH);
    }
}

void enable_read(uint8_t en)
{
    if(en){
        pin_set_output(PORTCTRL, PIN_RD, LOW);
    }else{
        pin_set_output(PORTCTRL, PIN_RD, HIGH);
    }
}

void set_char(char c)
{
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
    enable_write(1);
    set_char(c);
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
void send_data(char* data)
{

}
void read_data(char* buf)
{

}