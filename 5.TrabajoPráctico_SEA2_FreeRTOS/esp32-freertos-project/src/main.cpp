#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "FreeRTOSConfig.h"
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
    Serial.begin(115200); // Initialize serial communication
    pinMode(BUTTON_PIN, INPUT_PULLUP); // Configure button pin as input with pull-up
    pinMode(LED_PIN, OUTPUT); // Configure LED pin as output
    
    // Create button task
    xTaskCreate(buttonTask, "Button Task", BUTTON_TASK_STACK_SIZE, NULL, BUTTON_TASK_PRIORITY, &buttonTaskHandle);
}

void loop() {
    // The loop is intentionally left empty as FreeRTOS handles task scheduling
}