#include <sys/stat.h> // mkdir()
#include <dirent.h> // opendir() closedir()
#include <stdlib.h> // exit()
#include <stdio.h>  // printing to files
#include <time.h>   // for determining the time when logging to console or file
#include <string.h> // strcpy() and strcat()
#include <errno.h>  // access to errno
#include "../includes/util.h" // contains function declarations

/**
 * Created by: Nicholas Daddona
 * 
 * Contains various utilities to help manage the pictures and enviornmental data
 * collected. Also helps provide logs to the console to indicate what's currently
 * happening
*/


/**
 * Checks if a directory exists
 * 
 * Returns 1 if it exists, 0 otherwise
 */
int direxist(char *path)
{
        int retval = 0;
        DIR *dir; // pointer to directory stream
        if ((dir = opendir(path)) == NULL) {
                if (errno == ENOENT) { // if errno indicates it doesn't exist/NULL string
                        retval = 0;
                }
                else { // else print the specified error
                        fprintf(stderr, "Error when test opening %s: %s\n", path, strerror(errno));
                        exit(1); // exit on any other error
                }
        }
        else {
                closedir(dir); // close the directory if it exists
                retval = 1;    // set return val to 1 to indicate existance
        }
        return retval;
}

/**
 * Creates a directory using mkdir(), drwxr-xr-x
 * 
 * the program will exit if the directory cannot be created after 3 tries
 */
void createdir(char *path)
{
        char msgbuf[128]; // 128 byte message buffer for console
        int result = -1, tries = 0;
        while (result != 0 && tries < 3) { // try 3 times before exiting
                result = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                tries++;
        }
        if (result != 0) { // exit if meeting max tries
                fprintf(stderr, "Failed to create %s:/n%s", path, strerror(errno));
                exit(1); // TODO decide how to handle opened file descriptors
        }
        else { // success conditions
                strcpy(msgbuf, "Created Directory: "); // build the message
                strcat(msgbuf, path);
                logtoconsole(msgbuf);
        }
}

/**
 * Logs a message to stdout adding the time before the message
 */
void logtoconsole(char *message)
{
        char f_time[] = "HH:MM\0"; // date and time format
        time_t logtime = time(NULL); // get the current time in milliseconds
	strftime(f_time, sizeof(f_time), "%H:%M", localtime(&logtime));
        printf("%s: %s\n", f_time, message); // print the message with time
}

void logtofile(int fd, char *message)
{

}