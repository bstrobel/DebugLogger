/*
 * DebugLoggerItest.c
 *
 * Created: 10.11.2015 08:38:38
 * Author : Bernd
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "../DebugLogger/debug_logger.h"


int main(void)
{
	init_debug_log();
    /* Replace with your application code */
    while (1) 
    {
		debug_log("---------\r\n");
		_delay_ms(1000);
		debug_log("a123456789b123456789c123456789d1234567890123456789\r\n");
		_delay_ms(1000);
    }
}

