#ifndef _GROVE_WRAP_H_ // include guards
#define _GROVE_WRAP_H_

#include <stdint.h>  // used for definite integer types
#include <stdbool.h> // used for boolean data type
#ifdef __cplusplus
#include "grovepi.h" // include grovepi c++ library in c++ code
#endif

#ifdef __cplusplus
extern "C" { // allow functions to be callable in C
#endif
        int initgrovepi();

        int digitalread(uint8_t pin);

        short analogread(uint8_t pin);

#ifdef __cplusplus
}
#endif // end extern C

#endif // end _GROVE_WRAP_H_