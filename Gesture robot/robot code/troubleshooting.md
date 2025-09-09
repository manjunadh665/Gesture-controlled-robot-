---

# **Troubleshooting Guide**

This guide will help you resolve common issues you may encounter during the setup and operation of the gesture-controlled robot project. The issues covered here include sensor calibration problems and Wi-Fi/MQTT connection issues. Please follow the suggested solutions step by step to resolve these problems.

---

## **1. Sensor Calibration Issues (MPU6050)**

### **Problem:**
The hand gestures are not being recognized properly, or the robot moves unexpectedly based on incorrect tilt values from the MPU6050 sensor.

### **Solution:**

The MPU6050 sensor needs to be calibrated to ensure accurate readings. Calibration adjusts the sensor’s readings to a known baseline so that the gestures are detected correctly.

#### **Steps to Calibrate the MPU6050:**

1. **Ensure the Sensor is Connected Correctly:**
   - Double-check the wiring of the **MPU6050** to the **ESP32**.
     - **VCC** to **3.3V** (or **5V** if your sensor is rated for it).
     - **GND** to **GND**.
     - **SCL** to **GPIO 22** (default for ESP32).
     - **SDA** to **GPIO 21** (default for ESP32).
   - Ensure there are no loose connections.

2. **Install the Correct Library:**
   - Ensure you are using the correct MPU6050 library. The **MPU6050** library by **Jrowberg** is commonly used for easy integration:
     - In Arduino IDE, go to **Sketch > Include Library > Manage Libraries** and search for **MPU6050** by **Jrowberg** and install it.

3. **Run Calibration Code:**
   You can use the following code to print the sensor’s raw accelerometer and gyroscope values to monitor its readings and check if the sensor is functioning properly.

   ```cpp
   #include <Wire.h>
   #include <MPU6050.h>

   MPU6050 mpu;

   void setup() {
     Serial.begin(115200);
     Wire.begin();
     mpu.initialize();

     if (mpu.testConnection()) {
       Serial.println("MPU6050 connection successful.");
     } else {
       Serial.println("MPU6050 connection failed.");
       while (1);
     }
   }

   void loop() {
     int16_t ax, ay, az, gx, gy, gz;
     mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

     Serial.print("Accelerometer X: "); Serial.print(ax);
     Serial.print(" Y: "); Serial.print(ay);
     Serial.print(" Z: "); Serial.println(az);
     Serial.print("Gyroscope X: "); Serial.print(gx);
     Serial.print(" Y: "); Serial.print(gy);
     Serial.print(" Z: "); Serial.println(gz);

     delay(500);
   }
   ```
   - Watch the output in the **Serial Monitor**. When the sensor is in a neutral position, the accelerometer should show values close to **0** for the X and Y axes (or a small positive value if tilted), and **1** for the Z axis (assuming gravity is in the positive Z direction).

4. **Calibrate the Sensor:**
   - If you see any large offsets in the readings, try the following:
     - Place the sensor on a flat surface and record the accelerometer’s X, Y, and Z values. These values are typically offset and need to be adjusted in the code.
     - In your sketch, subtract the recorded offsets from the sensor’s raw readings:
       ```cpp
       ax = ax - offsetX;
       ay = ay - offsetY;
       az = az - offsetZ;
       ```

5. **Re-test Gesture Sensitivity:**
   After calibrating, upload the gesture control code and test if the gestures (left, right, forward, backward) are being recognized correctly.

---

## **2. Wi-Fi Connection Issues**

### **Problem:**
The ESP32 is not connecting to the Wi-Fi network, or the connection is unstable.

### **Solution:**

1. **Check Wi-Fi Credentials:**
   - Double-check the **SSID** and **password** in the code. If there’s a typo or incorrect credentials, the ESP32 will not be able to connect.
   - Example:
     ```cpp
     const char* ssid = "your_wifi_ssid";
     const char* password = "your_wifi_password";
     ```

