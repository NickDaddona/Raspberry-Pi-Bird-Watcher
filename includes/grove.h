#ifndef _GROVE_H_
#define _GROVE_H_

void initGrove(void);

int readPIR(void);

void safeDHTread(float *temp, float *humidity);

void unsafeDHTread(float *temp, float *humidity);

#endif // end _GROVE_H_