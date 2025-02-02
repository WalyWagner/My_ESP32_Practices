#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "config.h"

void ledTask(void *pvParameters) {
    digitalWrite(LED_PIN, HIGH);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    digitalWrite(LED_PIN, LOW);
    vTaskDelete(NULL);
}