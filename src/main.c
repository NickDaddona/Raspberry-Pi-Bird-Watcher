#include <stdlib.h> // standard library
#include <stdio.h>  // standard i/o
#include <unistd.h> // unix standard library
#include <dirent.h> // manage directories
#include "../includes/sensors.h" // access sensors
#include "../includes/util.h"    // access utility functions


void init(void); // initialze filesystem for program

int main(void)
{
        init(); // initialize filesystem
        /*
        #ifndef __cplusplus
                printf("Main Program Compiled for C \n");
        #endif
        int result = -1;
        initGrove();
        float temp, humidity;
        safeDHTread(&temp, &humidity);

        printf("Temp: %fC Humidity: %f%%\n", temp, humidity);
        for (int i = 0; i < 10; i++) {
                result = readPIR();
                if (result) {
                        printf("Motion Detected\n");
                }
                else {
                        printf("Motion not detected\n");
                }
                sleep(5);
        }
        takepic(NULL, "test.jpg");
        printf("Picture taken and saved to current directory\n");
        */
        return 0;
}

/**
 * Initializes filesystem used for program
 * 
 * If pictures and environment aren't present in the current directory, create them
 */
void init(void)
{
        if (!direxist(PICTUREPATH)) { // create picture directory if needed
                createdir(PICTUREPATH);
        }

        if (!direxist(ENVIORNMENTPATH)) { // create enviornment readings directory if needed
                createdir(ENVIORNMENTPATH);
        }
}