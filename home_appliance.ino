#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


#define RELAY_PIN 5  // The ESP8266 pin connected to Relay
#define SENSOR_PIN A0
#define TRIG_PIN 14
#define ECHO_PIN 12
#define WATER_PUMP_L_PIN 13
#define WATER_PUMP_M_PIN 15
const char* ssid = "Hardware-Lab";
const char* password = "H@rdw@re2o23";

ESP8266WebServer server(80); // Web server on port 80

int relay_state = LOW;
int water_pumpL= LOW;
int water_pumpM=LOW;

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(SENSOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(WATER_PUMP_L_PIN, OUTPUT);
  pinMode(WATER_PUMP_M_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, relay_state);
  digitalWrite(WATER_PUMP_L_PIN, water_pumpL);
  digitalWrite(WATER_PUMP_M_PIN, water_pumpM);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", getHTML());
  });

  server.on("/relay/off", HTTP_GET, []() {
    relay_state = LOW;
    digitalWrite(RELAY_PIN, relay_state);
    server.send(200, "text/html", getHTML());
  });

  server.on("/relay/on", HTTP_GET, []() {
    relay_state = HIGH;
    digitalWrite(RELAY_PIN, relay_state);
    server.send(200, "text/html", getHTML());
  });

  server.on("/water_pumpL/off", HTTP_GET, []() {
    water_pumpL = LOW;
    digitalWrite(WATER_PUMP_L_PIN, water_pumpL);
    server.send(200, "text/html", getHTML());
  });

  server.on("/water_pumpL/on", HTTP_GET, []() {
    water_pumpL = HIGH;
    digitalWrite(WATER_PUMP_L_PIN, water_pumpL);
    server.send(200, "text/html", getHTML());
  });

  server.on("/water_pumpM/off", HTTP_GET, []() {
    water_pumpM = LOW;
    digitalWrite(WATER_PUMP_M_PIN, water_pumpM);
    server.send(200, "text/html", getHTML());
  });

  server.on("/water_pumpM/on", HTTP_GET, []() {
    water_pumpM = HIGH;
    digitalWrite(WATER_PUMP_M_PIN, water_pumpM);
    server.send(200, "text/html", getHTML());
  });
  
  server.on("/values", HTTP_GET, []() {
    String json = "{\"distance\": " + String(getDistance()) + ", \"moisture\": " + String(getMoisture()) + "}";
    server.send(200, "application/json", json);
  });
  
  // Start the server
  server.begin();
}

void loop() {
  server.handleClient();
}

float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  float ans=duration * 0.034 / 2;
  if(ans>20){
    water_pumpL = LOW;
    digitalWrite(WATER_PUMP_L_PIN, water_pumpL);
  }else if(ans<10){
    water_pumpL = HIGH;
    digitalWrite(WATER_PUMP_L_PIN, water_pumpL);
  }
  return ans; // Convert duration to distance in cm
}

int getMoisture() {
  int moistureValue = analogRead(SENSOR_PIN);
  int ans=map(moistureValue, 550, 10, 0, 100);
  if(ans<0){
    water_pumpM = LOW;
    digitalWrite(WATER_PUMP_M_PIN, water_pumpM);
  }else{
    water_pumpM = HIGH;
    digitalWrite(WATER_PUMP_M_PIN, water_pumpM);
  }// Map sensor value to percentage
  return ans;
}
String getHTML() {
  String html = "<!DOCTYPE HTML>";
  html += "<html>";
  html += "<head>";
  html += "<title>Home Appliance</title>";
  html += "<link rel='icon' href='data:,' />";
  html += "<script>";
  html += "function updateValues() {";
  html += "var xhttp = new XMLHttpRequest();";
  html += "xhttp.onreadystatechange = function() {";
  html += "if (this.readyState == 4 && this.status == 200) {";
  html += "var response = JSON.parse(this.responseText);";
  html += "document.getElementById('distance').innerHTML = response.distance;";
  html += "document.getElementById('moisture').innerHTML = response.moisture;";
  html += "if(response.distance>20){";
  html += "document.getElementById('distanceStatus').innerHTML = 'ON';}";
  html += "else if(response.distance<10){";
  html += "document.getElementById('distanceStatus').innerHTML = 'OFF';}";
  html += "if(response.moisture<0){";
  html += "document.getElementById('moistureStatus').innerHTML = 'ON';}";
  html += "else{";
  html += "document.getElementById('moistureStatus').innerHTML = 'OFF';}";
  html += "}";
  html += "};";
  html += "xhttp.open('GET', '/values', true);";
  html += "xhttp.send();";
  html += "}";
  html += "setInterval(updateValues, 1000);";
  html += "</script>";
  html += "</head>";
  html += "<body style='margin:auto;width:50%'>";
  html += "<h1 style='text-align: center;'>Home Appliance</h1>";
  //////////////////////
  html += "<div style='float:left;padding:20px;border:1px solid;width:26%;text-align: center;'>";
  html += "<p>Blub</p>";
  html += "<p>Relay state: <span style='color: red;'>";
  if (relay_state == LOW)
    html += "ON";
  else
    html += "OFF";

  html += "</span></p>";
  html += "<a href='/relay/on'><button>Turn OFF</button></a>";
  html += "<a href='/relay/off' style='margin-left:10px'><button>Turn ON</button></a>";
  html += "</div>";
/////////////////////////////
  html += "<div style='float:left;padding:20px;margin-left:20px;border:1px solid;width:26%;text-align: center;'>";
  html += "<p>Water level : <span id='distance'>Loading...</span> cm</p>";
  html += "<p>water_pumpL: <span id='distanceStatus' style='color: red;'>";
  if (water_pumpL == LOW)
    html += "ON";
  else
    html += "OFF";

  html += "</span></p>";
  html += "<a href='/water_pumpL/on'><button>Turn OFF</button></a>";
  html += "<a href='/water_pumpL/off' style='margin-left:10px'><button>Turn ON</button></a>";
  html += "</div>";
////////////////////////////
  html += "<div style='float:left;padding:20px;margin-left:20px;border:1px solid;width:26%;text-align: center;'>";
  html += "<p>Moisure : <span id='moisture'>Loading...</span> %</p>";
  html += "<p>water_pumpM: <span id='moistureStatus' style='color: red;'>";
  if (water_pumpM == LOW)
    html += "ON";
  else
    html += "OFF";

  html += "</span></p>";
  html += "<a href='/water_pumpM/on'><button>Turn OFF</button></a>";
  html += "<a href='/water_pumpM/off' style='margin-left:10px'><button>Turn ON</button></a>";
  html += "</div>";
  
  html += "</body>";
  html += "</html>";

  return html;
}
