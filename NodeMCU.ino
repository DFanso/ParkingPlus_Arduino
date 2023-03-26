#include <Servo.h>
#include <SoftwareSerial.h>

#define SERVO_PIN D1   // Define the servo motor pin
#define NODEMCU_RX D2  // Define the NodeMCU RX pin
#define NODEMCU_TX D3  // Define the NodeMCU TX pin
#define TRIG_PIN D4
#define ECHO_PIN D5
#define MAX_DISTANCE 30 // Define the maximum distance (in cm) to trigger the servo

Servo servo;  // Create a Servo object

SoftwareSerial esp32_Serial(NODEMCU_RX, NODEMCU_TX);  // Set up a SoftwareSerial object for communication with ESP32-CAM

void setup() {
  Serial.begin(9600);
  esp32_Serial.begin(9600);

  servo.attach(SERVO_PIN);  // Initialize the servo motor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  long duration, distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 2) / 29.1;
  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance > 0 && distance < MAX_DISTANCE) {
    servo.write(0);    // Set servo to open position (0 degrees)
    delay(15000);      // Wait for 15 seconds
    servo.write(180);  // Set servo to closed position (180 degrees)
  }

  if (esp32_Serial.available()) {
    String command = esp32_Serial.readStringUntil('\n');
    command.trim();
    Serial.print(command);

    if (command == "OPEN") {
      servo.write(0);    // Set servo to open position (0 degrees)
      delay(15000);      // Wait for 15 seconds
      servo.write(180);  // Set servo to closed position (180 degrees)
    }
  }
}
