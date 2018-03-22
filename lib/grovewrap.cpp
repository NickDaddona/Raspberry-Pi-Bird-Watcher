#include <stdlib.h>
#include <stdio.h>
#include "grovewrap.h" // include function declarations

/**
 * Created by Nicholas Daddona
 * 
 * This file provides wrapper functions so the grovepi c++ library functions
 * may be called within C
 * 
*/
 
//TODO: decide handling of exceptions possibilty propigating to C code

extern "C" {

        /**
         * Establishes Connection with the GrovePi board so devices
         * may be read
         * Handles any exceptions the library may throw
         */
        int initgrovepi()
        {
                int success = 0;
                try {
                        GrovePi::initGrovePi(); // attempt to establish connection
                } catch (GrovePi::I2CError &e) {
                        printf("Connection with GrovePi board failed, Exiting\n");
                        success = 1;
                        exit(1);
                }
                printf("Grovepi Connection Established/n");
                return success;
        }

        /**
        * Allows the reading of digital i/o ports on the grovepi
        * returns an integer so C types are ensured to be used
        */
        int digitalread(uint8_t pin)
        {
                bool result = false; 
                try {
                        result = GrovePi::digitalRead(pin); // obtain the read result
                } catch (GrovePi::I2CError &e) {
                        printf("Error Communicating with device in port D%d, Exiting\n", pin);
                        printf("%s", e.detail());
                        exit(1);
                }
                return result ? 1 : 0; // test if will work, otherwise return an integer
        }

        /**
        * Allows the reading of analog i/o ports on the grovepi
        */
        short analogread(uint8_t pin)
        {
                short result = -1;
                try {
                        result = GrovePi::analogRead(pin); // attempt to read from the pin
                } catch (GrovePi::I2CError &e) {
                        printf("Error Communicating with device in port A%d, Exiting\n", pin);
                        printf("%s\n", e.detail());
                        exit(1);
                }
                return result;
        }
}