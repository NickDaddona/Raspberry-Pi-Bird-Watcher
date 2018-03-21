#include <stdlib.h> // used for system()
#include <stdio.h>  // used for sprintf()
#include "../includes/camera.h" // used for function declarations

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