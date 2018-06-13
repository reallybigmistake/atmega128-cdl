#ifndef __CDL_TYPES_H__
#define __CDL_TYPES_H__

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;


#define read8(a) (*(volatile u8 *)(a))
#define write8(a, v) (*(volatile u8 *)(a)) = ((u8)v)

#define DEBUG_OUTPUT
#ifdef 	DEBUG_OUTPUT
#define debug(...)	console_printf (__VA_ARGS__)
#else
#define debug(...)
#endif
#define info(...)	console_printf(__VA_ARGS__)


/*dependence apis*/
extern unsigned long get_timer(unsigned long start);
extern int console_printf(const char* fmt, ...);
extern void* memcpy(void*, const void*, unsigned int);
extern void* memset(void*, int, unsigned int);
extern void udelay(unsigned long usec);
extern void *malloc(unsigned int size);
extern void free(void* p);
#endif