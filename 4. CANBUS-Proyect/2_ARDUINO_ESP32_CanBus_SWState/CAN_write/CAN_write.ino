#include <SPI.h> // Librería para la comunicación SPI
#include <mcp2515.h> // Librería para el controlador CAN MCP2515

// Estructura para almacenar un mensaje CAN
struct can_frame canMsg1;
// Instancia del objeto MCP2515 en el pin 10
MCP2515 mcp2515(10);

// Definición del pin del pulsador
int pul1 = 8;
// Variable para almacenar el estado del pulsador
int vpul1 = 1; // Valor inicial del pulsador (no presionado)

void setup() {
  // Configurar el pin del pulsador como entrada con resistencia de pull-up
  pinMode(pul1, INPUT_PULLUP);

  // Configurar el mensaje CAN
  canMsg1.can_id  = 0x0F6; // Identificador del nodo transmisor
  canMsg1.can_dlc = 8; // Data Length Code - código de longitud de datos, 8 Bytes
  // Inicializar los datos del mensaje CAN a 0x00
  for (int i = 0; i < 8; i++) {
    canMsg1.data[i] = 0x00;
  }

  // Inicializar la comunicación serial a 115200 baudios
  Serial.begin(115200);
  while (!Serial); // Esperar hasta que la comunicación serial esté lista

  // Reiniciar el MCP2515
  mcp2515.reset();
  // Configurar la velocidad de bits a 125 Kbps
  mcp2515.setBitrate(CAN_125KBPS);
  // Configurar el MCP2515 en modo normal
  mcp2515.setNormalMode();
}

void loop() {
  // Leer el estado del pulsador
  vpul1 = digitalRead(pul1);
  // Pequeña pausa para evitar rebotes
  delay(50);

  // Actualizar el primer byte del mensaje CAN con el estado del pulsador (invertido)
  canMsg1.data[0] = !vpul1;
  // Enviar el mensaje CAN
  mcp2515.sendMessage(&canMsg1);

  // Imprimir un mensaje en el monitor serial indicando que el mensaje fue enviado
  Serial.println("Mensaje enviado");
  // Imprimir los datos del mensaje CAN en el monitor serial
  for (int i = 0; i < 8; i++) {
    Serial.print(canMsg1.data[i]);
    if (i < 7) Serial.print(",");
  }
  Serial.println();

  // Pausa antes de la siguiente iteración del loop
  delay(100);
}