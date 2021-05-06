#include "regfile.hpp"
#include "terminal.hpp"
#include "pins.hpp"

void setup() {	
  Arduino_IO_initialize();
  Terminal_initialize();
  RegFile_initialize();
  
  Serial.println("---- BU ANALOG COMPUTER STARTING UP ----");
}


void loop() {
  Terminal_loop();
}
