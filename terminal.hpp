/**

- terminal.hpp driver -

Binghamton University - Analog Computer
written by Jonah Hendler & Ashley Granieri

This driver defines functions for the Serial terminal
for the Arduino.

*/

#ifndef TERMINAL_H
#define TERMINAL_H

	#include <stdbool.h>
	#include "sampling.hpp"

	#define ARDUINO_BAUDRATE 115200 // 9600 // Variable but make sure PuTTY or other serial console is set up for same value
	#define TERMINAL_STR "BUAC>"

	/** Command Characters **/
	#ifndef COMMAND_CHARACTERS // Allows a user to modify these characters in another header file
		#define COMMAND_SAMPLE_RATE 's' // set the master sampling rate
		#define COMMAND_READ 'r' // read samples to sample register
		#define COMMAND_SAMPLE_NUM 'n' // set the master sampling count
		#define COMMAND_PLOT 'g' // plot a register in terminal
		#define COMMAND_PRINT_REG 'p' // print a register's contents
		#define COMMAND_CLEAR 'c' // empty a register of its contents
		#define COMMAND_INFO '?' // list stats (master_sampling_rate, master_sampling_count, etc.)
		#define COMMAND_HELP 'h' // list all commands available
		#define COMMAND_MUX_SELECT 'm' // Select which mux input to read from (0-F)
    #define COMMAND_TRIGGER 't' // Set the trigger
    #define COMMAND_TRIGGER_HIGH 'J' // Turn the trigger on
    #define COMMAND_TRIGGER_LOW 'j' // Turn the trigger off

    #define COMMAND_HIGH_BYTE 'B' // Set the high byte for the PWM value
    #define COMMAND_LOW_BYTE 'b' // Set the low byte for the PWM value
    /// Yes I know they're nibbles and not bytes >:(
		#define COMMAND_INITIAL_CONDITIONS 'N' // set the initial conditions using an 4-character command
		#define COMMAND_CONSTANT_MULTIPLIER 'M' // set the constant multiplier conditions using a 4-character command
    #define COMMAND_RESET_ALL_PINS 'R' // set all PWM pins back to 0V (50% duty cycle)
	#endif

	void Terminal_initialize();
	void Terminal_loop();

#endif
