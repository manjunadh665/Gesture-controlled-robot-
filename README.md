Gesture-Controlled Robot Using ESP32
Overview
This project demonstrates how to build a gesture-controlled robot using ESP32 microcontrollers and an MPU6050 sensor mounted on a glove. The robot is controlled wirelessly via MQTT, and its movements (forward, backward, left, right, and stop) are dictated by hand gestures. This interactive project is perfect for learning about embedded systems, wireless communication, and gesture recognition.

Features
Real-Time Gesture Recognition: Leverages the MPU6050 accelerometer and gyroscope to detect tilts and movements.
Wireless Communication: Uses MQTT protocol for seamless communication between the glove and the robot.
Customizable Movement Control: Easily adapt the code to add more gestures or actions.
Beginner-Friendly Design: Simple hardware setup and modular code for easy replication and learning.
Hardware Requirements
ESP32 Microcontroller (x2): One for the glove and one for the robot.
MPU6050 Sensor: For detecting hand gestures.
Motor Driver Module (L298N or similar): To control DC motors.
Motors (Servo or DC): For robot movement.
Robot Chassis: To mount the motors and ESP32.
Power Sources: Batteries for the ESP32 and motors.
Glove: To mount the MPU6050 sensor for gesture detection.
Additional Components: Jumper wires, resistors, and capacitors as needed.
Software Requirements
Arduino IDE: To upload code to the ESP32 boards.
MQTT Broker: For communication between the glove and the robot (e.g., Mosquitto or a cloud-based MQTT broker).
How It Works
Gesture Recognition:

The glove is equipped with an MPU6050 sensor connected to an ESP32.
The ESP32 reads data from the MPU6050 to detect tilt gestures (e.g., forward, backward, left, right).
Based on the detected gesture, the ESP32 sends corresponding commands via MQTT.
Robot Movement:

Another ESP32 on the robot receives the MQTT commands.
These commands control the motor driver, moving the robot in the desired direction or stopping it.
Steps to Replicate
Assemble the Hardware:

Mount the motors, motor driver, and ESP32 onto the robot chassis.
Attach the MPU6050 sensor to a glove and connect it to another ESP32.
Set Up the MQTT Broker:

Configure a local or cloud-based MQTT broker and update the ESP32 code with its address.
Upload the Code:

Flash the gesture detection code to the glove's ESP32.
Flash the robot control code to the robot's ESP32.
Test and Calibrate:

Test the system to ensure gestures are correctly detected and commands are received by the robot.
Adjust sensitivity in the code if needed.
Circuit Diagrams
The repository includes circuit diagrams for:

Connecting the MPU6050 to the glove's ESP32.
Interfacing the motor driver and motors with the robot's ESP32.
Applications
Robotics projects for education and hobbyists.
Hands-on experience with IoT and wireless communication.
Prototype for assistive technologies or remote-controlled devices.
Future Enhancements
Add obstacle detection with ultrasonic sensors.
Include haptic feedback in the glove for enhanced interactivity.
Develop additional gestures for complex robot tasks
