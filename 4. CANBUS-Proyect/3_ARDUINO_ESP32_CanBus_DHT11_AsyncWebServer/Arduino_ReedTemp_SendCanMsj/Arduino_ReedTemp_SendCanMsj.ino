#include <SPI.h> // Librería para la comunicación SPI
#include <mcp2515.h> // Librería para el controlador CAN MCP2515
#include "DHT.h" // Librería para el sensor DHT

#define DHTPIN 4     // Pin del sensor DHT11
#define DHTTYPE DHT11 // Tipo de sensor DHT11

// Estructura para almacenar un mensaje CAN
struct can_frame canMsg1;
// Instancia del objeto MCP2515 en el pin 10
MCP2515 mcp2515(10);

// Instancia del sensor DHT
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Inicializar el sensor DHT
  dht.begin();

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
  // Leer la temperatura y la humedad del sensor DHT11
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Verificar si la lectura es válida
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Error al leer el sensor DHT11");
    return;
  }

  // Pausa para estabilizar las lecturas del sensor
  delay(50);
  
  // Escalar la temperatura y la humedad a enteros (manteniendo dos decimales)
  int tempInt = (int)(temperature * 100);
  int humInt = (int)(humidity * 100);

  // Dividir los valores en bytes y almacenarlos en el mensaje CAN
  canMsg1.data[0] = (tempInt >> 8) & 0xFF; // Parte alta de la temperatura
  canMsg1.data[1] = tempInt & 0xFF;        // Parte baja de la temperatura
  canMsg1.data[2] = (humInt >> 8) & 0xFF;  // Parte alta de la humedad
  canMsg1.data[3] = humInt & 0xFF;         // Parte baja de la humedad

  // Enviar el mensaje CAN
  mcp2515.sendMessage(&canMsg1);

  // Imprimir un mensaje en el monitor serial indicando que el mensaje fue enviado
  Serial.println("Mensaje enviado");
  Serial.print(temperature);
  Serial.print("  ");
  Serial.println(humidity);
  // Imprimir los datos del mensaje CAN en el monitor serial
  for (int i = 0; i < 8; i++) {
    Serial.print(canMsg1.data[i]);
    if (i < 7) Serial.print(",");
  }
  Serial.println();

  // Pausa antes de la siguiente iteración del loop
  delay(2000); // Pausa de 2 segundos para evitar lecturas demasiado frecuentes
}