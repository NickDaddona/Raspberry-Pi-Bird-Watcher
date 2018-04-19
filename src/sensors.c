#include <stdlib.h> // exit()
#include <stdio.h>  // printf()
#include <string.h> // memcpy()
#include <math.h>   // isnan macro
#include <time.h>   // localtime(), strftime()
#include "../includes/sensors.h" // function declarations
#include "../includes/util.h"   // utility functions
#include "../lib/grovewrap.h"  // library wrapper functions

#define COMMAND_SIZE 128 // size of the buffer used to build the camera command

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
 * Used to take pictures using the raspberry pi camera using specified command line
 * arguements that will be saved in a specified directory. If the string parameter that
 * contains the arguements is null, it is assumed no extra options are provided.
 * The picture will be saved in a directory named the date of capture in ISO 8601
 */
int takepic(char *options)
{
        char command[COMMAND_SIZE]; // char array to hold the built camera command
        char dirbuf[] = "./pictures/YYYY-MM-DD"; // buffer to hold the directory
        char namebuf[] = "HH:MM:SS"; // buffer to hold the filename
        int tail = 0; // tail of the command
        tail += sprintf(command + tail, "raspistill "); // print the command name to the string

        if (options != NULL) { // print the options to the command
                tail += sprintf(command + tail, options);
        }
        time_t pictime = time(NULL);
        struct tm timep; // buffer for localtime
        localtime_r(&pictime, &timep); // obtain the broken down time
	strftime(dirbuf + 11, 11, "%Y-%m-%d", &timep); // format the time
        if (direxist(dirbuf) == 0) {
                createdir(dirbuf); // create the directory if it doesn't exist
        }
        strftime(namebuf, sizeof(namebuf), "%H:%M:%S", &timep); // format filename
        tail += sprintf(command + tail, "-o %s/%s", dirbuf, namebuf); // print save location to command

        logtoconsole("Taking picture\n");
        int result = system(command); // run the command to take a picture

        // TODO: Determine how many times to try to take a picture?
        // Taking a picture takes 6 seconds
        if (result == 64) { // check for errors
                fprintf(stderr, "Bad parameter passed to camera\n");
                exit(EXIT_FAILURE);
        }
        if (result == 70) {
                fprintf(stderr, "Camera or software error\n");
                exit(EXIT_FAILURE);
        }
        if (result == 130) {
                fprintf(stderr, "raspistill terminated by user\n");
                exit(EXIT_FAILURE);
        }
        sprintf(command, "Picture saved with name %s in %s/\n", namebuf, dirbuf);
        logtoconsole(command); // reuse command buffer to log to console
        return 0;
}

/**
 * Records a DHT reading to a file named after the current date
 * 
 */
int recordDHTread(void)
{
        float temp, humid; // temperature and humitity
        time_t readtime;   // time of reading
        struct tm tmbuf;   // buffer for localtime_r()
        char logbuf[] = "[HH:MM:SS] 00.0C 00.0%\n"; // buffer for logdata
        char pathbuf[] = "./enviornment/YYYY-MM-DD.txt"; // buffer for filename
        safeDHTread(&temp, &humid); // read the sensor
        readtime = time(NULL); // obtain time of reading
        localtime_r(&readtime, &tmbuf); // break down the time into components
        strftime(logbuf, 10, "[%H:%M:%S]", &tmbuf); // format the time into the string
        strftime(pathbuf + 14, 11, "%Y-%m-%d.txt", &tmbuf);    // format the filename
        sprintf(logbuf + 11, "%.1fC %.1f%%\n", temp, humid); // print the data to the buffer
        logtofile(pathbuf, logbuf); // log the data
        return 0;
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
                exit(EXIT_FAILURE);
        }
        if (!aregoodreadings(*temp, *humidity)) {
                printf("DHT bad readings check port");
                exit(EXIT_FAILURE);
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
