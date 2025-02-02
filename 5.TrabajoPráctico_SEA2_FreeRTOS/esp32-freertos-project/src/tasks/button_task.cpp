#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Arduino.h>
#include "config.h"

// Function prototypes
void ledTask(void *pvParameters);
void adcTask(void *pvParameters);

extern TaskHandle_t adcTaskHandle;
extern TaskHandle_t ledTaskHandle;

void buttonTask(void *pvParameters) {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    while (true) {
        if (digitalRead(BUTTON_PIN) == LOW) {
            vTaskDelay(pdMS_TO_TICKS(50)); // Debounce delay
            if (digitalRead(BUTTON_PIN) == LOW) {
                int randomDelay = random(2000, 5000); // Random delay between 2 and 5 seconds
                vTaskDelay(pdMS_TO_TICKS(randomDelay));
                xTaskCreate(ledTask, "LED Task", TASK_STACK_SIZE, NULL, 2, &ledTaskHandle);
                xTaskCreate(adcTask, "ADC Task", TASK_STACK_SIZE, NULL, configMAX_PRIORITIES - 1, &adcTaskHandle);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}