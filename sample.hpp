#ifndef SAMPLE_H
#define SAMPLE_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_NUM_SAMPLES 128 // Maximum amount of samples a register can hold

typedef uint16_t Sample; // Needs to hold a 12-bit value from the Arduino's ADC

typedef struct SampleRegister{
  bool dirty : 1;   // If this SampleRegister has been modified (CURRENTLY NOT USED)
  uint8_t reg_id : 7; // The ID of the individual sample register (CURRENTLY NOT USED)
  
  uint16_t last_index;  // Last index we put a sample in (CURRENTLY NOT USED)
  uint16_t num_samples; // How many samples we have (CURRENTLY NOT USED)
  uint16_t time_sampled;  // How much time was sampled, in ms (up to ~65 seconds) (CURRENTLY NOT USED)
  
  Sample samples[MAX_NUM_SAMPLES]; // An array of samples
} SampleRegister;

#endif
