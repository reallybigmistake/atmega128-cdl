#include "uart.h"
static inline void uart_write_reg(int reg_addr, u8 val)
{
	write8(reg_addr, val);
}

static inline u8 uart_read_reg(int reg_addr)
{
	return read8(reg_addr);
}

void uart_enable(struct atmega128_uart *dev, int en)
{
    int val;
    if(en){
        val = uart_read_reg(UART_CTL_STAT_B(dev->id));
        uart_write_reg(UART_CTL_STAT_B(dev->id), val|UART_ENABLE);
    }
    else{
        val = uart_read_reg(UART_CTL_STAT_B(dev->id));
        uart_write_reg(UART_CTL_STAT_B(dev->id), val & ~UART_ENABLE);
    }
    
}
u16 uart_calc_div(struct atmega128_uart* dev, u32 baudrate)
{
    return ((F_CPU + ((DIV_U2X(dev->u2x) * baudrate)/2))/ (DIV_U2X(dev->u2x) * baudrate) -1);
    // return (1600/DIV_U2X(dev->u2x)/144 -1);
        
}

void uart_enable_interrupt(int en)
{   ;
}
void uart_set_u2x(struct atmega128_uart* dev, int u2x)
{
    dev->u2x = u2x;
}

void uart_init(struct atmega128_uart* dev, u32 baudrate, char parity, int databits, int stopbit)
{
    int val;
    dev->baudrate = baudrate;
    dev->data_bits = databits;
    dev->stop_bits = stopbit;
    switch(parity)
    {
        case 'e':dev->parity=EVEN_PARITY;
            break;
        case 'o':dev->parity=ODD_PARITY;
            break;
        default:dev->parity=NO_PARITY;
    }
    dev->div = uart_calc_div(dev, baudrate);
    //set div
    uart_write_reg(UART_BAUD_H(dev->id), ((dev->div)>>8)&0x0f);
    uart_write_reg(UART_BAUD_L(dev->id), (dev->div)&0xff);


    //set data len
    
    if((dev->data_bits<9)&&(dev->data_bits>4)){
        val = uart_read_reg(UART_CTL_STAT_B(dev->id)) & ~DATA_BIT_H_MASK;

        val = uart_read_reg(UART_CTL_STAT_C(dev->id)) & ~DATA_BIT_L_MASK;
        uart_write_reg(UART_CTL_STAT_C(dev->id), val | ((dev->data_bits - 5)<<DATA_BIT_L_SHIFT));
    }else{
        val = uart_read_reg(UART_CTL_STAT_B(dev->id)) & ~DATA_BIT_H_MASK;
        uart_write_reg(UART_CTL_STAT_B(dev->id), val | (1<<DATA_BIT_H_SHIFT));
        val = uart_read_reg(UART_CTL_STAT_C(dev->id)) & ~DATA_BIT_L_MASK;
        uart_write_reg(UART_CTL_STAT_C(dev->id), val | (3<<DATA_BIT_L_SHIFT));
    }
    

    //set parity
    val = uart_read_reg(UART_CTL_STAT_C(dev->id)) & ~PARITY_MASK;
    uart_write_reg(UART_CTL_STAT_C(dev->id), val | ((dev->parity)<<PARITY_SHIFT));

    //set stop bit
    val = uart_read_reg(UART_CTL_STAT_C(dev->id)) & ~PARITY_MASK;
    uart_write_reg(UART_CTL_STAT_C(dev->id), val | ((dev->stop_bits - 1)<<STOP_BIT_SHIFT));
}

void uart_flush(struct atmega128_uart* dev)
{
    while(!(uart_read_reg(UART_CTL_STAT_A(dev->id)&(1<<UDRE0))));
}

void uart_putc(struct atmega128_uart* dev, char h)
{
    while(!(uart_read_reg(UART_CTL_STAT_A(dev->id))& (1<<UDRE0)));
    uart_write_reg(UART_DATA(dev->id), h);

}
char uart_getc(struct atmega128_uart* dev)
{
    while(!((uart_read_reg(UART_CTL_STAT_A(dev->id)))& (1<<RXC0)));
    return uart_read_reg(UART_DATA(dev->id));
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

void uart_enable_rx_interrupt(struct atmega128_uart* dev, int en)
{
    int val;
    if(en){
        val = uart_read_reg(UART_CTL_STAT_B(dev->id));
        uart_write_reg(UART_CTL_STAT_B(dev->id), val | 1<<RXCIE0);
    }
    else{
        val = uart_read_reg(UART_CTL_STAT_B(dev->id));
        uart_write_reg(UART_CTL_STAT_B(dev->id), val & ~(1<<RXCIE0));
    }
}

void uart_enable_tx_interrupt(struct atmega128_uart* dev, int en)
{
    int val;
    if(en){
        val = uart_read_reg(UART_CTL_STAT_B(dev->id));
        uart_write_reg(UART_CTL_STAT_B(dev->id), val | 1<<TXCIE0);
    }
    else{
        val = uart_read_reg(UART_CTL_STAT_B(dev->id));
        uart_write_reg(UART_CTL_STAT_B(dev->id), val & ~(1<<TXCIE0));
    }
}
