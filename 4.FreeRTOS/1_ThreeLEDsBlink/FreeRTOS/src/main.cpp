#include <Arduino.h>

// Definir los pines de los LEDs
#define LED1 18 
#define LED2 19
#define LED3 21

// Tarea para parpadear el LED 1 (más rápido)
void TaskLED1(void *pvParameters) {
  pinMode(LED1, OUTPUT);
  while (1) {
    digitalWrite(LED1, HIGH);
    vTaskDelay(pdMS_TO_TICKS(500));  // 500 ms encendido
    digitalWrite(LED1, LOW);
    vTaskDelay(pdMS_TO_TICKS(500));  // 500 ms apagado
    //bool state = digitalRead(LED1);
    //digitalWrite(LED1, !state);
    // delay(500);  // 500 ms encendido
    Serial.print("Iam task 1, core: ");
    Serial.println(xPortGetCoreID());  
  }
}

// Tarea para parpadear el LED 2 (frecuencia media)
void TaskLED2(void *pvParameters) {
  pinMode(LED2, OUTPUT);
  while (1) {
    digitalWrite(LED2, HIGH);
    vTaskDelay(pdMS_TO_TICKS(1000));  // 1s encendido
    digitalWrite(LED2, LOW);
    vTaskDelay(pdMS_TO_TICKS(1000));  // 1s apagado
    // bool state = digitalRead(LED2);
    // digitalWrite(LED2, !state);
    // delay(1000);  // 1000 ms encendido
    Serial.print("Iam task 2, core: ");
    Serial.println(xPortGetCoreID());   }
}

// Tarea para parpadear el LED 3 (más lento)
void TaskLED3(void *pvParameters) {
  pinMode(LED3, OUTPUT);
  while (1) {
    digitalWrite(LED3, HIGH);
    vTaskDelay(pdMS_TO_TICKS(1500));  // 1.5s encendido
    digitalWrite(LED3, LOW);
    vTaskDelay(pdMS_TO_TICKS(1500));  // 1.5s apagado
    // bool state = digitalRead(LED3);
    // digitalWrite(LED3, !state);
    // delay(1500);  // 1500 ms encendido
    Serial.print("Iam task 3, core: ");
    Serial.println(xPortGetCoreID());   
  }
}

void setup() {
  Serial.begin(115200);

  // Crear tareas con diferentes prioridades
  xTaskCreate(TaskLED1, "TaskLED1", 1024, NULL, 3, NULL);  // Mayor prioridad
  xTaskCreate(TaskLED2, "TaskLED2", 1024, NULL, 2, NULL);  // Prioridad media
  xTaskCreate(TaskLED3, "TaskLED3", 1024, NULL, 1, NULL);  // Menor prioridad
}

void loop() {
  Serial.print("Iam not a task, Iam the loop, core: ");
  Serial.println(xPortGetCoreID());  
  delay(10000);
  // No es necesario hacer nada aquí, las tareas se ejecutan en FreeRTOS
}