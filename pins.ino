/**

- pins.cpp driver -

Binghamton University - Analog Computer
written by Jonah Hendler & Ashley Granieri

*/

#include "sample.hpp"
#include "pins.hpp"

void Arduino_IO_initialize(){
	/** PWM Outputs **/ // Capable of values 0-1023
	// Used for the Initial Conditions
	pinMode(PWM0, OUTPUT);
	pinMode(PWM1, OUTPUT);
	pinMode(PWM2, OUTPUT);
	pinMode(PWM3, OUTPUT);
	pinMode(PWM4, OUTPUT);
	pinMode(PWM5, OUTPUT);
	
	// Used for the Constant Multipliers
	pinMode(PWM6, OUTPUT);
	pinMode(PWM7, OUTPUT);
	pinMode(PWM8, OUTPUT);
	pinMode(PWM9, OUTPUT);
	pinMode(PWM10, OUTPUT);
	pinMode(PWM11, OUTPUT);

  // Set PWM pins to higher frequencies
  TCCR4B = TCCR4B & B11111000 | B00000001;   //  D6, D7, D8 - for PWM frequency of 31372.55 Hz
  TCCR3B = TCCR3B & B11111000 | B00000001;  // D2, D3, D5 - for PWM frequency of 3921.16 Hz
  TCCR2B = TCCR2B & B11111000 | B00000001;  // D9, D10 - for  PWM frequency of 3921.16 Hz
  TCCR1B = TCCR1B & B11111000 | B00000001;  // D11, D12 - for PWM frequency of 3921.16 Hz
	TCCR0B = TCCR0B & B11111000 | B00000001; // D4, D13 - for PWM frequency of 7812.50 Hz

	/** MUX Select Inputs **/ // Allows access to 16 different MUX channels
	pinMode(MUX_SELECT_0, OUTPUT);
	pinMode(MUX_SELECT_1, OUTPUT);
	pinMode(MUX_SELECT_2, OUTPUT);
	pinMode(MUX_SELECT_3, OUTPUT);
	
	/** MUX Data Output **/ // Capable of values 0-255
	pinMode(MUX_DATA_IN, INPUT); // Analog MUX output to Arduino
	
	/** Other **/
	pinMode(MUX_ENABLE, OUTPUT); // MUX enable signal
	pinMode(MUX_RESET, OUTPUT); // MUX reset
	pinMode(TRIGGER, OUTPUT); // Start integrators & other components

  /** Initialize Values **/
  analogWrite(PWM0,PWM_ZERO_VOLTS);
  analogWrite(PWM1,PWM_ZERO_VOLTS);
  analogWrite(PWM2,PWM_ZERO_VOLTS);
  analogWrite(PWM3,PWM_ZERO_VOLTS);
  analogWrite(PWM4,PWM_ZERO_VOLTS);
  analogWrite(PWM5,PWM_ZERO_VOLTS);
  analogWrite(PWM6,PWM_ZERO_VOLTS);
  analogWrite(PWM7,PWM_ZERO_VOLTS);
  analogWrite(PWM8,PWM_ZERO_VOLTS);
  analogWrite(PWM9,PWM_ZERO_VOLTS);
  analogWrite(PWM10,PWM_ZERO_VOLTS);
  analogWrite(PWM11,PWM_ZERO_VOLTS);
  digitalWrite(MUX_SELECT_0,LOW);
  digitalWrite(MUX_SELECT_1,LOW);
  digitalWrite(MUX_SELECT_2,LOW);
  digitalWrite(MUX_SELECT_3,LOW);
  digitalWrite(MUX_ENABLE,LOW);
  digitalWrite(MUX_RESET,HIGH);
  digitalWrite(TRIGGER,LOW);
}

void turn_on_trigger(){
	digitalWrite(TRIGGER, HIGH);
}
void turn_off_trigger(){
	digitalWrite(TRIGGER, LOW);
}

#ifndef TRIGGER_PULSE_DELAY
	#define TRIGGER_PULSE_DELAY 1000
#endif
void pulse_trigger(){
	turn_on_trigger();
	delay(TRIGGER_PULSE_DELAY);
	turn_off_trigger();	
}

void mux_select(uint8_t code){
	// This isolates which bits in the 4-bit code are on vs. off
	bool bit0 = (code & 0b00000001);
	bool bit1 = (code & 0b00000010);
	bool bit2 = (code & 0b00000100);
	bool bit3 = (code & 0b00001000);
	
	/*Serial.print("MUX_SELECT=");
	Serial.print(bit3);
	Serial.print(bit2);
	Serial.print(bit1);
	Serial.println(bit0);*/
	
	// Converts from true/false to HIGH/LOW (idk if this is necessary?)
	digitalWrite(MUX_SELECT_0, HIGH ? bit0 : LOW);
	digitalWrite(MUX_SELECT_1, HIGH ? bit1 : LOW);
	digitalWrite(MUX_SELECT_2, HIGH ? bit2 : LOW);
	digitalWrite(MUX_SELECT_3, HIGH ? bit3 : LOW);
}

#ifndef ADC_SCALING_FACTOR
	#define ADC_SCALING_FACTOR 1
#endif
Sample read_Sample_from_ADC(){
	return analogRead(MUX_DATA_IN)/ADC_SCALING_FACTOR;
}

// 0 - always off, 255 - always on
static void set_pwm_pin(uint8_t number, uint8_t pwm_value){
  switch(number){
    case 0: analogWrite(PWM0, pwm_value); break;
    case 1: analogWrite(PWM1, pwm_value); break;
    case 2: analogWrite(PWM2, pwm_value); break;
    case 3: analogWrite(PWM3, pwm_value); break;
    case 4: analogWrite(PWM4, pwm_value); break;
    case 5: analogWrite(PWM5, pwm_value); break;
    case 6: analogWrite(PWM6, pwm_value); break;
    case 7: analogWrite(PWM7, pwm_value); break;
    case 8: analogWrite(PWM8, pwm_value); break;
    case 9: analogWrite(PWM9, pwm_value); break;
    case 10: analogWrite(PWM10, pwm_value); break;
    case 11: analogWrite(PWM11, pwm_value); break;
    default: break;
  }
}

void set_initial_condition(uint8_t number, uint8_t pwm_value){
  if(number>5) number = 0;
  set_pwm_pin(number, pwm_value);
}
void set_constant_coefficient(uint8_t number, uint8_t pwm_value){
  uint8_t new_pin_number = number+6; // b/c 6 initial conditions first
  if(number>11) number = 6;
  set_pwm_pin(new_pin_number, pwm_value);
}
void off_initial_condition(uint8_t number){
  set_initial_condition(number,0);
}
void off_constant_coefficient(uint8_t number){
  set_constant_coefficient(number,0);
}

void set_high_byte(uint8_t high_byte){
  PWM_value &= 0x0F;
  PWM_value |= (0x0F & high_byte)<<4;
}
void set_low_byte(uint8_t low_byte){
  PWM_value &= 0xF0;
  PWM_value |= (0x0F & low_byte);
}
