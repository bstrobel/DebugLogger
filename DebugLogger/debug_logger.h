/*
 * debug_logger.h
 *
 * Created: 10.11.2015 08:50:08
 *  Author: Bernd
 */ 


#ifndef DEBUG_LOGGER_H_
#define DEBUG_LOGGER_H_

#define LOG_BUF_SIZE 512
// user settings
#ifndef LOG_BUF_SIZE
#	define LOG_BUF_SIZE 50
#endif

#ifndef F_CPU
#	define F_CPU 8000000UL
#endif

void debug_log(char* str);
void init_debug_log();

#endif /* DEBUG_LOGGER_H_ */