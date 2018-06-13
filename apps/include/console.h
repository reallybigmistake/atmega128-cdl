#ifndef __CONSOLE_H__
#define __CONSOLE_H__
#include<stdarg.h>
void console_init(int uart_id, u32 baudrate);
unsigned long console_baud(void);
void console_flush(void);

// int console_tstc(void);
void console_putc(const char c);
char console_getc(void);

void console_puts(const char* s);
int console_gets(char* buf, int len);

int console_printf(const char* fmt, ...);

// int console_check_char(unsigned char check);

#endif
