#ifndef __UART_H__
#define __UART_H__
#include<avr/io.h>
#include"../inc/cdl_types.h"
/*Define register*/
#define UART_DATA(x) (UDR##x)
#define UART_CTL_STAT_A(x)  (UCSR##x##A)//RXCn TXCn UDREn FEn DORn UPEn U2Xn MPCMn
#define UART_CTL_STAT_B(x)  (UCSR##x##B)//RXCIEn TXCIEn UDRIEn RXENn TXENn UCSZn2 RXB8n TXB8n
#define UART_CTL_STAT_C(x)  (UCSR##x##C)//UMSELn UPMn1 UPMn0 USBSn UCSZn1 UCSZn0 UCPOLn
#define UART_BAUD_H(x)      (UBRR##x##H)
#define UART_BAUD_L(x)      (UBRR##x##L)
#define DIV_U2X(x)              (x?8:16)

/*line set offset*/
#define DATA_BIT_H_MASK (1<<2 )
#define DATA_BIT_L_MASK (3<<1)

#define STOP_BIT_MASK   (1<<3)

#define PARITY_MASK     (3<<4)
/*interrupt*/
/*data len UCSRnB[2] UCSRnC[2:1]*/
typedef enum{
    DATA_LEN_5BIT = 0,
    DATA_LEN_6BIT,
    DATA_LEN_7BIT,
    DATA_LEN_8BIT,
    DATA_LEN_9BIT = 0x7,
}E_DATA_BIT;
/*parity UCSRnC[5:4]*/
typedef enum{
    NO_PARITY,
    reserve,
    EVEN_PARITY,
    ODD_PARITY,
}E_PARITY;
/*stop bit UCSRnC[3]*/
typedef enum{
    STOP_BIT_1,
    STOP_BIT_2,
}E_STOP_BIT;



struct atmega128_uart{
    int id;
    u32 baudrate;
    u16 div;
    int u2x;
    int data_bits;
    int stop_bits;
    int parity;
};

#endif
