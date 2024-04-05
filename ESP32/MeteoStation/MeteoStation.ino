#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT22     // DHT 11

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

AsyncWebServer server(80);

const char* ssid = "Android";
const char* password = "12345678";

void setup() {
  Serial.begin(115200);
  dht.begin();

  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);

  delayMS = sensor.min_delay / 1000;

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<html><body><h2>Meteo Station</h2><p>Temperature: ";
    html += readTemperature();
    html += " C</p><p>Humidity: ";
    html += readHumidity();
    html += " %</p><meta http-equiv='refresh' content='5'></body></html>";
    request->send(200, "text/html", html);
  });
  
  server.begin();
}

void loop() {
  delay(delayMS);
}

float readTemperature() {
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
    return 0;
  }
  else {
    return event.temperature;
  }
}

float readHumidity() {
  sensors_event_t event;  
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
    return 0;
  }
  else {
    return event.relative_humidity;
  }
}



