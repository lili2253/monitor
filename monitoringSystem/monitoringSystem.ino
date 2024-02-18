#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>
#include <WiFi.h>
#include "DHT.h"

const char* ssid = "Hardware-Lab";
const char* password = "H@rdw@re2o23";
const char* server_address = "http://localhost/baby_monitor/baby_monitor.php";

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht11_sensor(DHTPIN, DHTTYPE);
WiFiClient client;

#define FAN_PIN 6
#define ON_BOARD_FAN_PIN 7

void setup() {
  Serial.begin(115200);

  pinMode(ON_BOARD_FAN_PIN, OUTPUT);
 pinMode(FAN_PIN, OUTPUT);

  digitalWrite(ON_BOARD_FAN_PIN, HIGH);
  digitalWrite(FAN_PIN, HIGH);

  delay(2000);

 digitalWrite(ON_BOARD_FAN_PIN, LOW);
 digitalWrite(FAN_PIN, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.println("Connecting to WiFi...");

  int timeout = 20; // Timeout in seconds
  while (WiFi.status() != WL_CONNECTED && timeout > 0) {
   Serial.print(".");
  digitalWrite(ON_BOARD_FAN_PIN, !digitalRead(ON_BOARD_FAN_PIN)); // Blink onboard fan LED
  delay(500);
  timeout--;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to WiFi. Restarting...");
    delay(3000);
    ESP.restart();
  }

  dht11_sensor.begin();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    controlFan();
    //readDHTSensorData();
    sendDataToServer();
    delay(4000);
  } else {
    Serial.println("WiFi disconnected. Reconnecting...");
    WiFi.begin(ssid, password);
    delay(5000);
  }
}

void controlFan() {
  // Read fan control data from server
  HTTPClient http;
  String postData = "id=esp8266_01";
  String payload = "";

  http.begin(client, server_address);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode = http.POST(postData);

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      payload = http.getString();
      http.end();

      JSONVar data = JSON.parse(payload);
      if (JSON.typeof(data) == "undefined") {
        Serial.println("Failed to parse JSON");
        return;
      }

      if (data.hasOwnProperty("Fan")) {
        String fanState = data["Fan"];
        if (fanState == "ON") {
          digitalWrite(FAN_PIN, HIGH);
          Serial.println("Fan Turned ON");
        } else if (fanState == "OFF") {
          digitalWrite(FAN_PIN, LOW);
          Serial.println("Fan Turned OFF");
        } else {
          Serial.println("Invalid fan state");
        }
      } else {
        Serial.println("Fan state not found in JSON");
      }
    } else {
      Serial.println("HTTP request failed");
    }
  } else {
    Serial.println("Error sending HTTP request");
  }
}

void readDHTSensorData() {
  float temperature = dht11_sensor.readTemperature();
  int humidity = dht11_sensor.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
}

void sendDataToServer() {
  float temperature = dht11_sensor.readTemperature();
  int humidity = dht11_sensor.readHumidity();

  String postData = "id=esp8266_01";
  postData += "&temperature=" + String(temperature);
  postData += "&humidity=" + String(humidity);

  HTTPClient http;
  http.begin(client, server_address);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode = http.POST(postData);
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      Serial.println("Data sent to server successfully");
    } else {
      Serial.print("HTTP request failed with error code: ");
      Serial.println(httpCode);
      // You can add more specific error handling here if needed
    }
  } else {
    Serial.println("Error sending HTTP request");
    // Here you can also try to catch the error if available
    // Note: Arduino environment does not have standard exception handling
  }

  http.end();
}


 
