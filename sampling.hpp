#ifndef SAMPLING_H
#define SAMPLING_H

#include <stdint.h>
#include <stdbool.h>

#include "sample.hpp"

/***********************************/

#define MASTER_SAMPLING_COUNT_DEFAULT 64

enum Sampling_Rate{
  Sampling_slowest = 128,
  Sampling_slower = 64,
  Sampling_slow = 32,
  Sampling_default = 16,
  Sampling_fast = 8,
  Sampling_faster = 4,
  Sampling_fastest = 2
} Master_Sampling_Rate = Sampling_default; // Speed that the sampling function runs

uint8_t Master_Sampling_Count = MASTER_SAMPLING_COUNT_DEFAULT; // Number of sample registers to be filled

void set_Master_Sampling_Rate(uint8_t sampling_rate); // Modify Master_Sampling_Rate enum
void set_Master_Sampling_Count(uint8_t num_samples); // Modify Master_Sampling_Count

/***********************************/

SampleRegister new_SampleRegister(uint8_t reg_id, uint16_t num_samples, uint16_t time_sampled); // Constructor
bool set_Sample(SampleRegister* sr, Sample s, uint8_t index); // Set a sample within a GlobalRegister at index
void populate_SampleRegister_with_samples(uint8_t reg_id); // Load samples into SR from analog input

/***********************************/

/** TODO: ADD a method to modify these**/
int8_t t_scale = 1, y_scale = 1; // Global variables used for plotting

void SampleRegister_plot(SampleRegister sr); // Plot a sample register according to t_scale & y_scale

/***********************************/

bool FSM_print_SampleRegister(SampleRegister sr, bool reset_val);

#endif
