#include "uart.h"
u16 uart_calc_div(struct atmega128_uart* dev, u32 baudrate)
{
    return (F_CPU / (DIV_U2X(dev->u2x) * baudrate) -1);
}

void uart_enable_interrupt(int en)
{   ;
}
void uart_set_u2x(struct atmega128_uart* dev, int u2x)
{
    dev->u2x = u2x;
}

void uart_init(struct atmega128_uart* dev, u32 baudrate, int parity, int databits, int stopbit)
{
    int val;
    dev->baudrate = baudrate;
    dev->data_bits = databits;
    dev->stop_bits = stopbit;
    dev->parity = parity;
    dev->div = uart_calc_div(dev, baudrate);
    //set div
    UART_BAUD_H(dev->id) = ((dev->div)>>8)&0x0f;
    UART_BAUD_L(dev->id) = (dev->div)&0xff;
    //set data len
    val = UART_CTL_STAT_B(dev->id) & ~DATA_BIT_H_MASK;
    UART_CTL_STAT_B(dev->id) = val | ((dev->data_bits -5) & DATA_BIT_H_MASK);
    val = UART_CTL_STAT_C(dev->id) & ~DATA_BIT_L_MASK;
    UART_CTL_STAT_C(dev->id) = val | ((dev->data_bits -5) & DATA_BIT_L_MASK);
    //set parity
    val = UART_CTL_STAT_C(dev->id) & ~PARITY_MASK;
    UART_CTL_STAT_C(dev->id) = val | ((dev->parity) & STOP_BIT_MASK);

    //set stop bit
    val = UART_CTL_STAT_C(dev->id) & ~PARITY_MASK;
    UART_CTL_STAT_C(dev->id) = val | ((dev->stopbit - 1) & STOP_BIT_MASK);    
}

void uart_flush(struct atmega128_uart* dev)
{
    while(!(UART_CTL_STAT_A(dev->id)&(1<<UDRE0)));
}

void uart_putc(struct atmega128_uart* dev, char c)
{
    uart_flush(dev);
    UART_DATA(dev->id) = c;
}
char uart_getc(struct atmega128_uart* dev)
{
	while(!(UART_CTL_STAT_A(dev->id) & (1<<RXC0)));
	return UART_DATA(dev->id);
}

int uart_write(struct atmega128_uart* dev, const char* buf, int len)
{
	unsigned int i;
    for (i = 0; i < len; i++) {
        uart_putc(dev, buf[i]);
    }
	return(i);
}
int uart_read(struct atmega128_uart* dev, char* buf, int len)
{
	unsigned int i;
	for (i = 0; i < len; i++) {
		buf[i] = uart_getc(dev);
    }
	return(i);
}