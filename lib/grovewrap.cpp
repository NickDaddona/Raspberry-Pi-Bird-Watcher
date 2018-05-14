#include <stdlib.h>
#include <stdio.h>
#include "grovepi.h"   // include grovepi library
#include "grovewrap.h" // include function declarations

extern "C" {
        #include "../includes/util.h" // access to cleanup()
}

/**
 * Created by Nicholas Daddona
 * 
 * This file provides wrapper functions so the grovepi c++ library functions
 * may be called within C. Any exceptions that can be thrown by the library are
 * handled before they are allowed to propigate into the C portion of the program.
 * 
 * As of 2018-03-25, the program will exit when an exception is thrown as this
 * usually involves some kind of connection error to the Grovepi board or any of
 * its sensors. Five attempts are currently allowed for these operations.
 * 
*/

extern "C" {

        /**
         * Establishes Connection with the GrovePi board so devices may be read
         * Wrapper function for GrovePi::initGrovePi()
         */
        void initgrovepi()
        {
                try {
                        GrovePi::initGrovePi(); // attempt to establish connection
                } catch (GrovePi::I2CError &e) {
                        printf("Connection with GrovePi board failed, Exiting\n");
                        cleanup(); // cleanup before exiting
                        exit(EXIT_FAILURE); // exit after failure
                }
        }

        /**
         * Writes a block of data to the slave device
         * Wrapper function for GrovePi::writeBlock 
         */
        void writeblock(uint8_t command, uint8_t pinnumber, uint8_t opt1, uint8_t opt2)
        {
                try { // attempt write
                        GrovePi::writeBlock(command, pinnumber, opt1, opt2);
                } catch (GrovePi::I2CError &e) { // exit on failure
                        printf("Writeblock Error \n%s", e.detail());
                        cleanup(); // cleanup before exiting
                        exit(EXIT_FAILURE); // exit after failure
                }
        }

        /**
         * Reads a block of data from the slave device
         * Wrapper function for GrovePi::readBlock 
         * returns the number of bytes read
         */
        uint8_t readblock(uint8_t *datablock)
        {
                uint8_t bytesread = 0;
                try {
                        bytesread = GrovePi::readBlock(datablock); // attempt read
                } catch (GrovePi::I2CError &e) { // exit on failure
                        printf("Readblock Error \n%s", e.detail());
                        cleanup(); // cleanup before exiting
                        exit(EXIT_FAILURE); // exit after failure
                }
                return bytesread;
        }

        /**
         * Reads a byte from the slave device
         * Wrapper function for GrovePi::readbyte()
         * returns the value read from the slave device
         */
        uint8_t readbyte()
        {
                uint8_t outputcode = 0;
                try {
                        outputcode = GrovePi::readByte(); // read a byte
                } catch (GrovePi::I2CError &e) {
                        printf("Error Reading byte \n%s", e.detail());
                        cleanup(); // cleanup before exiting
                        exit(EXIT_FAILURE); // exit after failure
                }
                return outputcode;
        }

        /**
        * Allows the reading of digital i/o ports on the grovepi returns an
        * integer so C types are ensured to be used
        * Wrapper for GrovePi::digitalRead()
        */
        int digitalread(uint8_t pin)
        {
                bool result = false; 
                try {
                        result = GrovePi::digitalRead(pin); // obtain the read result
                } catch (GrovePi::I2CError &e) {
                        printf("Error Communicating with device in port D%d, Exiting\n", pin);
                        printf("%s", e.detail());
                        cleanup(); // cleanup before exiting
                        exit(EXIT_FAILURE); // exit after failure
                }
                return result ? 1 : 0; // test if will work, otherwise return an integer
        }

        /**
        * Allows the reading of analog i/o ports on the grovepi
        * Wrapper for GrovePi::analogRead()
        */
        short analogread(uint8_t pin)
        {
                short result = -1;
                try {
                        result = GrovePi::analogRead(pin); // attempt to read from the pin
                } catch (GrovePi::I2CError &e) {
                        printf("Error Communicating with device in port A%d, Exiting\n", pin);
                        printf("%s\n", e.detail());
                        cleanup(); // cleanup before exiting
                        exit(EXIT_FAILURE); // exit after failure
                }
                return result;
        }
}