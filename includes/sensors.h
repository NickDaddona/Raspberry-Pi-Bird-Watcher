#ifndef _SENSORS_H_
#define _SENSORS_H_

// constants for the subdirectories containing pictures and enviornment log data
#define PICTUREPATH "./pictures"
#define ENVIORNMENTPATH "./enviornment"

int takepic(char * options);

void initGrove(void);

int readPIR(void);

void safeDHTread(float *temp, float *humidity);

void unsafeDHTread(float *temp, float *humidity);

#endif // end _SENSORS_H_