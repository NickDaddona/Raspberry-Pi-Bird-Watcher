#include <stdlib.h> // null macro
#include "../includes/camera.h" // to access camera functions


/**
 * Test program for communicating with raspberry pi sensors
 * 
 *
 */
int main(void)
{
        takepic(NULL, "test.jpg");
        return 0;
}