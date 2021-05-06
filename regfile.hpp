/**

- regfile.hpp driver -

Binghamton University - Analog Computer
written by Jonah Hendler & Ashley Granieri

Provides an internal storage for samples as described in sample.hpp
as well as functions to interact with said data.

*/

#ifndef REGFILE_H
#define REGFILE_H

	#include <stdint.h>
	#include <stdbool.h>

	#include "sample.hpp"
	#include "sampling.hpp"

	// Some briefer macros
	#define GlobalRegister(x) GlobalRegFile.registers[x]
	#define GlobalRegisterCount GlobalRegFile.num_registers

	#define MAX_NUM_REGISTERS 8 // How many SampleRegisters we can have in the RegFile

	/** RegFile Structure **/
	typedef struct RegFile{

		uint8_t num_registers; // How many SampleRegisters we actually have initialized
		SampleRegister registers[MAX_NUM_REGISTERS]; // Array of SampleRegisters

	} RegFile;

	RegFile GlobalRegFile; // The main RegFile for the entire Analog Computer

	/** Function Declarations **/
	void RegFile_initialize(); // Initialize all SRs in the GlobalRegFile
	void RegFile_clear_SampleRegister(uint8_t reg_id); // Clear the contents of an SR in the RegFile
	SampleRegister RegFile_read_SampleRegister(uint8_t reg_id); // Read an SR from the RegFile
	bool RegFile_write_SampleRegister(uint8_t reg_id, Sample read_sample, uint8_t index); // Write a value to an SR from the RegFile
	void RegFile_set_num_samples_SampleRegister(uint8_t reg_id, uint8_t num_samples); // Sets the number of samples stored in an SR in the RegFile
#endif
