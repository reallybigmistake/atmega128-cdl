#include "gpio.h"
#include"../inc/cdl_types.h"
const uint8_t io_addr[] = {
    // PORTA
    0x3B, 0x3A, 0x39,
    // PORTB
    0x38, 0x37, 0x36, 
    // PORTC
    0x35, 0x34, 0x33,
    // PORTD
    0x32, 0x31, 0x30,
    // PORTE
    0x23, 0x22, 0x21,
    // PORTF
    0x62, 0x61, 0x20,
    // PORTG
    0x65, 0x64, 0x63,
};
static inline void io_write_reg(uint8_t reg_addr, uint8_t val)
{
	write8(reg_addr, val);
}

static inline uint8_t io_read_reg(uint8_t reg_addr)
{
	return read8(reg_addr);
}

void port_set_output(E_IO_ID id, uint8_t val)
{
    io_write_reg(io_addr[id*3+PORT], val);
}

uint8_t port_get_output(E_IO_ID id)
{
    io_read_reg(io_addr[id*3+PIN]);
}

void port_set_direction(E_IO_ID id, uint8_t val)
{
    io_write_reg(io_addr[id*3+DDR], val);
}

uint8_t port_get_direction(E_IO_ID id)
{
    return io_read_reg(io_addr[id*3+DDR]);
}

uint8_t port_get_input(E_IO_ID id)
{
    io_read_reg(io_addr[id*3+PIN]);
}

void io_set_int(E_IO_ID id, uint8_t val)
{

}

void pin_set_direction(E_IO_ID id, uint8_t pin, E_PIN_DIR pin_dir)
{
    uint8_t val;
    val = port_get_direction(id) & ~(1<<pin);
    port_set_direction(id, val | (pin_dir<<pin));
}
void pin_set_output(E_IO_ID id, uint8_t pin, E_PIN_VAL pin_val)
{
    uint8_t val;
    val = port_get_output(id) & ~(1<<pin);
    port_set_output(id, val|(pin_val<<pin));
}

E_PIN_VAL pin_get_input(E_IO_ID id, uint8_t pin)
{
    return port_get_input(id)&(1<<pin);
}

void pullup_disable(uint8_t en)
{
    int val;
    val = SFIOR & ~(1<<PUD);
    if(en){
        SFIOR = val | (1<<PUD);
    }else{
        SFIOR = val;
    }

}