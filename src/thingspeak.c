#include <stdlib.h>    // c standard library
#include <stdio.h>     // standard i/0
#include <unistd.h>    // unix standard library
#include <fcntl.h>     // filecontrol
#include <sys/stat.h>  // stat()
#include <errno.h>     // access to errno
#include <curl/curl.h> // libcurl
#include "../includes/util.h" // access to utility functions
#include "../includes/thingspeak.h" // function declarations

#define APIKEY "./API-KEY"  // file where the apikey is stored

// helper function for libcurl
static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);

static char *apikey = NULL; // apikey, read from a file

/**
 * Created by Nicholas Daddona
 * 
 * Contains various functions to help manage connections to the thingspeak platform.
 * Uses libcurl to upload data read from the temperature probe to thingspeak
 */

/**
 * Reads the Thingspeak API key from a file named API-KEY
 * 
 * Space to hold the key is dynamically allocated, use dumpkey() to free once finished
 */
void loadapikey()
{
        if (apikey == NULL) {
                apikey = readfromfile(APIKEY);
        }
}

/**
 * Uploads temperature and humidity to thingspeak using libcurl to make an
 * http POST request. The api key is supplied as part of the url
 */
void uploaddata(float temp, float humid)
{
        char url[] = "http://api.thingspeak.com/update?api_key=XXXXXXXXXXXXXXXX";
        sprintf(url, "http://api.thingspeak.com/update?api_key=%s", apikey); // print api key
        char data[] = "field1=00.0&field2=00.0";
        sprintf(data, "field1=%.1f&field2=%.1f", temp, humid); // print data

        CURL *curl;
        CURLcode res;
 
        curl_global_init(CURL_GLOBAL_ALL); // initialize curl
 
        curl = curl_easy_init(); // get a curl handle
        if(curl) {
                curl_easy_setopt(curl, CURLOPT_URL, url); // specify the url with appended api key
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data); // specify post data
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); // override writing to stdout
                res = curl_easy_perform(curl); // perform the request

                if(res != CURLE_OK) { // check for errors
                        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                }
 
                curl_easy_cleanup(curl); // cleanup
        }
        curl_global_cleanup(); // possibly move global cleanup and init to main
}

/**
 * Function to stop libcurl writing to stdout
 */
size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
   return size * nmemb;
}

/**
 * Delete the API key from memory once it's no longer needed
 */
void dumpapikey()
{
        if (apikey != NULL) {
                free(apikey);
                apikey = NULL;
        }
}