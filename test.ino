#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


const char *ssid = "Hardware-Lab";
const char *password = "H@rdw@re2o23";
const char *phpServer = "http://127.0.0.1/test.php";
WiFiClient client;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  // Your data to be sent
  String data = "words=HELLO WEBSITE";

  // Start the HTTP client
  WiFiClient client;
  HTTPClient http;
  http.begin(client, phpServer);  // Use ::begin(WiFiClient, url) instead

  // Set content type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Send the POST request
  int httpResponseCode = http.POST(data);

  // Check for a successful response
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    // Print the response payload
    String payload = http.getString();
    Serial.println("Response payload: " + payload);
  } else {
    Serial.print("HTTP Request failed. Error code: ");
    Serial.println(httpResponseCode);

    // Print additional error details
    Serial.println("Error detail: " + http.errorToString(httpResponseCode));
  }

  // End the HTTP client
  http.end();

  // Wait for some time before sending the next request
  delay(5000);
}
