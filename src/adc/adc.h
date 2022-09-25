#ifndef FLYN_ADC_H
#define FLYN_ADC_H

#define ADC_RESOLUTION 12

#include <device.h>

int adc_init(const struct device *);
int adc_sample(const struct device *);

#endif