#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>

// Replace with your network credentials
const char* ssid = "abc";
const char* password = "abcd1234";

// Replace with your server's IP address and port
const char* serverUrl = "http://20.2.80.190:3000/upload";

// Pin definition for the AI-THINKER ESP32-CAM module
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define ESP32_TX_PIN 14 // Define the ESP32-CAM TX pin
#define ESP32_RX_PIN 12 // Define the ESP32-CAM RX pin

HardwareSerial nodeMCU_Serial(2); // Create a HardwareSerial object for communication with NodeMCU

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_UXGA;
  config.jpeg_quality = 15;
  config.fb_count = 2;

  // Set up the serial communication with NodeMCU
  nodeMCU_Serial.begin(9600, SERIAL_8N1, ESP32_RX_PIN, ESP32_TX_PIN);

  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Wi-Fi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
return;
}

if (WiFi.status() == WL_CONNECTED) {
HTTPClient http;

http.begin(serverUrl);
http.addHeader("Content-Type", "multipart/form-data; boundary=esp32_boundary");
String startBoundary = "--esp32_boundary\r\nContent-Disposition: form-data; name=\"image\"; filename=\"picture.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
String endBoundary = "\r\n--esp32_boundary--\r\n";
int httpCode = http.POST(startBoundary + String((const char*)fb->buf, fb->len) + endBoundary);

if (httpCode > 0) {
  String payload = http.getString();
  Serial.println("Server Response: ");
  Serial.println(payload);

  // Check if the server response equals "true"
  if (payload == "true") {
    nodeMCU_Serial.print("OPEN"); // Send the "OPEN" command to the NodeMCU
  }
} else {
  Serial.printf("Error sending image to server, HTTP code: %d\n", httpCode);
}
http.end();
} else {
Serial.println("Error in WiFi connection");
}

esp_camera_fb_return(fb);

delay(5000); // Wait for 5 seconds before taking the next picture
}