#ifndef _SENSORS_H_
#define _SENSORS_H_


int takepic(char * options, char * savepath);

void initGrove(void);

int readPIR(void);

void safeDHTread(float *temp, float *humidity);

void unsafeDHTread(float *temp, float *humidity);

#endif // end _SENSORS_H_