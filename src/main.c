#include <stdlib.h> // standard library
#include <stdio.h>  // standard i/o
#include <time.h>   // time_t and time related functions
#include <stdbool.h> // true macro
#include <unistd.h> // unix standard library
#include <fcntl.h>  // file control
#include <signal.h> // signal handling
#include "../includes/sensors.h" // access sensors
#include "../includes/util.h"    // access utility functions

#define FIFTEENMIN 60 * 15 // number of seconds in fifteenminutes

void init(void); // initialze filesystem for program
void signalhandler(int signum); // handle sigint and sigalarm

int sigint_flag = 0;  // flag set to 1 when process recieves sigint
int environment_flag = 0; // flag for determining when to take the next reading
int camera_timeout_flag = 0; // set to 1 when camera is timed out

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
        timer_t environment_timer, camera_timer;
        sigevent_t environment_event = {.sigev_notify = SIGEV_SIGNAL, .sigev_signo = SIGUSR1};
        sigevent_t camera_event = {.sigev_notify = SIGEV_SIGNAL, .sigev_signo = SIGUSR2};
        timer_create(CLOCK_MONOTONIC, &environment_event, &environment_timer); // create environment timer
        timer_create(CLOCK_MONOTONIC, &camera_event, &camera_timer); // create camera timer
        struct itimerspec environ = {{FIFTEENMIN, 0}, {FIFTEENMIN, 0}}, // arm for 15 minutes, rearm for 15 minutes
                           camera = {{0, 0},{60, 0}}; // arm for a 1 minute, don't repeat
        timer_settime(environment_timer, 0, &environ, NULL); // arm timer for environmental readings
        while (sigint_flag == 0) {
                if (environment_flag) {
                        recordDHTread(); // record an envoirnmental reading
                        environment_flag = 0; // reset flag to 0
                }
                if (readPIR() == 1 && camera_timeout_flag != 1) {
                        takepic(); // if motion is detected and camera isn't timed out
                        camera_timeout_flag = 1; // timeout camera
                        timer_settime(camera_timer, 0, &camera, NULL); // arm timer for camera
                }
        }
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
        signal(SIGUSR1, signalhandler); // specify handler for environmental alarm
        signal(SIGUSR2, signalhandler); // specify handler for camera timeout
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
}

/**
 * Preforms cleanup before exiting via interrupt
 */
void signalhandler(int signum)
{
        if (signum == SIGINT) { // cleanup and exit
                sigint_flag = 1;
        }
        if (signum == SIGUSR1) { // set flag to take enviornmental reading
                environment_flag = 1;
        }
        if (signum == SIGUSR2) { // set flag to timeout the camera
                camera_timeout_flag = 0;
        }
}