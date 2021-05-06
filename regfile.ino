#include <stdint.h>
#include <stdbool.h>

#include "sampling.hpp"
#include "sample.hpp"
#include "regfile.hpp"

static bool RegFile_valid_register(uint8_t reg_id){ // Checks if this SampleRegister exists
  return (reg_id < GlobalRegisterCount); 
}

void RegFile_initialize(){
  GlobalRegisterCount = MAX_NUM_REGISTERS;
  
  for(int i = 0; i<MAX_NUM_REGISTERS; i++){ // Initialize all the individual registers
    GlobalRegister(i) = new_SampleRegister(i, 0, 0);
  }
}

void RegFile_clear_SampleRegister(uint8_t reg_id){
  for(uint8_t i = 0; i<MAX_NUM_SAMPLES; i++){
    GlobalRegister(reg_id).samples[i] = 0; 
  }
}

SampleRegister RegFile_read_SampleRegister(uint8_t reg_id){
  if(!RegFile_valid_register(reg_id)) reg_id = 0;
  return GlobalRegister(reg_id);
}

bool RegFile_write_SampleRegister(uint8_t reg_id, uint8_t index){
  if(!RegFile_valid_register(reg_id)) return false;
  Sample read_sample = read_Sample_from_ADC(); // Grab sample from pins
  Serial.print("SAMP=");
  Serial.println(read_sample);
  bool result = set_Sample(&GlobalRegister(reg_id), read_sample, index);
  return result;
}

void RegFile_set_num_samples_SampleRegister(uint8_t reg_id, uint8_t num_samples){ // Sets the number of registers in a SampleRegister
  GlobalRegister(reg_id).num_samples = num_samples;
}
