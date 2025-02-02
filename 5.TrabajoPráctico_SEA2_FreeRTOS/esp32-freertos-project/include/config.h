
#ifndef CONFIG_H
#define CONFIG_H

#define BUTTON_PIN 5
#define LED_PIN 18

#define BUTTON_TASK_STACK_SIZE 2048 // Stack size for button task
#define LED_TASK_STACK_SIZE 1024 // Stack size for LED task
#define ADC_TASK_STACK_SIZE 100000 // Increase stack size for ADC task

#define BUTTON_TASK_PRIORITY 1
#define ADC_TASK_PRIORITY 2
#define LED_TASK_PRIORITY 1

#define ADC_PIN 34
#define ADC_RESOLUTION 12
#define SAMPLE_RATE 6000 // 6 kSPS
#define ADC_MEASUREMENT_COUNT 3000 // Maximum number of measurements to store

#define ADC_MEASUREMENT_DURATION 3000 // in milliseconds
#define RANDOM_DELAY_MIN 2000 // in milliseconds
#define RANDOM_DELAY_MAX 5000 // in milliseconds

#endif // CONFIG_H