#include <stdlib.h> // standard library
#include <stdio.h>  // standard i/o
#include <unistd.h> // unix standard library
#include "../lib/grovewrap.h"
#include "../includes/camera.h" // to access camera functions

#define PIR_ID 8  // pir motion sensor in digital port 8
#define TEMP_ID 1 // Temperature probe in analog port 1

/**
 * Test program for communicating with raspberry pi sensors
 * 
 *
 */
int main(void)
{
        #ifndef __cplusplus
                printf("WOOOHHH I DID IT\n");
        #endif
        int result = -1;
        initgrovepi();
        while (true) {
                result = digitalread(PIR_ID);
                //printf("Result read");
                if (result) {
                        printf("Motion Detected\n");
                }
                else {
                        printf("Motion not detected\n");
                }
                sleep(5);
        }
        //takepic(NULL, "test.jpg");
        return 0;
}