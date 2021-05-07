#include "sample.hpp"
#include "sampling.hpp"
#include "terminal.hpp"
#include "pins.hpp"

/***********************************/
/** Terminal **/

static bool is_command_char(uint8_t c){ // 1 to 1 correspondence with 2-Character COMMAND_CHARACTERS
  return (
    c==COMMAND_SAMPLE_RATE ||
    c==COMMAND_READ ||
    c==COMMAND_SAMPLE_NUM ||
    c==COMMAND_PLOT ||
    c==COMMAND_PRINT_REG ||
    c==COMMAND_CLEAR ||
    c==COMMAND_INFO ||
    c==COMMAND_HELP ||
    c==COMMAND_MUX_SELECT ||
    c==COMMAND_HIGH_BYTE ||
    c==COMMAND_LOW_BYTE ||
    c==COMMAND_INITIAL_CONDITIONS ||
    c==COMMAND_CONSTANT_MULTIPLIER ||
    c==COMMAND_TRIGGER ||
    c==COMMAND_TRIGGER_HIGH ||
    c==COMMAND_TRIGGER_LOW ||
    c==COMMAND_RESET_ALL_PINS
    );
}

static void print_command_list(){
	Serial.println("-------------------");
	Serial.println("Available commands");
	Serial.println("s - set the master (s)ampling rate");
	Serial.println("r - (r)ead samples to sample register");
	Serial.println("n - set the master sampling cou(n)t");
	Serial.println("g - plot a register in terminal");
	Serial.println("p - (p)rint a register\'s contents");
	Serial.println("c - (c)lear a register");
	Serial.println("? - list global stats");
	Serial.println("h - (h)elpful list all commands available");
	Serial.println("m - set (m)ux inputs");
  Serial.println("t - pulse the (t)rigger");
	Serial.println("-------------------");	
	Serial.println("b - set the low (b)yte for ICs / CMs");
  Serial.println("B - set the high (B)yte for ICs / CMs");
	Serial.println("N - set the i(N)itial conditions");
	Serial.println("M - set the constant (M)ultiplier");
  Serial.println("R - (R)eset all PWM pins to 50% duty cylce (0V)");
	Serial.println("-------------------");
}

static bool is_digit_char(uint8_t c){
  return (c>='0' && c<='9');
}

static bool is_hex_char(uint8_t c){
  return (is_digit_char(c) || (c>='a' && c<='f'));
}

static uint8_t hex2dec(char c){
  if(!is_hex_char(c)) return 0;
  if(is_digit_char(c)) return c-'0';
  else return c-'a'+10;
}

/*
 * 255 = -15V
 * 128 = 0V
 * 0 = 15V
 */

