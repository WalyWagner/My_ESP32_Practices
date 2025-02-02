#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "config.h"

// Task handles
TaskHandle_t buttonTaskHandle = NULL;
TaskHandle_t adcTaskHandle = NULL;
TaskHandle_t ledTaskHandle = NULL;

// Function prototypes
void buttonTask(void *pvParameters);
void adcTask(void *pvParameters);
void ledTask(void *pvParameters);

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    
    // Create tasks
    xTaskCreate(buttonTask, "Button Task", TASK_STACK_SIZE, NULL, 1, &buttonTaskHandle);
}

void loop() {
    // The loop is intentionally left empty as FreeRTOS handles task scheduling
}