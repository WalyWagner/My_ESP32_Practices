/*
  POWERED BY -WSW- 
  El propósito de este programa es controlar tres LEDs encendiedno y apagando a frecuencias distintas
  haciendo uso de 3 timers.
  La importancia de este código radia en el uso de la API de timers anterior a la actual 3.1.1 (27/1/25)
  En la API de timers del paquete 2.x.x de ESP32 (hasta 3.0.0 sin incluir), los temporizadores se utilizaban
  con funciones distintas

  **Hardware necesario:**
  - Un ESP32.
  - Tres LEDs y sus resistencias.
  
    **Importante:** Este código no es bloqueante

  **Aclaración:**  --> https://randomnerdtutorials.com/esp32-migrating-version-2-to-3-arduino/
   Timer – New APIs
   Timer has been refactored to use the new ESP-IDF driver and its API got simplified – new Timer API.

      *timerAlarm used to set up Alarm for the timer and enable it automatically (merged timerAlarmWrite and timerAlarmEnable functions).
      *timerGetFrequency used to get the actual frequency of the timer.
      *timerAttachInterruptArg used to attach the interrupt to a timer using arguments.
      
      *timerBegin has now only 1 parameter (frequency). There is an automatic calculation of the divider using different clock sources to achieve the selected frequency.
      *timerAttachInterrupt has now only 2 parameters. The edge parameter has been removed.

  Licencia Dual:
Este código está disponible bajo la Licencia MIT para fines no comerciales. 
Si desea usar el código en un proyecto comercial, debe obtener una licencia comercial poniéndose en contacto conmigo.

*/

hw_timer_t * timer0 = NULL;
hw_timer_t * timer1 = NULL;
hw_timer_t * timer2 = NULL;
portMUX_TYPE timerMux0 = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timerMux1 = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timerMux2 = portMUX_INITIALIZER_UNLOCKED;

volatile uint8_t led1stat = 0; 
volatile uint8_t led2stat = 0; 
volatile uint8_t led3stat = 0; 

void IRAM_ATTR onTimer0(){
  // Critical Code here
  portENTER_CRITICAL_ISR(&timerMux0);
  led1stat=1-led1stat;
  digitalWrite(16, led1stat);   // turn the LED on or off
  portEXIT_CRITICAL_ISR(&timerMux0);
}

void IRAM_ATTR onTimer1(){
  // Critical Code here
  portENTER_CRITICAL_ISR(&timerMux1);
  led2stat=1-led2stat;
  digitalWrite(17, led2stat);   // turn the LED on or off
  portEXIT_CRITICAL_ISR(&timerMux1);
}

void IRAM_ATTR onTimer2(){
  // Critical Code here
  portENTER_CRITICAL_ISR(&timerMux2);
  led3stat=1-led3stat;
  digitalWrite(5, led3stat);   // turn the LED on or off
  portEXIT_CRITICAL_ISR(&timerMux2);
}

void setup() {
  Serial.begin(115200);

  pinMode(17, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(17, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(16, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(5, LOW);    // turn the LED off by making the voltage LOW

  Serial.println("start timer 1");
  timer1 = timerBegin(1, 80, true);  // timer 1, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
  timerAttachInterrupt(timer1, &onTimer1, true); // edge (not level) triggered 
  timerAlarmWrite(timer1, 250000, true); // 250000 * 1 us = 250 ms, autoreload true

  Serial.println("start timer 0");
  timer0 = timerBegin(0, 80, true);  // timer 0, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
  timerAttachInterrupt(timer0, &onTimer0, true); // edge (not level) triggered 
  timerAlarmWrite(timer0, 1000000, true); // 1000000 * 1 us = 1 s, autoreload true

  Serial.println("start timer 2");
  timer2 = timerBegin(2, 80, true);  // timer 2, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
  timerAttachInterrupt(timer2, &onTimer2, true); // edge (not level) triggered 
  timerAlarmWrite(timer2, 750000, true); // 750000 * 1 us = 750 ms, autoreload true

  // at least enable the timer alarms
  timerAlarmEnable(timer0); // enable
  timerAlarmEnable(timer1); // enable
  timerAlarmEnable(timer2); // enable
}

void loop() {
  // nope nothing here
  vTaskDelay(portMAX_DELAY); // wait as much as posible ...
}