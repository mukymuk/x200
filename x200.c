
#include <modbus/modbus.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "x200.h"


static modbus_t * s_p_modbus;

bool x200_open( void )
{
	if( !( s_p_modbus = modbus_new_rtu("/dev/ttyO1", 19200, 'N', 8, 1) ) )
	{
		printf("failed to open /dev/ttyO1:  %s\r\n", modbus_strerror(errno) );
		return false;
	}
    //modbus_set_debug( s_p_modbus, TRUE );
	modbus_set_slave(s_p_modbus, 1);
	if( modbus_connect(s_p_modbus) == -1 )
	{
		printf("failed to connect1:  %s\r\n", modbus_strerror(errno) );
		x200_close();
		return false;
	}
	return true;
}

void x200_close( void )
{
	modbus_free(s_p_modbus);
	s_p_modbus = NULL;
}

bool x200_set_freq( uint16_t hz10 )
{
    uint8_t	on;
    if( hz10 == 0 )
	{
		on = 0;
		if( modbus_write_bits(s_p_modbus, 0, 1, &on ) == -1 )
		{
			printf("failed to turn off:  %s\r\n", modbus_strerror(errno) );
			return false;
		}
	}
	else if( hz10 )
	{
		on = 1;
		if( modbus_write_bits(s_p_modbus, 0, 1, &on ) == -1 )
		{
			printf("failed to turn on:  %s\r\n", modbus_strerror(errno) );
			return false;
		}
	}
    usleep(5000);
	if( modbus_write_register( s_p_modbus, 1, hz10 ) == -1 )
	{
		printf("\r\nfailed to set speed\r\n", modbus_strerror(errno) );
		return false ;
	}
	return true;
}

bool x200_get_freq( uint16_t *hz10 )
{
	if( modbus_read_registers( s_p_modbus, 0x1001, 1, hz10 ) == -1 )
	{
		printf("\r\nfailed to read speed\r\n", modbus_strerror(errno) );
		return false ;
	}
	return true;
}