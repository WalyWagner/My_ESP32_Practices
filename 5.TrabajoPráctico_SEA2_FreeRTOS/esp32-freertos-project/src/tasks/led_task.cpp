#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "config.h"

void ledTask(void *pvParameters) {
    digitalWrite(LED_PIN, HIGH); // Turn on the LED
    vTaskDelay(pdMS_TO_TICKS(2000)); // Keep the LED on for 2 seconds
    digitalWrite(LED_PIN, LOW); // Turn off the LED
    vTaskDelete(NULL); // Delete the LED task
}