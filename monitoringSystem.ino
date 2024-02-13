#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>
#include "DHT.h"

#define DHTPIN 2 // Defines the Digital Pin connected to the DHT11 sensor.
#define DHTTYPE DHT11 // Defines the type of DHT sensor used. Here used is the DHT11 sensor.
DHT dht11_sensor(DHTPIN, DHTTYPE); // Initialize DHT sensor.
WiFiClient client;

const char* ssid = "Hardware-Lab";
const char* password = "H@rdw@re2o23";
const char* serverAddress = "192.168.30.130"; // IP address of your server
const String endpoint = "/baby_monitor/baby_monitor.php"; // Endpoint for the HTTP POST request

String postData = ""; // Variables sent for HTTP POST request data.
String payload = "";  // Variable for receiving response from HTTP POST.

float send_Temp;
int send_Humd;

void get_DHT11_sensor_data() {
  send_Temp = dht11_sensor.readTemperature();
  send_Humd = dht11_sensor.readHumidity();

  // Check if any reads failed.
  if (isnan(send_Temp) || isnan(send_Humd)) {
    Serial.println("Failed to read from DHT sensor!");
    send_Temp = 0.00;
    send_Humd = 0;
  } else {
    Serial.printf("Temperature : %.2f °C\n", send_Temp);
    Serial.printf("Humidity : %d %%\n", send_Humd);
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.println("-------------");
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Successfully connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("-------------");

  // Setting up the DHT sensor (DHT11).
  dht11_sensor.begin();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    postData = "id=esp8266_01"; // Data to be sent in the HTTP POST request

    if (http.begin(client, "http://" + String(serverAddress) + endpoint)) {
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      int httpCode = http.POST(postData);

      if (httpCode > 0) {
        payload = http.getString();
        Serial.print("HTTP code: ");
        Serial.println(httpCode);
        Serial.print("Response: ");
        Serial.println(payload);
      } else {
        Serial.println("Error in HTTP request");
      }

      http.end();
    } else {
      Serial.println("Failed to connect to server");
    }

    get_DHT11_sensor_data();

    delay(4000); // Delay before next iteration
  } else {
    Serial.println("WiFi connection lost. Reconnecting...");
    WiFi.begin(ssid, password); // Attempt to reconnect to WiFi
  }
}
