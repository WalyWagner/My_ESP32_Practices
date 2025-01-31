#include <SPI.h>
#include <mcp2515.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Replace with your network credentials
const char* ssid = "Pb 2.4";
const char* password = "";

#define DHTPIN 27     // Digital pin connected to the DHT sensor

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// CAN bus configuration
MCP2515 mcp2515(5); // CS pin is GPIO 5
struct can_frame canMsg;

// Variables to store the received temperature and humidity
float receivedTemperature = 0.0;
float receivedHumidity = 0.0;

String readDHTTemperature() {
  float t = dht.readTemperature();
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  } else {
    Serial.print(t);
    Serial.print("  ");
    return String(t);
  }
}

String readDHTHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  } else {
    Serial.println(h);
    return String(h);
  }
}

// Function to read temperature from the CAN message
String readCANTemperature() {
  if (isnan(receivedTemperature)) {
    return "--";
  } else {
    return String(receivedTemperature, 2);
  }
}

// Function to read humidity from the CAN message
String readCANHumidity() {
  if (isnan(receivedHumidity)) {
    return "--";
  } else {
    return String(receivedHumidity, 2);
  }
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
    .container {
      display: flex;
      justify-content: space-around;
    }
    .sensor {
      flex: 1;
      padding: 20px;
    }
  </style>
</head>
<body>
  <h2>ESP32 DHT Server</h2>
  <div class="container">
    <div class="sensor">
      <h3>NODO MAESTRO (ESP32)</h3>
      <p>
        <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
        <span class="dht-labels">Temperature</span> 
        <span id="temperature1">%TEMPERATURE1%</span>
        <sup class="units">&deg;C</sup>
      </p>
      <p>
        <i class="fas fa-tint" style="color:#00add6;"></i> 
        <span class="dht-labels">Humidity</span>
        <span id="humidity1">%HUMIDITY1%</span>
        <sup class="units">&percnt;</sup>
      </p>
    </div>
    <div class="sensor">
      <h3>NODO ESCLAVO 1 (ARDUINO UNO)</h3>
      <p>
        <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
        <span class="dht-labels">Temperature</span> 
        <span id="temperature2">%TEMPERATURE2%</span>
        <sup class="units">&deg;C</sup>
      </p>
      <p>
        <i class="fas fa-tint" style="color:#00add6;"></i> 
        <span class="dht-labels">Humidity</span>
        <span id="humidity2">%HUMIDITY2%</span>
        <sup class="units">&percnt;</sup>
      </p>
    </div>
  </div>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature1").innerHTML = this.responseText.split(",")[0];
      document.getElementById("temperature2").innerHTML = this.responseText.split(",")[1];
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 2000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity1").innerHTML = this.responseText.split(",")[0];
      document.getElementById("humidity2").innerHTML = this.responseText.split(",")[1];
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 2000 ) ;
</script>
</html>)rawliteral";

// Replaces placeholder with DHT values
String processor(const String& var){
  if(var == "TEMPERATURE1"){
    return readDHTTemperature();
  }
  else if(var == "HUMIDITY1"){
    return readDHTHumidity();
  }
  else if(var == "TEMPERATURE2"){
    return readCANTemperature();
  }
  else if(var == "HUMIDITY2"){
    return readCANHumidity();
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  dht.begin();

  // Initialize CAN bus
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    String temperatures = readDHTTemperature() + "," + readCANTemperature();
    request->send_P(200, "text/plain", temperatures.c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    String humidities = readDHTHumidity() + "," + readCANHumidity();
    request->send_P(200, "text/plain", humidities.c_str());
  });

  // Start server
  server.begin();
}

void loop(){
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    // Convert the received bytes back to float values
    int tempInt = (canMsg.data[0] << 8) | canMsg.data[1];
    int humInt = (canMsg.data[2] << 8) | canMsg.data[3];
    receivedTemperature = tempInt / 100.0;
    receivedHumidity = humInt / 100.0;
  }
}