#ifndef _NETWORKING_H_
#define _NETWORKING_H_

#include <time.h> // struct tm

void loadcredentials();

void uploaddata(float temp, float humid);

void uploadpicture(char *path, struct tm *timep);

void dumpcredentials();

#endif // _NETWORKING_H_