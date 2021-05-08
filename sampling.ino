#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "sample.hpp"
#include "sampling.hpp"
#include "regfile.hpp"
#include "pins.hpp"

/***********************************/
/**Master Sampling Variable Setup**/

void set_Master_Sampling_Rate(uint8_t sampling_rate){
  switch(sampling_rate){
    default:
    case 0:
      Master_Sampling_Rate = Sampling_slowest;
      break;
    case 1:
      Master_Sampling_Rate = Sampling_slower;
      break;
    case 2:
      Master_Sampling_Rate = Sampling_slow;
      break;
    case 3:
      Master_Sampling_Rate = Sampling_default;
      break;
    case 4:
      Master_Sampling_Rate = Sampling_fast;
      break;
    case 5:
      Master_Sampling_Rate = Sampling_faster;
      break;
    case 6:
      Master_Sampling_Rate = Sampling_fastest;
      break;
  }
}

void set_Master_Sampling_Count(uint8_t num_samples){
  Master_Sampling_Count = 16 + num_samples * 16; // This worked best but subject to change
  if(Master_Sampling_Count>127) Master_Sampling_Count=127;
}

/***********************************/
/**Static Functions**/

/* Empty an array of samples */
static void clean_samples(Sample* samples, uint16_t num_samples){
  for(uint16_t i = 0; i<MAX_NUM_SAMPLES; i++){
    samples[i] = 0;
  }
}

/* If there is room left to put another sample */
static bool has_sample_room(SampleRegister sr){
  return (sr.last_index < sr.num_samples);
}

/***********************************/
/**Function Definitions**/

/* Generates a new SampeRegister */
SampleRegister new_SampleRegister(uint8_t reg_id, uint16_t num_samples, uint16_t time_sampled){
  SampleRegister sr;
  
  sr.reg_id = reg_id;
  sr.num_samples = (num_samples <= MAX_NUM_SAMPLES) ? num_samples : MAX_NUM_SAMPLES; // So we don't overflow samples
  sr.time_sampled = time_sampled;
  
  sr.dirty = false; // Not used but could be :)
  sr.last_index = 0;
  
  clean_samples(sr.samples, sr.num_samples); // So we have a nice clean sample array
  
  return sr;
}

bool set_Sample(SampleRegister* sr, Sample s, uint8_t index){
  //if(!has_sample_room(*sr)) return false; // No more room --> false

  sr->samples[index] = s;
  sr->last_index = index;
  
  return true; // Proper assignment happened
}

void populate_SampleRegister_with_samples(uint8_t reg_id){
  pulse_trigger(); // Necessary to begin integration
  digitalWrite(MUX_ENABLE,HIGH);
  RegFile_set_num_samples_SampleRegister(reg_id, Master_Sampling_Count);
  RegFile_clear_SampleRegister(reg_id);
  for(uint8_t sr_i = 0; sr_i<Master_Sampling_Count; sr_i++){
    bool b = RegFile_write_SampleRegister(reg_id, sr_i);
    if(b==false){
      Serial.println("!! RegFile_write_SampleRegister ERROR");
      break;
    }
    
    delay(Master_Sampling_Rate*10);
  }
  digitalWrite(MUX_ENABLE,LOW);
}

/***********************************/
/**SampleRegister Plotting Methods**/

/* Find the lowest value in a SampleRegister */
static Sample min_Sample(SampleRegister sr){
  Sample lowest_value = 0;
  
  for(uint16_t i = 0; i<sr.num_samples; i++){
    Sample s = sr.samples[i];
    if(s<lowest_value) lowest_value = s;
  }
  
  return lowest_value;
}

/* Find the highest value in a SampleRegister */
static Sample max_Sample(SampleRegister sr){
  Sample highest_value = 0;
  
  for(uint16_t i = 0; i<sr.num_samples; i++){
    Sample s = sr.samples[i];
    if(s>highest_value) highest_value = s;
  }
  
  return highest_value;
}

#define Y_MAX_SCALE 10

void SampleRegister_plot(SampleRegister sr){
  Serial.println();
  uint16_t t_min = 0, t_max = sr.num_samples;
  if(t_max == 0){
    Serial.println("!! No data to plot");
    return;
  }
  Sample y_min = min_Sample(sr), y_max = max_Sample(sr); // Doesn't do anything rn

  double t_scale_double = 1.0;
  if(t_scale > 0) t_scale_double = 1.0/(double)(t_scale);
  else if(t_scale < 0) t_scale_double = (double)(-t_scale); 

  double y_scale_double = 1.0;
  if(y_scale > 0) y_scale_double = (double)(y_scale);
  else if(y_scale < 0) y_scale_double = 1.0/(double)(-y_scale);
  
  for(double i = t_min; i<t_max; i+=t_scale_double){
    uint8_t sample_index = (uint8_t)(i);
    Sample s = sr.samples[sample_index];
    s = (Sample)((double)(s) * y_scale_double)/Y_MAX_SCALE;
    if(s>200) s=200;
    char printfbuf[32];
    sprintf(printfbuf,"%4d|",sample_index);
    Serial.print(printfbuf);
    for(int16_t y = 0; y<s; y++){
      Serial.print(".");
    }Serial.println("#");
  }
}

/***********************************/
/** Functions for Testing Registers **/

bool FSM_print_SampleRegister(SampleRegister sr, bool reset_val){
  static enum PSR_State{
    st_PSR_Title,
    st_PSR_Line,
    st_PSR_End,
  } current_state = st_PSR_Title;

  static uint16_t line_counter = 0;
  
  if(reset_val){
    current_state = st_PSR_Title;
    line_counter = 0;
  }
  
  char printfbuf[64];
  switch(current_state){
    default:
    case st_PSR_Title:
      sprintf(printfbuf,"--SampleRegister[%d]: num_samples=%d,", sr.reg_id, sr.num_samples);
      Serial.println(printfbuf);
      sprintf(printfbuf," time_sampled=%d, is_dirty=%s, last_index=%d\n", sr.time_sampled, sr.dirty?"YES":"NO", sr.last_index);
      Serial.println(printfbuf);
      Serial.println("----");
      current_state = st_PSR_Line;
      break;
    case st_PSR_Line:
      sprintf(printfbuf,"%4d : %d",line_counter,sr.samples[line_counter]);
      Serial.println(printfbuf);
      line_counter++;
      if(line_counter>=sr.num_samples || line_counter>1024) current_state = st_PSR_End;
      break;
    case st_PSR_End:
      Serial.println("----");
      return true;
  }

  return false;
}
