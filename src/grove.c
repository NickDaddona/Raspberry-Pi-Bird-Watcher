#include <stdlib.h> // exit()
#include <stdio.h>  // printf()
#include <string.h> // memcpy()
#include <math.h>   // isnan macro
#include "../includes/grove.h" // function declarations
#include "../lib/grovewrap.h"  // library wrapper functions

#define DHT_CMD 40 // Command for reaching the DHT sensor
#define DHT_ID   7 // DHT sensor connected to digital port 7
#define DHT_TYPE 0 // type of dht sensor, 0 for blue 1 for white
#define PIR_ID   8 // PIR sensor connected to digital port 8

// helper function
static int aregoodreadings(float temp, float humidity);

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
        return (temp > -100 && temp < 150.0 && humidity >= 0.0 && humidity <= 100.0);
}