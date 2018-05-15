#include <stdlib.h> // standard library
#include <stdio.h>  // standard i/o
#include <time.h>   // time_t and time related functions
#include <stdbool.h> // true macro
#include <unistd.h> // unix standard library
#include <fcntl.h>  // file control
#include <signal.h> // signal handling
#include "../includes/sensors.h" // access sensors
#include "../includes/util.h"    // access utility functions

#define FIFTEENMIN 900 // number of seconds in fifteenminutes

void init(void); // initialze filesystem for program
void signalhandler(int signum); // handle sigint and sigalarm

int sigint_flag = 0;
int sigalrm_flag = 0; // flag set to true when alarm goes off

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
        while (sigint_flag == 0) {
                if (sigalrm_flag) {
                        recordDHTread(); // record an envoirnmental reading
                        sigalrm_flag = 0; // reset flag to 0
                        alarm(FIFTEENMIN); // set an alarm for 15 minutes
                }
                if (readPIR() == 1) { // take a picture if motion is detected
                        takepic();
                }
        }
        cleanup(); // cleanup before exiting
        return EXIT_SUCCESS; // exit successfully
}

/**
 * Initializes filesystem used for program, tests connection to all sensors
 * 
 * If pictures and environment aren't present in the current directory, create them
 * Polls each sensor used with a test reading
 */
void init(void)
{
        signal(SIGINT, signalhandler);  // specifiy handler for interrupts
        signal(SIGALRM, signalhandler); // specify handler for alarm
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
        float temp, humid; // temp variables used to test DHT Sensor
        safeDHTread(&temp, &humid); // test the dht sensor
        logtoconsole("Temperature and Humidity Sensor Detected\n");
        startcamera(NULL); // test the camera and start background camera process
        alarm(FIFTEENMIN); // set the initial alarm for 15 minutes
}

/**
 * Preforms cleanup before exiting via interrupt
 */
void signalhandler(int signum)
{
        if (signum == SIGINT) { // cleanup and exit
                sigint_flag = 1;
        }
        if (signum == SIGALRM) { // set flag to take enviornmental reading
                sigalrm_flag = 1;
        }
}