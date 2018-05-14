#ifndef _UTIL_H_
#define _UTIL_H_

void cleanup(void);

int direxist(char *path);

void createdir(char *path);

void logtoconsole(char *message);

void logtofile(char *path, char *message);

#endif