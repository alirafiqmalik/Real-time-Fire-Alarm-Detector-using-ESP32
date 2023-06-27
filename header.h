#include "src/arduinoFFT.h"
#include <driver/i2s.h>

#include "src/index.h" //Web page header file


static void integerToFloat(int32_t *integer, float *vReal, float *vImag, uint16_t samples);

unsigned int countSetBits(unsigned int n);

bool detectFrequency(unsigned int *mem, unsigned int minMatch, double peak, unsigned int bin1, unsigned int bin2, bool wide);

void vCalculateFFT(void *vpParameter);

void vWifi(void *vpParameter);