/*
  POWERED BY -WSW- 
  El propósito de este programa es controlar una alarma activada por un sensor de movimiento PIR. 
  Cuando se detecta movimiento, se enciende un LED y se inicia un temporizador. 
  Al pasar un tiempo predefinido (5 segundos), el LED se apaga automáticamente.
  Además, cada 10 segundos cambia el estado del led BUILT_IN
  
  **Hardware necesario:**
  - Un ESP32.
  - Un sensor PIR (conectado al pin 27 del ESP32).
  - Un LED (conectado al pin 26 del ESP32).
  - Led BUILT_IN
  
  **Conexiones:**
  - El sensor PIR se conecta al pin GPIO 27.
  - El LED se conecta al pin GPIO 26.

  **Importante:** Este código no es bloqueante, lo que significa que el microcontrolador puede seguir 
  ejecutando otras tareas mientras se maneja la detección de movimiento y el temporizador sin detener 
  el flujo principal del programa.

  **Aclaración:** Este código está desarrollado para el paquete de esp32 by Espressif Systems desde V
  la versión 3.0.0 en adelante. Esto se debe a que las funciones de temporizado cambiaron respecto a 
  la versión 2.x.x

  Licencia Dual:
Este código está disponible bajo la Licencia MIT para fines no comerciales. 
Si desea usar el código en un proyecto comercial, debe obtener una licencia comercial poniéndose en contacto conmigo.

*/

// Crear un puntero para cada temporizador
hw_timer_t *timer = NULL;
hw_timer_t *timer2 = NULL;

//utilizaremos interrupciones críticas
portMUX_TYPE timerMux1 = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timerMux2 = portMUX_INITIALIZER_UNLOCKED;

// Declarar los pines de los LEDs y el sensor PIR
const int led = 26;  // Pin GPIO para el LED
const int led2 = LED_BUILTIN;
const int motionSensor = 27;  // Pin GPIO para el sensor de movimiento

// Función que se ejecuta cuando se detecta movimiento
void IRAM_ATTR detectsMovement() {
  // Encender el LED
  digitalWrite(led, HIGH);
  // Enviar un mensaje de "ALARMA ON" por el puerto serie
  Serial.println("ALARMA ON");
  // Iniciar el temporizador
  timerStart(timer);
}

// Función que se ejecuta cuando el temporizador finaliza
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux1);
  // Apagar el LED
  digitalWrite(led, LOW);
  // Enviar un mensaje de "ALARMA OFF" por el puerto serie
  Serial.println("ALARMA OFF");
  // Detener el temporizador
  timerStop(timer);
  portEXIT_CRITICAL_ISR(&timerMux1);
}

// Función que se ejecuta cuando el temporizador 2 cuenta 10 segundos
void IRAM_ATTR onTimer2() {
  portENTER_CRITICAL_ISR(&timerMux2);
  // Togglear el LED
  bool newEstLed2 = 1 - digitalRead(led2);
  digitalWrite(led2, newEstLed2);
  //timerRestart(timer2);
  portEXIT_CRITICAL_ISR(&timerMux2);
}

void setup() {
  // Inicializar el puerto serie para depuración
  Serial.begin(115200);
  
  // Configurar el sensor PIR como entrada con resistencia pull-up
  pinMode(motionSensor, INPUT_PULLUP);
  
  // Configurar la interrupción para el sensor PIR: se ejecuta la función detectsMovement cuando el pin sube (RISING)
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

  // Configurar los LEDs como salida
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led, LOW);  // Inicialmente, el LED está apagado
  digitalWrite(led2, LOW);  // Inicialmente, el LED está apagado
  
  // Inicializar los temporizadores con una frecuencia de 1 MHz
  timer = timerBegin(1000000);
  timer2 = timerBegin(1000000);

  // Asociar la función onTimer al temporizador correspondiente
  timerAttachInterrupt(timer, &onTimer);
  timerAttachInterrupt(timer2, &onTimer2);
  
  // Configurar el temporizador para que se ejecute cada 'timeSeconds' segundos
  // El temporizador se repetirá indefinidamente (tercer parámetro 'true') y con un único conteo
  timerAlarm(timer, 5 * 1000000, true, 0);
  timerAlarm(timer2, 10 * 1000000, true, 0);

  // Detener el temporizador inicialmente
  timerStop(timer);
  }

void loop() {
  // El ciclo 'loop' está vacío porque el código no es bloqueante
  // Las acciones se manejan a través de interrupciones y el temporizador
}
