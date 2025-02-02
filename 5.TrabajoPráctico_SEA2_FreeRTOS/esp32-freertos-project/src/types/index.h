#ifndef INDEX_H
#define INDEX_H

#include <stdint.h>

// Structure to store ADC measurements
typedef struct {
    uint32_t timestamp; // Timestamp of the measurement
    uint16_t value;     // ADC value
} ADCMeasurement;

#define ADC_MEASUREMENT_COUNT 2000 // Maximum number of measurements to store

#endif // INDEX_H