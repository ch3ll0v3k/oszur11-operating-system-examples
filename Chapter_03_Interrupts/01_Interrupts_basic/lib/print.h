/*! "print template" */

#include <arch/console.h>
#include <lib/string.h>

/* template should be called from real "print":
	int print ( format, ... )
	{
		return __print ( stdio, font, &format );
	}
   where "stdio" and "font" are appropriately set
   (e.g. diferent for kernel and programs )
*/
static inline int __print ( console_t *console, int attr, char **fmt )
{
	char *format = *fmt;
	char **arg = fmt;
	int c;
	char buf[20];

	#define PRINT_CHAR(C)	console->print_char ( C, attr )

	if ( !fmt || !format )
		return 0;

	arg++; /* first argument after 'format' (on original 'print' stack) */

	while ( (c = *format++) != 0 )
	{
		if ( c != '%' )
		{
			PRINT_CHAR ( c );
		}
		else {
			char *p;

			c = *format++;
			switch ( c )
			{
			case 'd':
			case 'u':
			case 'x':
			case 'X':
				itoa ( buf, c, *((int *) arg++) );
				p = buf;
				while ( *p )
					PRINT_CHAR ( (int) *p++ );
				break;

			case 's':
				p = *arg++;
				if ( !p )
					p = "(null)";

				while ( *p )
					PRINT_CHAR ( (int) *p++ );
				break;

			default: /* assuming 'c' */
				PRINT_CHAR ( *((int *) arg++) );
				break;
			}
		}
	}

	return 0;
}