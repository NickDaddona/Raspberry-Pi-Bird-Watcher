#ifndef _GROVE_WRAP_H_ // include guards
#define _GROVE_WRAP_H_

#include <stdint.h>  // used for definite integer types
#include <stdbool.h> // used for boolean data type
#ifdef __cplusplus
#include "grovepi.h" // include grovepi c++ library in c++ code
#endif

/**
 * Created by Nicholas Daddona
 * 
 * Contains function declarations for Grovepi library wrappers so that the C++
 * Grovepi library can be used within C. All Exceptions that may occur should be
 * handled in the definitions of these functions.
 */
#ifdef __cplusplus
extern "C" { // allow functions to be callable in C
#endif
        void initgrovepi();

        void writeblock(uint8_t command, uint8_t pinnumber, uint8_t opt1, uint8_t opt2);
        
        uint8_t readblock(uint8_t *datablock);

        uint8_t readbyte();

        int digitalread(uint8_t pin);

        short analogread(uint8_t pin);
#ifdef __cplusplus
}
#endif // end extern C
#endif // end _GROVE_WRAP_H_