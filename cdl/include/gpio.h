#ifndef __IO_H__
#define __IO_H__
#include <avr/io.h>
#include <inttypes.h>
#define PORT 0
#define DDR  1
#define PIN  2

typedef enum{
    IOA,
    IOB,
    IOC,
    IOD,
    IOE,
    IOF,
    IOG,
}E_IO_ID;
typedef enum{
    LOW,
    HIGH,
}E_PIN_VAL;
typedef enum{
    IN,
    OUT,
}E_PIN_DIR;


void port_set_output(E_IO_ID id, uint8_t val);
uint8_t port_get_output(E_IO_ID id);
void port_set_direction(E_IO_ID id, uint8_t val);
uint8_t port_get_direction(E_IO_ID id);
uint8_t port_get_input(E_IO_ID id);
void pin_set_output(E_IO_ID id, uint8_t pin, E_PIN_VAL pin_val);
E_PIN_VAL pin_get_input(E_IO_ID id, uint8_t pin);
void pin_set_direction(E_IO_ID id, uint8_t pin, E_PIN_DIR pin_dir);
void pullup_disable(uint8_t en);
#endif