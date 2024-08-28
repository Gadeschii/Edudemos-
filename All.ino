#include <DHT.h>
#include <Arduino.h>
#include <ESP32Servo.h>
#include <ArduinoOTA.h>
#include <WiFi.h>

// DHT Sensor setup
const int DHTPin = 32; // Define the pin connected to the DHT11 sensor
#define DHTTYPE DHT11   // Define the type of DHT sensor
DHT dht(DHTPin, DHTTYPE); // Initialize the DHT sensor

// LED pin definitions
const int ledPin1Cold = 18;
const int ledPin2Good = 19;
const int ledPin3Heat = 21;

// LDR and Servo setup
const int ldrPin1 = 34; // Assuming this is the pin for the first LDR
const int ldrPin2 = 35; // Assuming this is the pin for the second LDR
const int servoPin = 33; // Defines the pin to which the servo is connected
const int OFFSET = 75;
Servo myServo; // Creates a Servo object

// WiFi credentials
const char* ssid = "WorkinCholas";
const char* password = "WiC!2023";

void setup() {
  Serial.begin(115200); // Start serial communication at 115200 baud rate
  setupMain(); // Setup main functionalities including WiFi connection
  setupAccion(); // Call to the new initialization function from accion.ino
  setupDHT(); // Setup for DHT sensor
}

void loop() {
  loopMain(); // Main loop function
  loopAccion(); // Call to the new loop function from accion.ino
  loopDHT(); // Read and print DHT sensor values
}

void setupMain() {
  // Connect to the WiFi network
  WiFi.begin(ssid, password);
  Serial.print("Connecting.");
  while (WiFi.status() != WL_CONNECTED) { // Wait for the connection to establish
    delay(500); // Delay to prevent spamming
    Serial.print("."); // Print dots to indicate the connection process
  }
  Serial.println("Connected to WiFi"); // Print when connected

  // Initialize ArduinoOTA with the hostname and optional password
  ArduinoOTA.setHostname("ESP32-OTA"); // Set the device's name for OTA updates
  // ArduinoOTA.setPassword("admin"); // Uncomment to set a password for OTA updates

  // Define OTA event handlers
  ArduinoOTA.onStart([]() {
    Serial.println("Starting OTA update..."); // Called when the OTA update starts
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA Update Finished"); // Called when the OTA update ends
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100))); // Called to show OTA update progress
  });

  ArduinoOTA.begin(); // Start the OTA service
}

void loopMain() {
  ArduinoOTA.handle(); // Handle OTA updates
}

void setupDHT() {
  dht.begin(); // Starts the DHT sensor

  // Setup LED pins as outputs
  pinMode(ledPin1Cold, OUTPUT);
  pinMode(ledPin2Good, OUTPUT);
  pinMode(ledPin3Heat, OUTPUT);
}

void loopDHT() {
  // Reading temperature or humidity takes about 250 milliseconds!
  float humidity = dht.readHumidity(); // Read humidity
  float temperature = dht.readTemperature(); // Read temperature as Celsius

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // Example LED control based on temperature
  if (temperature < 20) {
    digitalWrite(ledPin1Cold, HIGH); // Turn on LED 1 for temp < 20°C
    digitalWrite(ledPin2Good, LOW);
    digitalWrite(ledPin3Heat, LOW);
    Serial.println("Cold is HIGH");
  } else if (temperature >= 20 && temperature < 25) {
    digitalWrite(ledPin1Cold, LOW);
    digitalWrite(ledPin2Good, HIGH); // Turn on LED 2 for 20°C <= temp < 25°C
    digitalWrite(ledPin3Heat, LOW);
    Serial.println("Good is HIGH");
  } else {
    digitalWrite(ledPin1Cold, LOW);
    digitalWrite(ledPin2Good, LOW);
    digitalWrite(ledPin3Heat, HIGH); // Turn on LED 3 for temp >= 25°C
    Serial.println("Heat is HIGH");
  }

  // Print the humidity and temperature values to the serial monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%, Temp: ");
  Serial.print(temperature);
  Serial.println(" Celsius");
}

void setupAccion() {
  Serial.begin(115200); // Starts serial communication at 115200 baud
  pinMode(ldrPin1, INPUT); // Sets the first LDR pin as input
  pinMode(ldrPin2, INPUT); // Sets the second LDR pin as input
  myServo.attach(servoPin); // Associates the Servo object with the servo pin
  dht.begin(); // Starts the DHT sensor
}

void loopAccion() {
  int valueLdrPin1 = analogRead(ldrPin1) - OFFSET; // Reads the value from the first LDR
  int valueLdrPin2 = analogRead(ldrPin2); // Reads the value from the second LDR

  // Calculates the difference between the LDR values
  int difference = abs(valueLdrPin1 - valueLdrPin2);

  // Maps the difference to a range of 0 to 180 degrees
  int angle = map(difference, 0, 4095, 0, 180); // Assuming the reading range of the LDRs is 0 to 4095

  // Moves the servo to the calculated angle
  myServo.write(angle);

  // Sends the read values and the calculated angle to the serial monitor
  Serial.print("LDR1 Value: ");
  Serial.print(valueLdrPin1);
  Serial.print(", LDR2 Value: ");
  Serial.print(valueLdrPin2);
  Serial.print(", Servo Angle: ");
  Serial.println(angle);

  delay(1000); // Waits one second before the next reading
}