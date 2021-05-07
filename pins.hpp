/**

- pins.hpp driver -

Binghamton University - Analog Computer
written by Jonah Hendler & Ashley Granieri

This driver lists all necessary I/O for the Arduino Mega
to communicate with the plugboard.

*/

#include "sample.hpp"

#ifndef PINS_H
#define PINS_H

	/** PWM Outputs **/ // Capable of values 0-128-255 (+15V - 0 - -15V)

	// Used for the Initial Conditions
	#define PWM0 2 // Int1
	#define PWM1 12 // Int2
	#define PWM2 3 // Int3
	#define PWM3 13 // Int4
	#define PWM4 4 // Int5
	#define PWM5 11 // Int6
	
	// Used for the Constant Multipliers
	#define PWM6 5 // CM1
	#define PWM7 10 // CM2
	#define PWM8 6 // CM3
	#define PWM9 9 // CM4
	#define PWM10 7 // CM5
	#define PWM11 8 // CM6

	/** MUX Select Inputs **/ // Allows access to 16 different MUX channels
	#define MUX_SELECT_0 42
	#define MUX_SELECT_1 44
	#define MUX_SELECT_2 46
	#define MUX_SELECT_3 48
	
	/** MUX Data Output **/ // Capable of values 0-255
	#define MUX_DATA_IN A1 // Analog MUX output to Arduino
	
	/** Other **/
	#define MUX_ENABLE 40 // MUX enable signal
	#define MUX_RESET 50 // MUX reset (active LOW)
	#define TRIGGER 52 // Start integrators & other components
	
	/** Function Declarations **/
	void Arduino_IO_initialize(); // Initialize all pins above as inputs/outputs
  void reset_pwm_pins(); // Sets all PWM pins to 50% duty cycle (0V)
	void turn_on_trigger(); // Turn on the integration trigger signal
	void turn_off_trigger(); // Turn off the integration trigger signal
	void pulse_trigger(); // Gives us a rapid pulse for the trigger to initialize the integrators
	void print_selected_mux();
	void mux_select(uint8_t code); // Control the 4-bit MUX select signal with a 4-bit value
	Sample read_Sample_from_ADC(); // Reads the 12-bit value in the ADC

  uint8_t bit0, bit1, bit2, bit3;
  uint8_t PWM_value;

  void set_initial_condition(uint8_t number, uint8_t pwm_value);
  void set_constant_coefficient(uint8_t number, uint8_t pwm_value);
  void off_initial_condition(uint8_t number);
  void off_constant_coefficient(uint8_t number);
  void set_high_byte(uint8_t high_byte);
  void set_low_byte(uint8_t low_byte);
  int8_t pwm_to_voltage(uint8_t pwm_value);

#endif