// FSM for the keyboard inputs (this is the meat and potatoes)
void FSM_Serial_Control(uint8_t char_read){
  static enum state{
    st_Wait_For_Char, // Wait until we get a valid char
    st_Evaluate_Char, // Evaluate a command
    st_Wait_For_Num, // Wait for digit
    st_Newline, // Print a new line
  } current_state = st_Newline;

  static bool fsm_reseter = true; // Reset other FSMs back to their initial states

  static uint8_t command_char = 0; // The initial character read --> determines the command to be sent
  static uint8_t num_char = 0; // The parameter for the command to be sent

  switch(current_state){
    default:
    case st_Wait_For_Char:
      if(is_command_char(char_read)){
        current_state = st_Wait_For_Num;
        command_char = char_read;
        fsm_reseter = true;
        Serial.print((char)(command_char));        
      }
      else
        current_state = st_Wait_For_Char;
      break;
      
    case st_Wait_For_Num:
      if(is_hex_char(char_read)){
        num_char = char_read;
        Serial.print((char)(num_char));
        current_state = st_Evaluate_Char;
      }
      else
        current_state = st_Wait_For_Num;
      break;

    case st_Evaluate_Char:
      uint8_t reg_id = hex2dec(num_char);
      if(command_char == COMMAND_SAMPLE_RATE){
        Serial.print("SAMPLE RATE");
        set_Master_Sampling_Rate(reg_id);
        current_state = st_Newline;
      }
      else if (command_char == COMMAND_READ){
        Serial.println("READ");
        populate_SampleRegister_with_samples(reg_id);
        current_state = st_Newline;
      }
      else if (command_char == COMMAND_SAMPLE_NUM){
        Serial.println("SAMPLE NUM");
        set_Master_Sampling_Count(reg_id);
        current_state = st_Newline;
      }
      else if (command_char == COMMAND_CLEAR){ // WORKS
        //Serial.println("CLEAR");
        RegFile_clear_SampleRegister(reg_id);
        current_state = st_Newline;
      }
      else if (command_char == COMMAND_PLOT){ // WORKS
        //Serial.println("PLOT");
        SampleRegister_plot(GlobalRegFile.registers[reg_id]);
        current_state = st_Newline;
      }
      else if (command_char == COMMAND_PRINT_REG){ // Fully Works!
        bool psr_done = FSM_print_SampleRegister(GlobalRegFile.registers[reg_id], fsm_reseter);
        if(psr_done){
          current_state = st_Newline;
        }
        else{
          current_state = st_Evaluate_Char;
        }
        
        fsm_reseter = false; 
      }
      else if (command_char == COMMAND_INFO){
        Serial.print("MASTER_SAMPLING_RATE=");
        Serial.println(Master_Sampling_Rate);
        Serial.print("MASTER_SAMPLING_COUNT=");
        Serial.println(Master_Sampling_Count);
        Serial.print("PWM_VALUE=");
        Serial.print(PWM_value);
        Serial.print("PWM VOLTAGE~=");
        Serial.println(PWM2Voltage(PWM_value));
        print_selected_mux();
        current_state = st_Newline;
      }
      else if (command_char == COMMAND_HELP){
        Serial.println("HELP");
        current_state = st_Newline;
      }
      else if (command_char == COMMAND_MUX_SELECT){
        Serial.println("MUX_SELECT");
        mux_select(reg_id);        
        current_state = st_Newline;
      }
      else if (command_char == COMMAND_HIGH_BYTE){
        Serial.println("HIGH_BYTE");
        set_high_byte(reg_id);
        current_state = st_Newline;
      }
      else if (command_char == COMMAND_LOW_BYTE){
        Serial.println("LOW_BYTE");
        set_low_byte(reg_id);
        current_state = st_Newline;
      }
      else if (command_char == COMMAND_INITIAL_CONDITIONS){
        Serial.println("INIT_COND");
        set_initial_condition(reg_id, PWM_value);
        current_state = st_Newline;
      }
      else if (command_char == COMMAND_CONSTANT_MULTIPLIER){
        Serial.println("CONST_MULT");
        set_constant_coefficient(reg_id, PWM_value);
        current_state = st_Newline;
      }
      else if(command_char == COMMAND_TRIGGER){
        Serial.println("TRIGGER");
        pulse_trigger();
        current_state = st_Newline;
      }
      else if(command_char == COMMAND_TRIGGER_HIGH){
        Serial.println("TRIGGERHIGH");
        turn_on_trigger();
        current_state = st_Newline;
      }
      else if(command_char == COMMAND_TRIGGER_LOW){
        Serial.println("TRIGGERLOW");
        turn_off_trigger();
        current_state = st_Newline;
      }
      else if(command_char == COMMAND_RESET_ALL_PINS){
        Serial.println("RESETALLPWM");
        reset_pwm_pins();
        current_state = st_Newline;
      }
      break;
      

    case st_Newline:
      Serial.println("\n\r");
      //Serial.print(TERMINAL_STR);
      current_state = st_Wait_For_Char;
      break;
  }
}

/** Terminal Methods **/

void Terminal_initialize(){
  Serial.begin(ARDUINO_BAUDRATE); // Allows all Serial functions to work
}

void Terminal_loop(){
  char char_read = Serial.read();
  //if(is_hex_char(char_read) || (char_read>='a' && char_read<='z') || (char_read>='A' && char_read<='Z') || char_read=='?') // Prevents miscellaneous chars
  FSM_Serial_Control(char_read);
}
