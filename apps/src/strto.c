/*
 *	linux/lib/vsprintf.c
 *
 *	Copyright (C) 1991, 1992	Linus Torvalds
 */

/* vsprintf.c -- Lars Wirzenius & Linus Torvalds. */
/*
 * Wirzenius wrote this portably, Torvalds fucked it up :-)
 */

#include "ctype.h"
#include "errno.h"
#include "strto.h"
#include <string.h>
typedef unsigned char size_uc;

unsigned long simple_strtoul( const char* cp, char** endp, unsigned int base )
{
	unsigned long result = 0;
	unsigned long value;

	if ( *cp == '0' )
	{
		cp++;
		if ( ( *cp == 'x' ) && isxdigit( cp[1] ) )
		{
			base = 16;
			cp++;
		}

		if ( !base )
		{
			base = 8;
		}
	}

	if ( !base )
	{
		base = 10;
	}

	while ( isxdigit( *cp ) &&
		( value = isdigit( *cp ) ? *cp - '0' :
		( islower( *cp ) ? toupper( *cp ) : *cp ) - 'A' + 10 ) < base )
	{
		result = result * base + value;
		cp++;
	}

	if ( endp )
	{
		*endp = ( char* )cp;
	}

	return result;
}

int strict_strtoul( const char* cp, unsigned int base, unsigned long* res )
{
	char* tail;
	unsigned long val;
	size_uc len;

	*res = 0;
	len = strlen( cp );
	if ( len == 0 )
	{
		return -EINVAL;
	}

	val = simple_strtoul( cp, &tail, base );
	if ( tail == cp )
	{
		return -EINVAL;
	}

	if ( ( *tail == '\0' ) ||
		( ( len == ( size_uc )( tail - cp ) + 1 ) && ( *tail == '\n' ) ) )
	{
		*res = val;
		return 0;
	}

	return -EINVAL;
}

long simple_strtol( const char* cp, char** endp, unsigned int base )
{
	if ( *cp == '-' )
	{
		return -simple_strtoul( cp + 1, endp, base );
	}

	return simple_strtoul( cp, endp, base );
}

unsigned long ustrtoul( const char* cp, char** endp, unsigned int base )
{
	unsigned long result = simple_strtoul( cp, endp, base );
	switch ( **endp )
	{
		case 'G':
			result *= 1024;
			/* fall through */
		case 'M':
			result *= 1024;
			/* fall through */
		case 'K':
		case 'k':
			result *= 1024;
			if ( ( *endp )[1] == 'i' )
			{
				if ( ( *endp )[2] == 'B' )
				{
					( *endp ) += 3;
				}
				else
				{
					( *endp ) += 2;
				}
			}
	}
	return result;
}