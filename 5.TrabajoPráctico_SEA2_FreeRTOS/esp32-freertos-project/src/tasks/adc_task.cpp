#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "config.h"
#include "../types/index.h"
#include <esp_heap_caps.h>

// Handle for the button task
extern TaskHandle_t buttonTaskHandle;

// Function to print heap memory status
/*void printHeapMemoryStatus() {
    Serial.print("Free heap size: ");
    Serial.println(esp_get_free_heap_size());
    Serial.print("Minimum free heap size: ");
    Serial.println(esp_get_minimum_free_heap_size());
}*/

// Function to print task stack high water mark
/*void printTaskStackHighWaterMark(TaskHandle_t xTask) {
    UBaseType_t highWaterMark = uxTaskGetStackHighWaterMark(xTask);
    Serial.print("Task stack high water mark: ");
    Serial.print(highWaterMark);
    Serial.println(" words");
}*/

void adcTask(void *pvParameters) {
    analogReadResolution(ADC_RESOLUTION); // Set ADC resolution
    const int maxSamples = ADC_MEASUREMENT_COUNT; // Maximum number of samples based on stack size
    int counterSamples = 0;
    ADCMeasurement samples[maxSamples]; // Array to store ADC samples
    TickType_t startTime = xTaskGetTickCount(); // Task start time
    TickType_t endTime = startTime + pdMS_TO_TICKS(ADC_MEASUREMENT_DURATION); // Task end time

    //Serial.println("Memory status before ADC task:");
    //printHeapMemoryStatus(); // Print heap memory status before ADC task
    //printTaskStackHighWaterMark(NULL); // Print stack high water mark for the current task

    while (xTaskGetTickCount() < endTime && counterSamples < maxSamples) {
        samples[counterSamples].value = analogRead(ADC_PIN); // Read ADC value
        samples[counterSamples].timestamp = (xTaskGetTickCount() - startTime) * portTICK_PERIOD_MS; // Store sample timestamp
        counterSamples++;
        vTaskDelay(1); // Delay for sampling rate
    }

    Serial.print("ADC Task running... \n Total samples: ");
    Serial.println(counterSamples);

    for (int i = 0; i < counterSamples; i++) {
        Serial.print("Sample ");
        Serial.print(i);
        Serial.print(" - Time: ");
        Serial.print(samples[i].timestamp);
        Serial.print(" ms, Value: ");
        Serial.println(samples[i].value);
    }
    /*This information will be sent to other task...*/

    /*
    Serial.println("Memory status after ADC task:");
    printHeapMemoryStatus(); // Print heap memory status after ADC task
    printTaskStackHighWaterMark(NULL); // Print stack high water mark for the current task
    */
    vTaskResume(buttonTaskHandle); // Resume the button task
    vTaskDelete(NULL); // Delete the ADC task

}