#ifndef _SENSORS_H_
#define _SENSORS_H_

// constants for the subdirectories containing pictures and environment log data
#define PICTUREPATH "./pictures"
#define ENVIRONMENTPATH "./environment"

void initGrove(void);

int takepic(char * options);

int recordDHTread(void);

int readPIR(void);

void safeDHTread(float *temp, float *humidity);

void unsafeDHTread(float *temp, float *humidity);

#endif // end _SENSORS_H_