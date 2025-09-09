#include <Wire.h>
#include <MPU6050.h>
#include <WiFi.h>
#include <PubSubClient.h>

// Wi-Fi Credentials
const char* ssid = "your_wifi_ssid";  // Replace with your Wi-Fi SSID
const char* password = "your_wifi_password";  // Replace with your Wi-Fi password

// MQTT Configuration
const char* mqtt_server = "192.168.0.10";  // Replace with your MQTT broker IP address
const char* topic = "robot/commands";

WiFiClient espClient;
PubSubClient client(espClient);
MPU6050 mpu;

// Variables to store sensor data
int16_t ax, ay, az, gx, gy, gz;

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
}

void setup() {
  Serial.begin(115200);

  // Initialize MPU6050
  Wire.begin();
  mpu.initialize();
  if (mpu.testConnection()) {
    Serial.println("MPU6050 connection successful");
  } else {
    Serial.println("MPU6050 connection failed");
    while (1); // Halt execution if MPU6050 fails
  }

  // Initialize Wi-Fi and MQTT
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  // Wait for MQTT connection
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("GloveController")) {
      Serial.println("Connected to MQTT broker");
    } else {
      delay(500);
      Serial.print(".");
    }
  }
}

void loop() {
  // Reconnect to MQTT broker if connection is lost
  if (!client.connected()) {
    while (!client.connect("GloveController")) {
      Serial.println("Reconnecting to MQTT...");
      delay(500);
    }
  }

  // Read sensor data
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Gesture detection logic
  if (ax > 5000) {
    client.publish(topic, "left");
    Serial.println("Gesture: Left");
  } else if (ax < -5000) {
    client.publish(topic, "right");
    Serial.println("Gesture: Right");
  } else if (ay > 5000) {
    client.publish(topic, "forward");
    Serial.println("Gesture: Forward");
  } else if (ay < -5000) {
    client.publish(topic, "backward");
    Serial.println("Gesture: Backward");
  } else {
    client.publish(topic, "stop");
    Serial.println("Gesture: Stop");
  }

  delay(100); // Adjust delay for responsiveness
}