2. **Check Wi-Fi Range and Signal Strength:**
   - Ensure the ESP32 is within range of the Wi-Fi router. Weak Wi-Fi signals may cause the ESP32 to fail in connecting.
   - Use a Wi-Fi signal strength checker app to verify the signal strength in the area where the ESP32 is located.

3. **Check for IP Conflicts:**
   - If your router assigns IP addresses dynamically (via DHCP), make sure there are no conflicts. If multiple devices are assigned the same IP, the ESP32 may fail to connect.
   - You can also set a static IP in your ESP32 code to avoid this issue:
     ```cpp
     IPAddress local_IP(192, 168, 1, 50);  // Set a static IP address
     IPAddress gateway(192, 168, 1, 1);    // Set the gateway
     IPAddress subnet(255, 255, 255, 0);   // Set the subnet mask
     WiFi.config(local_IP, gateway, subnet);
     ```

4. **Debugging with Serial Output:**
   - Add `Serial.print(WiFi.localIP());` in your `setup()` function to print the IP address once the ESP32 successfully connects to Wi-Fi. This can help you verify the connection.
   - Use the **Serial Monitor** to watch the connection status. If you see continuous "Attempting to connect..." messages, it means the ESP32 is not able to connect to the network.

---

## **3. MQTT Connection Issues**

### **Problem:**
The ESP32 fails to connect to the MQTT broker, or the connection is unstable.

### **Solution:**

1. **Verify MQTT Broker IP Address:**
   - Double-check that the **IP address** of the MQTT broker is correctly set in the code:
     ```cpp
     const char* mqtt_server = "192.168.1.100";  // Replace with your MQTT broker IP
     ```

2. **Check MQTT Broker Status:**
   - Ensure the MQTT broker (e.g., Mosquitto) is running on your local machine or server.
   - Test the broker using an MQTT client (e.g., MQTT.fx or MQTT Explorer) to verify that it is accessible and functioning properly.

3. **Check MQTT Port:**
   - The default MQTT port is **1883**. Ensure that your MQTT broker is listening on this port, or configure it to use another port in both the broker configuration and your code.

4. **Verify MQTT Client Credentials:**
   - If you’ve set up authentication on your MQTT broker (e.g., username/password), make sure these credentials are correctly set in your ESP32 code:
     ```cpp
     if (client.connect("ESP32Client", "username", "password")) {
       // Your MQTT connection code
     }
     ```

5. **Enable Debugging in MQTT Code:**
   - Add debugging statements in the MQTT code to understand where it’s failing. For example, check the connection status with `Serial.println(client.state());` to get the error code for the MQTT connection.

---

## **4. Robot Control Issues**

### **Problem:**
The robot does not respond to the commands sent from the glove, or it moves erratically.

### **Solution:**

1. **Check Motor Driver Connections:**
   - Verify that the **motor driver** is connected properly to the ESP32 and motors.
     - Ensure the **IN1, IN2, IN3, and IN4** pins (for L298N) are connected to the appropriate ESP32 GPIO pins.

2. **Verify MQTT Command Reception:**
   - Use **Serial Monitor** to confirm that the ESP32 on the robot is receiving the correct MQTT commands (e.g., “left”, “right”, “forward”, etc.). If the robot is receiving unexpected messages, check the gesture glove’s code for correct message publishing.

3. **Test Motor Driver with Direct Commands:**
   - Bypass the MQTT control and test the motor driver by sending simple high/low signals to the motor driver’s control pins from the ESP32. This helps confirm that the motor driver is functioning correctly.

---

## **Conclusion**

This troubleshooting guide covers common issues related to the MPU6050 sensor calibration, Wi-Fi connection, MQTT setup, and robot control. By following the suggested steps, you should be able to diagnose and fix the problems you encounter during the setup of the **gesture-controlled robot** project.

If you still experience issues, feel free to consult the official documentation for **Mosquitto**, **MPU6050**, or the **ESP32**, or reach out for community support.
