#include <stdlib.h>    // c standard library
#include <stdio.h>     // standard i/0
#include <unistd.h>    // unix standard library
#include <fcntl.h>     // filecontrol
#include <sys/stat.h>  // stat()
#include <errno.h>     // access to errno
#include <curl/curl.h> // libcurl
#include "../includes/util.h" // access to utility functions
#include "../includes/networking.h" // function declarations

#define THINGSPEAKKEY "./THINGSPEAK-KEY"  // file where the thingspeak apikey is stored
#define IMGURKEY "./IMGUR-KEY" // file where imgur oauth key is stored

// helper function for libcurl
static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);

static char *thingspeakkey = NULL; // thingspeak api key
static char *imgurkey = NULL;  // imgur oauth key, obtained on startup from a request

/**
 * Created by Nicholas Daddona
 * 
 * Contains various functions to help manage connections to the thingspeak platform.
 * Uses libcurl to upload data read from the temperature probe to thingspeak
 */

/**
 * Obtains neccesary credentials for the networking component
 * 
 * Both the thingspeak api key and the oauth key for imgur are allocated dynamically
 * Use dumpcredentials() to free
 */
void loadcredentials()
{
        if (thingspeakkey == NULL) { // load the thingspeak api key
                thingspeakkey = readfromfile(THINGSPEAKKEY);
        }
        if (imgurkey == NULL) { // load the imgur oauth key
                imgurkey = readfromfile(IMGURKEY);
        }
}

/**
 * Uploads temperature and humidity to thingspeak using libcurl to make an
 * http POST request. The api key is supplied as part of the url
 */
void uploaddata(float temp, float humid)
{
        char url[] = "http://api.thingspeak.com/update?api_key=XXXXXXXXXXXXXXXX";
        sprintf(url, "http://api.thingspeak.com/update?api_key=%s", thingspeakkey); // print api key
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
                        fprintf(stderr, "Thingspeak upload failed: %s\n", curl_easy_strerror(res));
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
 * Uploads a picture to Imgur
 * 
 * path is formatted ./pictures/YYYY-MM-DD/HH:MM:SS.jpg
 * timep is a pointer to a tm struct formed when the picture was saved to disk
 */
void uploadpicture(char *path, struct tm *timep)
{
        char authorization[] = "Authorization: Bearer XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
        char description[] = "Picture taken on YYYY-MM-DD at HH:MM:SS"; // description for the picture
        sprintf(authorization, "Authorization: Bearer %s", imgurkey); // print the oauth key
        strftime(description, sizeof(description), "Picture taken on %Y-%m-%d at %H:%M:%S", timep);

        CURLcode res; // result from curl
        CURL *curl; // pointer to curl handle
        struct curl_httppost *post1 = NULL;
        struct curl_httppost *postend = NULL;
        struct curl_slist *slist1 = NULL;
        
        curl_global_init(CURL_GLOBAL_ALL); // initialize curl
        curl = curl_easy_init(); // get a curl handle
        if (curl) {
                curl_formadd(&post1, &postend, // add the image to the form data
                        CURLFORM_COPYNAME, "image",
                        CURLFORM_FILE, path,
                        CURLFORM_CONTENTTYPE, "image/jpeg",
                        CURLFORM_END);
                curl_formadd(&post1, &postend, // add the album hash to the form data
                        CURLFORM_COPYNAME, "album",
                        CURLFORM_COPYCONTENTS, "h2XmLmn",
                        CURLFORM_END);

                curl_formadd(&post1, &postend, // add the description of the picture
                        CURLFORM_COPYNAME, "description",
                        CURLFORM_COPYCONTENTS, description,
                        CURLFORM_END);

                // add the header
                slist1 = curl_slist_append(slist1, authorization); // add the oauth key
                slist1 = curl_slist_append(slist1, "content-type: multipart/form-data;");

                curl_easy_setopt(curl, CURLOPT_URL, "https://api.imgur.com/3/image");
                curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
                curl_easy_setopt(curl, CURLOPT_HTTPPOST, post1);
                curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.52.1");
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);
                curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
                curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
                curl_easy_setopt(curl, CURLOPT_SSH_KNOWNHOSTS, "/home/pi/.ssh/known_hosts");
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
                curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); // override writing to stdout
                
                res = curl_easy_perform(curl);

                if(res != CURLE_OK) { // check for errors
                        fprintf(stderr, "Imgur upload failed: %s\n", curl_easy_strerror(res));
                }

                curl_easy_cleanup(curl); // perform cleanup
                curl_formfree(post1);
                curl_slist_free_all(slist1);
        }
        curl_global_cleanup(); // cleanup
}

/**
 * Delete the API key from memory once it's no longer needed
 */
void dumpcredentials()
{
        if (thingspeakkey != NULL) {
                free(thingspeakkey);
                thingspeakkey = NULL;
        }
        if (imgurkey != NULL) {
                free(imgurkey);
                imgurkey = NULL;
        }
}