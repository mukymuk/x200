#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "x200.h"

int main(int argc, char** argv) 
{
	uint16_t hz10;
    x200_open();
    if( argv[1] )
    {
        hz10 =  (uint16_t)(atof( argv[1] ) * 10.0);
        if( hz10 > 600 )
	        hz10 = 600;
        x200_set_freq( hz10 );
    }
    else
    {
        if( x200_get_freq( &hz10 ) )
		{
			printf( "%.1f\r\n", (double)hz10/10.0 );
		}
    }
    x200_close();
    return 0;
}

