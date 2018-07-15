#ifndef __CMDLINE_H__
#define __CMDLINE_H__

#define debug(...)	console_printf (__VA_ARGS__)

/* dependence on external functions*/
extern int console_printf( const char* fmt, ... );
extern void console_puts( const char* s );
extern void console_putc( const char c );
extern int console_gets(char* buf, int len);
extern char console_getc(void);
extern unsigned long simple_strtoul( const char* cp, char** endp, unsigned int base );
#define puts console_puts
#define putc console_putc
#define printf console_printf
#define gets console_gets
#define getc console_getc

#endif