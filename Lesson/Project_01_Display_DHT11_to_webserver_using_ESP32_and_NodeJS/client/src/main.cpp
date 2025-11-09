#include <Arduino.h>
#include "WiFi.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <HTTPClient.h>

const char* ssid     = "CAFE CAY BANG";
const char* password = "113113113";
const char* url = "http://192.168.100.7:3000/data"; // Server URL to send data

#define DHT_PIN 2
#define DHTTYPE DHT11

DHT dht(DHT_PIN, DHTTYPE);

void setup() {
  Serial.begin(9600);                      // Start serial communication at 9600 baud rate
  Serial.println("\nStarting WiFi connection...");

  WiFi.begin(ssid, password);              // Connect to WiFi network
  dht.begin();                             // Initialize DHT sensor

  // Wait until connected
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("..");                    // Print dots while connecting
    delay(100);
  } 

  Serial.println("\nConnected to WiFi network.");
  Serial.println(WiFi.localIP());          // Print ESP32 local IP address
}

void loop() {
  // Read temperature and humidity from DHT11 sensor
  float temp = 0;
  float hum = 0;
  if ((dht.readTemperature()) && (dht.readHumidity())) {
    temp = dht.readTemperature();          // Read temperature in Celsius
    hum = dht.readHumidity();              // Read humidity percentage

    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print(" °C, Humidity: ");
    Serial.print(hum);
    Serial.println(" %");
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }
  
  // Send data to server by post request
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url);                 // Specify destination for HTTP request
    http.addHeader("Content-Type", "application/json"); // Specify content-type header
    String jsonData = "{\"temp\": " + String(temp, 2) + ", \"hum\": " + String(hum, 2) + "}";
    int res = http.POST(jsonData);          // Send the request
    if (res > 0) {
      Serial.print("Send successfully\n");
      Serial.println(http.getString());                  // Print HTTP response code
    } else {
      Serial.print("Send failed\n");
      Serial.println(http.getString());
    }
    http.end();                            // Free resources
  }
  delay(5000);                           // Wait for a minute before next reading
}

