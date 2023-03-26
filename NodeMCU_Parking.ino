#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>


const char* ssid = "abc";
const char* password = "abcd1234";

// Ultrasonic sensor pins
const int trigPin = D1;
const int echoPin = D2;

// IR sensors pins
const int irSensor1Pin = D5;
const int irSensor2Pin = D6;

// Servo motor pin
const int servoPin = D7;

// Servo motor object
Servo servo;

// Constants
const int objectThreshold = 10; // in cm, change according to your needs
const unsigned long openTime = 15000; // 15 seconds


void setup() {
   pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(irSensor1Pin, INPUT);
  pinMode(irSensor2Pin, INPUT);
  servo.attach(servoPin);

  // Initialize serial communication
  Serial.begin(9600);
  delay(10);

  // Connect to WiFi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Print WiFi connection details
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int distanceToObject = measureDistance();
  Serial.print("Distance: ");
  Serial.print(distanceToObject);
  Serial.println(" cm");

  int irSensor1State = digitalRead(irSensor1Pin);
  int irSensor2State = digitalRead(irSensor2Pin);

  if (distanceToObject < objectThreshold) {

    Serial.print("IR Sensor 1: ");
    Serial.println(irSensor1State == LOW ? "Object Detected" : "No Object");
    Serial.print("IR Sensor 2: ");
    Serial.println(irSensor2State == LOW ? "Object Detected" : "No Object");

    if (irSensor1State == HIGH || irSensor2State == HIGH) {
      // No objects detected by IR sensors
      Serial.println("Opening servo motor...");
      servo.write(0); // Open servo motor
      delay(openTime); // Wait for 15 seconds
      Serial.println("Closing servo motor...");
      servo.write(90); // Close servo motor
    }
  }
 if (irSensor1State == LOW || irSensor2State == LOW) {
    sendDataToServer("true");
    delay(5000); // Add a 10-second delay after sending data to the server
  }
  else
  {
    sendDataToServer("false");
    delay(5000);
  }
  delay(100); // Short delay between readings
}

int measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.0344 / 2;

  return distance;
}

void sendDataToServer(String bothOccupied) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;

    // Replace with your Node.js server URL and endpoint
    String url = "http://20.2.80.190:3000/parking";
    url += "?bothOccupied=" + bothOccupied;

    http.begin(client, url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Server response: " + payload);
    } else {
      Serial.println("Error on sending request");
    }

    http.end();
  }
}
