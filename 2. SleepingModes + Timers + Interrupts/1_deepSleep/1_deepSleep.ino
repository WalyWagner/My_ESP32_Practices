/* 
  POWERED BY -WSW- 
  El propósito de este programa es controlar una alarma activada por un sensor de movimiento PIR. 
  Cuando se detecta movimiento, se enciende un LED y se inicia un temporizador. 
  Al pasar un tiempo predefinido (5 segundos), el LED se apaga automáticamente. 
  Además, el microcontrolador entrará en modo de sueño profundo cuando no se detecte movimiento.
  
  **Hardware necesario:**
  - Un ESP32.
  - Un sensor PIR (conectado al pin 27 del ESP32).
  - Un LED (conectado al pin 26 del ESP32).
  
  **Conexiones:**
  - El sensor PIR se conecta al pin GPIO 27.
  - El LED se conecta al pin GPIO 26.

  **Importante:** Este código no es bloqueante, lo que significa que el microcontrolador puede seguir 
  ejecutando otras tareas mientras se maneja la detección de movimiento y el temporizador sin detener 
  el flujo principal del programa.

  **Aclaración:** Este código está desarrollado para el paquete de esp32 by Espressif Systems desde 
  la versión 3.0.0 en adelante. Esto se debe a que las funciones de temporizado cambiaron respecto a 
  la versión 2.x.x

  Licencia Dual:
Este código está disponible bajo la Licencia MIT para fines no comerciales. 
Si desea usar el código en un proyecto comercial, debe obtener una licencia comercial poniéndose en contacto conmigo.
*/

// Definir el tiempo del temporizador en segundos
#define timeSeconds 5
#define MOTION_SENSOR_PIN GPIO_NUM_27  // Pin GPIO para el sensor de movimiento
#define LED_PIN GPIO_NUM_26            // Pin GPIO para el LED

// Crear un puntero para el temporizador
hw_timer_t *timer = NULL;
RTC_DATA_ATTR int bootCount = 0;

void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  
  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:     
      Serial.println("Wakeup caused by external signal using RTC_IO"); 
      break;
    case ESP_SLEEP_WAKEUP_EXT1:     
      Serial.println("Wakeup caused by external signal using RTC_CNTL"); 
      break;
    case ESP_SLEEP_WAKEUP_TIMER:    
      Serial.println("Wakeup caused by timer"); 
      break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: 
      Serial.println("Wakeup caused by touchpad"); 
      break;
    case ESP_SLEEP_WAKEUP_ULP:      
      Serial.println("Wakeup caused by ULP program"); 
      break;
    default:                        
      Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); 
      break;
  }
}

// Función que se ejecuta cuando se detecta movimiento
void detectsMovement() {
  // Encender el LED
  digitalWrite(LED_PIN, HIGH);
  // Enviar un mensaje de "ALARMA ON" por el puerto serie
  Serial.println("ALARMA ON");
  // Iniciar el temporizador
  timerStart(timer);
}

// Función que se ejecuta cuando el temporizador finaliza
void ARDUINO_ISR_ATTR onTimer() {
  // Apagar el LED
  digitalWrite(LED_PIN, LOW);
  // Enviar un mensaje de "ALARMA OFF" por el puerto serie
  Serial.println("ALARMA OFF");
  // Detener el temporizador
  timerStop(timer);
  // Entrar en modo de sueño profundo
  Serial.println("Going to sleep again");
  esp_deep_sleep_start();
}

void setup() {
  if (bootCount == 0) { // primer setup
    // Inicializar el puerto serie para depuración
    Serial.begin(115200);
    // delay(1000);  // Tomarse un tiempo para abrir el Monitor Serie

    // Incrementar el contador de inicio y mostrarlo en cada reinicio
    ++bootCount;
    Serial.println("Boot number: " + String(bootCount));

    // Configurar el sensor PIR como entrada con resistencia pull-up
    pinMode(MOTION_SENSOR_PIN, INPUT_PULLUP);

    // Configurar el LED como salida
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);  // Inicialmente, el LED está apagado

    // Configurar la fuente de despertar externa usando RTC_IO
    esp_sleep_enable_ext0_wakeup(MOTION_SENSOR_PIN, 1);  // 1 = High, 0 = Low

    // Entrar en modo de sueño profundo
    Serial.println("Going to sleep now");
    esp_deep_sleep_start();
  } else { // cada vez que despierte de un deepsleep
    // Inicializar el puerto serie para depuración
    Serial.begin(115200);
    // delay(1000);  // Tomarse un tiempo para abrir el Monitor Serie

    // Incrementar el contador de inicio y mostrarlo en cada reinicio
    ++bootCount;
    Serial.println("Boot number: " + String(bootCount));

    // Mostrar la razón del despertar del ESP32
    print_wakeup_reason();

    // Configurar el sensor PIR como entrada con resistencia pull-up
    pinMode(MOTION_SENSOR_PIN, INPUT_PULLUP);

    // Configurar el LED como salida
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);  // Inicialmente, el LED está apagado

    // Inicializar el temporizador con una frecuencia de 1 MHz
    timer = timerBegin(1000000);

    // Asociar la función onTimer al temporizador
    timerAttachInterrupt(timer, &onTimer);

    // Configurar el temporizador para que se ejecute cada 'timeSeconds' segundos
    timerAlarm(timer, timeSeconds * 1000000, true, 0);

    // Configurar la fuente de despertar externa usando RTC_IO
    esp_sleep_enable_ext0_wakeup(MOTION_SENSOR_PIN, 1);  // 1 = High, 0 = Low

    detectsMovement();  // --> se ha detectado movimiento
  }
}

void loop() {
  // El ciclo 'loop' está vacío porque el código no es bloqueante
  // Las acciones se manejan a través de interrupciones y el temporizador
}

