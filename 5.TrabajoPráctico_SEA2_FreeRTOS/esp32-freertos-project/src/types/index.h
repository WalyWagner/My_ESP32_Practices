#ifndef INDEX_H
#define INDEX_H

#include <stdint.h>

// Structure to store ADC measurements
typedef struct {
    uint32_t timestamp; // Timestamp of the measurement
    uint16_t value;     // ADC value
} ADCMeasurement;



#endif // INDEX_H