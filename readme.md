#Parking_plus_Arduino

#NodeMCU_Parking code purpose

uses an ultrasonic sensor, two IR sensors, and a servo motor to detect and control the parking slot's occupancy. The code also sends occupancy data to a server for monitoring purposes.

The code first initializes the pins for the ultrasonic sensor, IR sensors, and servo motor. It also creates a Servo object for controlling the servo motor. The constants for the object threshold and open time are also defined.

In the setup() function, the code connects to Wi-Fi and prints the connection details. In the loop() function, the code measures the distance to an object using the ultrasonic sensor and determines if an object is within the defined threshold. If an object is detected, the code checks the state of the two IR sensors. If at least one IR sensor detects an object, the servo motor is triggered to open the parking slot. After 15 seconds, the servo motor is triggered to close the parking slot.

The code also sends data to a server for monitoring purposes. If both IR sensors detect an object, the code sends "true" to the server. If not, the code sends "false" to the server.

The measureDistance() function measures the distance to an object using the ultrasonic sensor.

The sendDataToServer() function sends the occupancy data to the server. If the Wi-Fi connection is established, the code sends a GET request to the server's endpoint. If the request is successful, the server's response is printed. If not, an error message is printed.

Overall, this code provides a basic implementation of a parking system that uses various sensors and a servo motor to control the occupancy of a parking slot and sends data to a server for monitoring purposes.

#ESP32 Cam code purpose

capture an image using an ESP32-CAM module and send it to a server using Wi-Fi. The captured image is in JPEG format, and the code uses the esp_camera library to configure the camera and capture the image. The captured image is then sent to a server using an HTTP POST request. The server address and port are defined in the serverUrl variable.

The code initializes the camera configuration using the camera_config_t structure, which includes pins definition and other camera parameters such as resolution and JPEG quality. The Wi-Fi network is then connected using the WiFi.begin() function, and the code waits until the connection is established.

In the loop() function, the code captures an image using the esp_camera_fb_get() function and sends it to the server using an HTTP POST request. The HTTP POST request includes a boundary string and the image data in binary format. If the server responds with the word "true", the code sends the command "OPEN" to a connected NodeMCU module using the HardwareSerial object nodeMCU_Serial. If the HTTP POST request fails or the Wi-Fi connection is lost, an error message is displayed.

Finally, the captured frame buffer is released using the esp_camera_fb_return() function, and the code waits for 5 seconds before taking the next picture.

#NodeMCU code purpose

This code uses an ultrasonic sensor and a servo motor to control the opening and closing of a door. The ultrasonic sensor is used to detect if an object is within a certain distance from the sensor, and if so, it triggers the servo motor to open the door.

The code initializes the Servo and SoftwareSerial objects, and defines the pins used for the servo motor, ultrasonic sensor, and ESP32-CAM communication. The maximum distance to trigger the servo is also defined.

In the loop() function, the ultrasonic sensor is triggered to measure the distance to the nearest object. If the distance is within the range of 0 to MAX_DISTANCE, the servo motor is triggered to open the door by moving to the 0-degree position, waiting for 15 seconds, and then moving back to the 180-degree position.

Additionally, the code listens for commands received from the ESP32-CAM module through the SoftwareSerial object. If the command is "OPEN", the servo motor is triggered to open the door in the same manner as described above.

The code provides real-time distance measurements using the Serial.print() function, and also displays any commands received from the ESP32-CAM module.

Overall, this code demonstrates a basic implementation of using an ultrasonic sensor and a servo motor to control the opening and closing of a door.
