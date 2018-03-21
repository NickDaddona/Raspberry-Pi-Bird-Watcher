#ifndef _GROVE_WRAP_H_ // include guards
#define _GROVE_WRAP_H_

#include <stdint.h>  // used for definite integer types
#include <stdbool.h> // used for boolean data type
#include "grovepi.h" // include grovepi c++ library

extern "C" int digitalRead(uint8_t pin);

extern "C" short analogRead(uint8_t pin);

#endif // _GROVE_WRAP_H_