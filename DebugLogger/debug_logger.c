/*
 * DebugLogger.c
 *
 * Created: 10.11.2015 08:35:12
 * Author : Bernd
 */ 

#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>
#include "debug_logger.h"

static volatile char _log_buf[LOG_BUF_SIZE];
static volatile uint8_t _log_buf_head = 0;
static volatile uint8_t _log_buf_len = 0;

// we can't use cli()/sei() - called from inside ISR
static char _get_char_from_log_buf() {
	if (_log_buf_len == 0) {
		return 0;
	}
	char c = _log_buf[_log_buf_head];
	_log_buf_head++;
	_log_buf_len--;
	if (_log_buf_head >= LOG_BUF_SIZE) {
		_log_buf_head = 0;
	}
	return c;
}

// returns true if successful and false if buffer full
static bool _put_char_into_log_buf(char c) {
	cli();
	if (_log_buf_len >= LOG_BUF_SIZE) {
		sei();
		return false;
	}
	uint8_t i = _log_buf_head + _log_buf_len;
	if (i >= LOG_BUF_SIZE) {
		i -= LOG_BUF_SIZE;
	}
	_log_buf[i] = c;
	_log_buf_len++;
	sei();
	return true;
}

ISR(USART_UDRE_vect) {
	// if num char in buf <= 1 then disable this int
	// if char in buf -> get char from bug and put in UART
	char c = _get_char_from_log_buf();
	if (c) {
		UDR0 = c;
	} else {
		UCSR0B &= ~_BV(UDRIE0);
	}
}

void debug_log(char* str) {
	// put as many chars from str into buffer as fit
	for (int i = 0; str[i] != 0; i++)
	{
		if (!_put_char_into_log_buf(str[i])) {
			break;
		}
	}
	// if !(USART_UDRE enabled) then enabled it and put first char in UART
	if (!(UCSR0B & _BV(UDRIE0))) {
		cli();
		char c = _get_char_from_log_buf();
		if (c) {
			UCSR0B |= _BV(UDRIE0);
			UDR0 = c;
		}
		sei();		
	}
}

void init_debug_log() {
	UCSR0A = _BV(U2X0);
	UBRR0L = 8; //115200 baud - see page 189 of ATmega328 book (error is -3.5%!)
	UCSR0B = _BV(TXEN0); /* only tx enable */
}

