#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DHT.h"

const char* ssid = "Hardware-Lab";     // Your WiFi SSID
const char* password = "H@rdw@re2o23"; // Your WiFi password

const int RELAY_PIN = 2;  // The GPIO pin connected to the IN pin of the relay
const int FLAME_SENSOR_PIN = 13; // Flame sensor pin
const int DHT_PIN = 4; // DHT sensor pin
const int DHT_TYPE = DHT11; // DHT sensor type (DHT11 or DHT22)
const int BUZZER_PIN = 14;      // Buzzer pin

DHT dht(DHT_PIN, DHT_TYPE);     // Initialize DHT sensor

ESP8266WebServer server(80);

String getHTML() {
  String html = "<!DOCTYPE HTML>";
  html += "<html>";
  html += "<head>";
  html += "<title>ESP8266 WITH MYSQL DATABASE</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.7.2/css/all.css' integrity='sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr' crossorigin='anonymous'>";
  html += "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.4/css/all.min.css'>";
  html += "<style>";
  html += "/* Your CSS styles here */";
  html += "</style>";
  html += "</head>";
  html += "<body>";
  html += "<div class='topnav'>";
  html += "<h3>ESP8266 WITH MYSQL DATABASE</h3>";
  html += "</div>";
  html += "<br>";
  html += "<div class='content'>";
  html += "<div class='cards'>";
  
  // Monitoring section for temperature and humidity
  html += "<div class='card'>";
  html += "<div class='card header'>";
  html += "<h3 style='font-size: 1rem;'>MONITORING</h3>";
  html += "</div>";
  html += "<h4 class='temperatureColor'><i class='fas fa-thermometer-half'></i> TEMPERATURE</h4>";
  html += "<p class='temperatureColor'><span class='reading'><span id='ESP8266_01_Temp'>" + String(getTemperature()) + "</span> &deg;C</span></p>";
  html += "<h4 class='humidityColor'><i class='fas fa-tint'></i> HUMIDITY</h4>";
  html += "<p class='humidityColor'><span class='reading'><span id='ESP8266_01_Humd'>" + String(getHumidity()) + "</span> &percnt;</span></p>";
  html += "</div>";
  
  // Controlling section for the fan
  html += "<div class='card'>";
  html += "<div class='card header'>";
  html += "<h3 style='font-size: 1rem;'>CONTROLLING</h3>";
  html += "</div>";
  html += "<h4 class='FanColor'><i class='fas fa-fan fan-icon'></i> Fan</h4>";
  html += "<label class='switch'>";
  html += "<input type='checkbox' id='ESP8266_01_Fan' onclick='GetTogBtnLEDState('ESP8266_01_Fan')'>";
  html += "<div class='sliderTS'></div>";
  html += "</label>";
  html += "</div>";
  
  html += "</div>";
  html += "</div>";
  html += "</body>";
  html += "</html>";
  
  return html;
}

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Initially turn off the relay

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Initially turn off the buzzer

  dht.begin(); // Initialize DHT sensor

  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", getHTML());
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  
  // Check for fire detection
  if (isFlameDetected()) {
    // Activate the buzzer alarm
    digitalWrite(BUZZER_PIN, HIGH); // You can adjust the frequency as needed
  } else {
    // Turn off the buzzer alarm
    digitalWrite(BUZZER_PIN,LOW);
  }
}

float getHumidity() {
  return dht.readHumidity(); // Read humidity from DHT sensor
}

float getTemperature() {
  return dht.readTemperature(); // Read temperature from DHT sensor
}

bool isFlameDetected() {
  return digitalRead(FLAME_SENSOR_PIN) == LOW; // Check flame sensor status
}
