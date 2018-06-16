/*
 *	linux/lib/vsprintf.c
 *
 *	Copyright (C) 1991, 1992	Linus Torvalds
 */

/* vsprintf.c -- Lars Wirzenius & Linus Torvalds. */
/*
 * Wirzenius wrote this portably, Torvalds fucked it up :-)
 */

#ifndef __STRTO_H__
#define __STRTO_H__

unsigned long simple_strtoul( const char* cp, char** endp, unsigned int base );
int strict_strtoul( const char* cp, unsigned int base, unsigned long* res );
long simple_strtol( const char* cp, char** endp, unsigned int base );
unsigned long ustrtoul( const char* cp, char** endp, unsigned int base );
#endif