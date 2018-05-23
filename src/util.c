#include <unistd.h> // unix standard library
#include <fcntl.h>  // filecontrol
#include <sys/stat.h> // mkdir()
#include <dirent.h> // opendir() closedir()
#include <stdlib.h> // exit()
#include <stdio.h>  // printing to console
#include <time.h>   // for determining the time when logging to console or file
#include <string.h> // strcpy()
#include <errno.h>  // access to errno
#include "../includes/networking.h" // access to dumpcredentials()
#include "../includes/util.h" // contains function declarations

/**
 * Created by: Nicholas Daddona
 * 
 * Contains various utilities to help manage the pictures and environmental data
 * collected. Also helps provide logs to the console to indicate what's currently
 * happening
*/

/**
 * Preform cleanup operations before exiting
 * 
 */
void cleanup(void)
{
         // kill background raspistill process, redirect all output into /dev/null
        system("kill -KILL $(pgrep raspistill) > /dev/null 2>&1 ");
        dumpcredentials(); // free memory allocated for credentials
}

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
                        cleanup();
                        exit(EXIT_FAILURE); // exit on any other error
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
                cleanup();
                exit(EXIT_FAILURE);
        }
        else { // success conditions
                sprintf(msgbuf, "Created Directory: %s\n", path); // build the message
                logtoconsole(msgbuf);
        }
}

/**
 * Logs a message to stdout adding the time before the message
 */
void logtoconsole(char *message)
{
        char f_time[] = "[HH:MM:SS]"; // date and time format
        time_t logtime = time(NULL); // get the current time in milliseconds
        struct tm timep; // buffer for localtime
        localtime_r(&logtime, &timep); // obtain the broken down time
	strftime(f_time, sizeof(f_time), "[%H:%M:%S]", &timep); // format the time
        printf("%s: %s", f_time, message); // print the message with time
}

/**
 * Reads the Entire contents of a file into a dynamically allocated buffer
 * The buffer is null terminated and thus its size can be determined with strlen()
 * Buffer must be deallocated with free once it is no longer needed/in use
 */
char * readfromfile(char *name)
{
        int srcfd, bytesread, totalread = 0;
        char pathbuf[256]; // buffer for filepath
        char *buffer; // buffer for file contents
        struct stat statbuf; // buffer for stat()
        if (*(name) != '/' || strncmp(name, "./", 2) != 0 
            || strncmp(name, "../", 3) != 0) { // append ./ to filename
            sprintf(pathbuf,"./%s", name);
        }
        else {
                sprintf(pathbuf, "%s", name);
        }

        if (stat(pathbuf, &statbuf) == -1) { // stat the file
                fprintf(stderr, "Stat failed on %s\n%s", name, strerror(errno));
                exit(EXIT_FAILURE);
        }
        
        // create a buffer big enough to store the contents of the file
        if ((buffer = (char *) calloc(sizeof(char), statbuf.st_size + 1)) == NULL) { 
                fprintf(stderr, "Error allocation failed \n%s", strerror(errno));
                exit(EXIT_FAILURE);
        }
        
        if ((srcfd = open(name, O_RDONLY)) == -1) { // open the file for reading
                fprintf(stderr, "Error opening %s\n%s", name, strerror(errno));
                exit(EXIT_FAILURE);
        }

        while ((bytesread = read(srcfd, buffer, statbuf.st_size) > 0)) { // read the file
                totalread += bytesread; // keep track of total bytes
        }
        if (bytesread == -1) { // exit if there was a read failure
                fprintf(stderr, "Error Reading %s\n%s\n", name, strerror(errno));
                exit(1);
        }
        close(srcfd); // close the opened file once reading finishes
        return buffer; // return a pointer to the allocated block        
}

/**
 * Logs a message to the file path using system calls and append mode
 */
void logtofile(char *path, char *message)
{
        int tries = 0;
        int logfd = -1; // filedescriptor
        while ((logfd = open(path, O_CREAT | O_WRONLY | O_APPEND, S_IRUSR 
            | S_IWUSR | S_IRGRP | S_IROTH)) == -1 && tries < 5) {
                tries++; // attempt to open the file, 5 tries max
        }
        if (tries == 5) {
                fprintf(stderr, "Failed to open %s\n%s\n", path, strerror(errno));
                cleanup();
                exit(EXIT_FAILURE); // exit if failed to open the file
        }

        int msgsz = strlen(message);
        int byteswritten = 0;
        int result = 0;
        while ((result = write(logfd, message + byteswritten, msgsz)) > 0 
            && byteswritten >= msgsz) { // stop when message is written
                if (result == -1) {
                        fprintf(stderr, "Error when writing to %s\n%s\n", path, strerror(errno));
                        cleanup();
                        exit(EXIT_FAILURE); // exit if write error
                }
                tries++; // write the message to the file, max 5 tries to write fully
        }
}
