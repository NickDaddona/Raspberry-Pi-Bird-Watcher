#include <stdlib.h> // exit()
#include <stdio.h>  // printf()
#include <string.h> // memcpy()
#include <math.h>   // isnan macro
#include "../includes/sensors.h" // function declarations
#include "../lib/grovewrap.h"  // library wrapper functions

#define COMMAND_SIZE 128 // size of the buffer used to build the camera command

#define DHT_CMD 40 // Command for reaching the DHT sensor
#define DHT_ID   7 // DHT sensor connected to digital port 7
#define DHT_TYPE 0 // type of dht sensor, 0 for blue 1 for white
#define PIR_ID   8 // PIR sensor connected to digital port 8

// helper function
static int aregoodreadings(float temp, float humidity);

/**
 * Used to take pictures using the raspberry pi camera using specified command line
 * arguements that will be saved in a specified directory. If the string parameter that
 * contains the arguements is null, it is assumed no extra options are provided
 */
int takepic(char *options, char *savepath)
{
        // TODO: Test if directory exists and if it doesn't, create it
        char command[COMMAND_SIZE]; // char array to hold the built camera command
        int tail = 0; // tail of the command
        for (int i = 0; i < COMMAND_SIZE; i++) { // zero out array
                command[i] = '\0';
        }
        tail += sprintf(command + tail, "raspistill "); // print the command name to the string

        if (options != NULL) { // print the options to the command
                tail += sprintf(command + tail, options);
        }

        if (savepath == NULL) { // exit if no path specified
                fprintf(stderr, "No path specified for picture save location");
                exit(1);
        }
        else { // print the path to the command
                tail += sprintf(command + tail, "-o ");
                tail += sprintf(command + tail, savepath);
        }

        int result = system(command); // run the command to take a picture

        if (result == 64) { // check for errors
                fprintf(stderr, "Bad parameter passed to camera");
                exit(1);
        }
        if (result == 70) {
                fprintf(stderr, "Camera or software error");
                exit(1);
        }
        if (result == 130) {
                fprintf(stderr, "raspistill terminated by user");
                exit(1);
        }
        return 0;
}

/**
 * Initializes Connection with the grove pi, uses provided
 * wrapper function
 * 
 * program will exit if connection fails
 */
void initGrove(void)
{
        initgrovepi();
}

/**
 * Reads the current state of the motion sensor
 */
int readPIR(void)
{
        return digitalread(PIR_ID);
}

/**
 * Attempts to read the temperature and humidity from the dht probe,
 * program will exit if probe fails to return NaN values after 5 tries
 * 
 * Temperature is reported in Degrees Celsius and humidity as a percentage
 */
void safeDHTread(float *temp, float *humidity)
{
        int tries = 0;
        unsafeDHTread(temp, humidity); // inital read
        while (tries < 5 && (isnan(*temp) || isnan(*humidity)
            || !aregoodreadings(*temp, *humidity))) {
                tries++; // continue reading until good readings or max tries reached
                unsafeDHTread(temp, humidity);
        }
        if (isnan(*temp) || isnan(*humidity)) {
                printf("DHT NaN readings check port");
                exit(1);
        }
        if (!aregoodreadings(*temp, *humidity)) {
                printf("DHT bad readings check port");
                exit(1);
        }
}

/**
 * Reads the temperature and humidity from the dht probe. Not guaranteed
 * to return numerical values, (can return NaN). Use with caution.
 */
void unsafeDHTread(float *temp, float *humidity)
{
        writeblock(DHT_CMD, DHT_ID, DHT_TYPE, 0);
        readbyte();

        uint8_t buffer[33]; // buffer for reading from the sensor
        readblock(buffer);

        memcpy(temp,buffer + 1,sizeof(float));
        memcpy(humidity,buffer + 5,sizeof(float));
}

/**
 * Tests if the readings from the temperature probe are usuable
 */
int aregoodreadings(float temp, float humidity)
{
        return (temp > -100.0 && temp < 150.0 && humidity >= 0.0 && humidity <= 100.0);
}
