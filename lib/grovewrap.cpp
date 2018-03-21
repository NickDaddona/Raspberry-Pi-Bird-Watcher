#include "grovewrap.h" // include function declarations

/**
 * Created by Nicholas Daddona
 * 
 * This file provides wrapper functions so the grovepi c++ library functions
 * may be called within C
 * 
*/

//TODO: decide handling of exceptions possibilty propigating to C code

/**
 * Allows the reading of digital i/o ports on the grovepi
 * returns an integer so C types are ensured to be used
*/
extern "C" int digitalread(uint8_t pin)
{
        bool result = GrovePi::digitalRead(pin); // obtain the read result
        if (result == true) {
                return 1;
        }
        else {
                return 0;
        }
}

/**
 * Allows the reading of analog i/o ports on the grovepi
 */
extern "C" short analogwrite(uint8_t pin)
{
        return GrovePi::analogWrite(pin);
}