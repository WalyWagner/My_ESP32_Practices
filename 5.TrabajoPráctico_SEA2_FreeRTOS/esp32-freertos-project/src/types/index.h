#ifndef INDEX_H
#define INDEX_H

#include <stdint.h>

typedef struct {
    uint32_t timestamp; // Timestamp of the measurement
    uint16_t value;     // ADC value
} ADCMeasurement;

#define ADC_MEASUREMENT_COUNT 100 // Maximum number of measurements to store

#endif // INDEX_H