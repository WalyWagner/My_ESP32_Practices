#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "config.h"

void adcTask(void *pvParameters) {
    analogReadResolution(ADC_RESOLUTION);
    const int maxSamples = 2000; // Reduce the number of samples to avoid memory issues
    int samples[maxSamples];
    unsigned long startTime = millis();
    unsigned long endTime = startTime + 2000;
    int index = 0;

    while (millis() < endTime && index < maxSamples) {
        Serial.println("ADC Task running...");
        Serial.println (analogRead(ADC_PIN));
        vTaskDelay(pdMS_TO_TICKS(1000 / SAMPLE_RATE));

    }

    /*Serial.println("ADC Samples:");
    for (int i = 0; i < index; i++) {
        Serial.println(samples[i]);
    }*/

    vTaskDelete(NULL);
}