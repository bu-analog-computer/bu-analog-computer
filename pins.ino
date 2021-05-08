/**

- pins.cpp driver -

Binghamton University - Analog Computer
written by Jonah Hendler & Ashley Granieri

*/

#include "sample.hpp"
#include "pins.hpp"

#ifndef PWM_ZERO_VOLTS
  #define PWM_ZERO_VOLTS 128 // Because PWM 128 = 50% duty cycle = 0V
#endif
void reset_pwm_pins(){
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
}

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
  reset_pwm_pins();
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
	#define TRIGGER_PULSE_DELAY 100
#endif
void pulse_trigger(){
	turn_on_trigger();
	delay(TRIGGER_PULSE_DELAY);
	turn_off_trigger();	
}

void print_selected_mux(){
  Serial.print("MUX_SELECT=");
  Serial.print(bit3);
  Serial.print(bit2);
  Serial.print(bit1);
  Serial.println(bit0);
}

void mux_select(uint8_t code){
	// This isolates which bits in the 4-bit code are on vs. off
	bit0 = (code & 0b00000001)>>0;
	bit1 = (code & 0b00000010)>>1;
	bit2 = (code & 0b00000100)>>2;
	bit3 = (code & 0b00001000)>>3;
	
  print_selected_mux();
	
	// Converts from true/false to HIGH/LOW (idk if this is necessary?)
	digitalWrite(MUX_SELECT_0, HIGH ? bit0 : LOW);
	digitalWrite(MUX_SELECT_1, HIGH ? bit1 : LOW);
	digitalWrite(MUX_SELECT_2, HIGH ? bit2 : LOW);
	digitalWrite(MUX_SELECT_3, HIGH ? bit3 : LOW);
}

#ifndef ADC_SCALING_FACTOR
	#define ADC_SCALING_FACTOR 64
#endif
Sample read_Sample_from_ADC(){
  uint16_t mux_data = analogRead(MUX_DATA_IN);
  if(mux_data<124) mux_data = 124;
  else if(mux_data>961) mux_data = 961;
  double mux_data_double = (double)(analogRead(MUX_DATA_IN));
  double real_value = mux_data_double * 1.22342 - 151.704;
  if(real_value<0) real_value = 0;
  else if(real_value>1024) real_value = 1024;
  return (Sample)(real_value);
	//return analogRead(MUX_DATA_IN)/ADC_SCALING_FACTOR;
}

// 0 - always off, 255 - always on
static void set_ic_pin(uint8_t number, uint8_t pwm_value){
  switch(number){
    // Integrators
    case 1: analogWrite(PWM0, pwm_value); break;
    case 2: analogWrite(PWM1, pwm_value); break;
    case 3: analogWrite(PWM2, pwm_value); break;
    case 4: analogWrite(PWM3, pwm_value); break;
    case 5: analogWrite(PWM4, pwm_value); break;
    case 6: analogWrite(PWM5, pwm_value); break;
    default: break;
  }
}

static void set_cm_pin(uint8_t number, uint8_t pwm_value){
  switch(number){
    // Constant Multipliers
    case 1: analogWrite(PWM6, pwm_value); break;
    case 2: analogWrite(PWM7, pwm_value); break;
    case 3: analogWrite(PWM8, pwm_value); break;
    case 4: analogWrite(PWM9, pwm_value); break;
    case 5: analogWrite(PWM10, pwm_value); break;
    case 6: analogWrite(PWM11, pwm_value); break;
    default: break;
  }
}

void set_initial_condition(uint8_t number, uint8_t pwm_value){
  if(number<1 || number>6) return; // Error prevention
  set_ic_pin(number, pwm_value);
  Serial.print("Integrator [");
  Serial.print(number);
  Serial.print("] <= ");
  Serial.print(pwm_value);
  Serial.print(" ~= ");
  Serial.print(PWM2Voltage(pwm_value));
  Serial.println("V");
}
void set_constant_coefficient(uint8_t number, uint8_t pwm_value){
  if(number<1 || number>6) return; // Error prevention
  set_cm_pin(number, pwm_value);
  Serial.print("Const Mult [");
  Serial.print(number);
  Serial.print("] <= ");
  Serial.print(pwm_value);
  Serial.print(" ~= ");
  Serial.print(PWM2Voltage(pwm_value));
  Serial.println("V");

}
void off_initial_condition(uint8_t number){
  set_initial_condition(number,0);
}
void off_constant_coefficient(uint8_t number){
  set_constant_coefficient(number,0);
}

#ifndef BIT_MASKS
  #define LOW_BIT_MASK 0xF0
  #define HIGH_BIT_MASK 0x0F
#endif
void set_high_byte(uint8_t high_byte){
  PWM_value &= HIGH_BIT_MASK;
  PWM_value |= (HIGH_BIT_MASK & high_byte)<<4;
}
void set_low_byte(uint8_t low_byte){
  PWM_value &= LOW_BIT_MASK;
  PWM_value |= (LOW_BIT_MASK & low_byte);
}


int8_t PWM2Voltage(uint8_t pwm_value){ // Very rough approximation of PWM to Voltage reading
  uint8_t high_nibble = (pwm_value&0xF0)>>4;
  switch(high_nibble){
    default: return 99; // How you know you messed up
    case 0: return 15;
    case 1: return 14;
    case 2: return 12;
    case 3: return 10;
    case 4: return 8;
    case 5: return 6;
    case 6: return 4;
    case 7: return 2;
    case 8: return 0;
    case 9: return -2;
    case 10: return -4;
    case 11: return -6;
    case 12: return -8;
    case 13: return -11;
    case 14: return -13;
    case 15: return -15;
  }
}
