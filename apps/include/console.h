#ifndef __CONSOLE_H__
#define __CONSOLE_H__
#include<stdarg.h>
#include<stdio.h>
#include<string.h>

#define CONSOLE_BUF_SIZE	128

void console_init(int uart_id, unsigned long baudrate);
unsigned long console_baud(void);
void console_flush(void);

// int console_tstc(void);
void console_putc(const char c);
char console_getc(void);

void console_puts(const char* s);
int console_gets(char* buf, int len);

int console_printf(const char* fmt, ...);
// int printf(const char* fmt, ...);
// int console_check_char(unsigned char check);

#endif
