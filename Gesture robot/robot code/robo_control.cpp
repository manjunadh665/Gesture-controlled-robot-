#include <WiFi.h>
#include <PubSubClient.h>
#include <L298N.h>

// Wi-Fi Credentials
const char* ssid = "your_wifi_ssid";  // Replace with your Wi-Fi SSID
const char* password = "your_wifi_password";  // Replace with your Wi-Fi password

// MQTT Configuration
const char* mqtt_server = "192.168.0.10";  // Replace with your MQTT broker IP address
const char* topic = "robot/commands";

WiFiClient espClient;
PubSubClient client(espClient);

// Motor driver setup
L298N motorDriver(5, 18, 19, 23);  // Define pins for motor control (adjust to your wiring)

// Robot movement commands
#define STOP 0
#define FORWARD 1
#define BACKWARD 2
#define LEFT 3
#define RIGHT 4

int currentCommand = STOP;

// Wi-Fi setup
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

// MQTT callback function to process received messages
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.println("Received message: " + message);

  if (message == "forward") {
    currentCommand = FORWARD;
  } else if (message == "backward") {
    currentCommand = BACKWARD;
  } else if (message == "left") {
    currentCommand = LEFT;
  } else if (message == "right") {
    currentCommand = RIGHT;
  } else if (message == "stop") {
    currentCommand = STOP;
  }
}

// Setup function
void setup() {
  Serial.begin(115200);
  setup_wifi();
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  motorDriver.begin();
}

// Reconnect to MQTT if disconnected
void reconnect() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("RobotController")) {
      Serial.println("Connected to MQTT broker");
      client.subscribe(topic);
    } else {
      delay(500);
      Serial.print(".");
    }
  }
}

// Loop function
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  
  client.loop();  // Listen for incoming MQTT messages
  
  // Execute robot commands based on gesture input
  switch (currentCommand) {
    case FORWARD:
      motorDriver.forward();
      Serial.println("Moving Forward");
      break;
    case BACKWARD:
      motorDriver.backward();
      Serial.println("Moving Backward");
      break;
    case LEFT:
      motorDriver.turnLeft();
      Serial.println("Turning Left");
      break;
    case RIGHT:
      motorDriver.turnRight();
      Serial.println("Turning Right");
      break;
    case STOP:
      motorDriver.stop();
      Serial.println("Stopping");
      break;
  }

  delay(100);  // Delay to manage command frequency
}