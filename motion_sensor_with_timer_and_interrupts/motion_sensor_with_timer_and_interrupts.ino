/*
  POWERED BY -WSW- . 
  El propósito de este programa es controlar una alarma activada por un sensor de movimiento PIR. 
  Cuando se detecta movimiento, se enciende un LED y se inicia un temporizador. 
  Al pasar un tiempo predefinido (5 segundos), el LED se apaga automáticamente.
  
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

  Licencia Dual:
Este código está disponible bajo la Licencia MIT para fines no comerciales. 
Si desea usar el código en un proyecto comercial, debe obtener una licencia comercial poniéndose en contacto conmigo.

*/

// Definir el tiempo del temporizador en segundos
#define timeSeconds 5

// Crear un puntero para el temporizador
hw_timer_t *timer = NULL;

// Declarar los pines del LED y el sensor PIR
const int led = 26;  // Pin GPIO para el LED
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
void ARDUINO_ISR_ATTR onTimer() {
  // Apagar el LED
  digitalWrite(led, LOW);
  // Enviar un mensaje de "ALARMA OFF" por el puerto serie
  Serial.println("ALARMA OFF");
  // Detener el temporizador
  timerStop(timer);
}

void setup() {
  // Inicializar el puerto serie para depuración
  Serial.begin(115200);
  
  // Configurar el sensor PIR como entrada con resistencia pull-up
  pinMode(motionSensor, INPUT_PULLUP);
  
  // Configurar la interrupción para el sensor PIR: se ejecuta la función detectsMovement cuando el pin sube (RISING)
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

  // Configurar el LED como salida
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);  // Inicialmente, el LED está apagado

  // Inicializar el temporizador con una frecuencia de 1 MHz
  timer = timerBegin(1000000);

  // Asociar la función onTimer al temporizador
  timerAttachInterrupt(timer, &onTimer);

  // Configurar el temporizador para que se ejecute cada 'timeSeconds' segundos
  // El temporizador se repetirá indefinidamente (tercer parámetro 'true') y con un único conteo
  timerAlarm(timer, timeSeconds * 1000000, true, 0);

  // Detener el temporizador inicialmente
  timerStop(timer);
}

void loop() {
  // El ciclo 'loop' está vacío porque el código no es bloqueante
  // Las acciones se manejan a través de interrupciones y el temporizador
}
