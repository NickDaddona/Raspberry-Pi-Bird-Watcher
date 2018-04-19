#include <stdlib.h> // standard library
#include <stdio.h>  // standard i/o
#include <time.h>   // time_t and time related functions
#include <stdbool.h> // true macro
#include <unistd.h> // unix standard library
#include <fcntl.h>  // file control
#include "../includes/sensors.h" // access sensors
#include "../includes/util.h"    // access utility functions

#define FIFTEENMIN 900 // number of seconds in fifteenminutes

void init(void); // initialze filesystem for program

/**
 * Created by Nicholas Daddona
 * 
 * Program Entry point, once the startup check finishes, an initial temperature
 * and humidity reading will be taken. Every 15 minutes afterwards, another reading
 * will be taken. While the system is waiting, it will continuously poll the motion
 * sensor until it detects motion. Once motion is detected, a picture will be taken
 * and sorted into the pictures directory by date and time. 
 */
int main(void)
{
        init(); // run startup check
        time_t current, prev; // time in seconds of last reading and current time
        recordDHTread();   // initial temp and humidity recording for start
        prev = time(NULL); // obtain a time to start
        while (true) {
                current = time(NULL); // check to see if 15 minutes have passed
                if ((current - prev) >= FIFTEENMIN) {
                        recordDHTread(); // record an envoirnmental reading
                        prev = current; // record a new previous time
                }
                if (readPIR() == 1) { // take a picture if motion is detected
                        takepic(NULL);
                }
        }
        return EXIT_SUCCESS;
}

/**
 * Initializes filesystem used for program, tests connection to all sensors
 * 
 * If pictures and environment aren't present in the current directory, create them
 * Polls each sensor used with a test reading
 */
void init(void)
{
        float temp, humid; // temp variables used to test DHT Sensor
        if (!direxist(PICTUREPATH)) { // create picture directory if needed
                createdir(PICTUREPATH);
        }
        if (!direxist(ENVIRONMENTPATH)) { // create environment readings directory if needed
                createdir(ENVIRONMENTPATH);
        }

        initGrove(); // initialize connection with the grovepi board
        logtoconsole("Connection Established with GrovePi Board\n");
        readPIR(); // test the motion sensor
        logtoconsole("PIR Motion Sensor detected\n");
        safeDHTread(&temp, &humid); // test the dht sensor
        logtoconsole("Temperature and Humidity Sensor Detected\n");
        if (system("raspistill -t 5") == 0) { // test the camera
                logtoconsole("Camera Module Detected\n");
        }
        else { // error handling for camera connection failing
                logtoconsole("Communication error with camera, check connection\n");
                exit(EXIT_FAILURE);
        }
}