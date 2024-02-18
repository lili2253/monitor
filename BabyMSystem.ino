#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include<Arduino_JSON.h>
#include<WiFiClient.h>



const char *ssid = "Hardware-Lab";
const char *password = "H@rdw@re2o23";
const char *server_address = "http://localhost/baby_monitor/baby_monitor.php";

#define DHTPIN 2     // D4 pin for DHT sensor
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
  dht.begin();
}

void loop() {
  delay(2000);

  float temperature = dht.readTemperature();
  int humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C");
    Serial.print(" Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    sendTemperatureData(temperature, humidity);
  }

  delay(5000);
}

void sendTemperatureData(float temperature, int humidity) {
  HTTPClient http;
  int httpCode;
  String postData = "temperature=" + String(temperature) + "&humidity=" + String(humidity);

  Serial.println();
  Serial.println("Sending temperature and humidity data...");

  http.begin(client,server_address);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  httpCode = http.POST(postData);

  if (httpCode > 0) {
    Serial.print("HTTP response code: ");
    Serial.println(httpCode);

    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println("Server response: " + payload);
    } else {
      Serial.println("HTTP request failed");
      // Handle the failure (e.g., retry, notify user)
    }
  } else  {
    Serial.println("Error sending HTTP request");
    // Handle the error (e.g., retry, check network connection)
  }

  http.end();
}
