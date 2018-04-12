#include <stdlib.h> // standard library
#include <stdio.h>  // standard i/o
#include <unistd.h> // unix standard library
#include "../includes/sensors.h"  // access sensors


/**
 * Test program for communicating with raspberry pi sensors
 * 
 *
 */
int main(void)
{
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
        return 0;
}