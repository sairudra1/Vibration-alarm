#include "BluetoothSerial.h"  // Bluetooth Serial library

// Create a BluetoothSerial object
BluetoothSerial SerialBT;

// Define the GPIO pins where motors are connected
const int motor1 = 32;
const int motor2 = 33;
const int motor3 = 25;
const int motor4 = 26;
const int motor5 = 27;

// Duration for motor ON and OFF (in milliseconds)
const int delayTime = 1000;  // Adjust the time as needed

int delayrange;

void setup() {
  // Initialize Bluetooth Serial
  Serial.begin(115200);
  SerialBT.begin("vibealert");  // Bluetooth device name

  //SerialBT.println("Bluetooth Started");
  Serial.println("Serial communication started");

  delayrange = 1;  // Initialize delayrange

  // Set the motor pins as outputs
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(motor3, OUTPUT);
  pinMode(motor4, OUTPUT);
  pinMode(motor5, OUTPUT);

  // Optional: Set up the default state of the motors (OFF)
  digitalWrite(motor1, LOW);
  digitalWrite(motor2, LOW);
  digitalWrite(motor3, LOW);
  digitalWrite(motor4, LOW);
  digitalWrite(motor5, LOW);

  Serial.println("Setup complete, motors are OFF.");
}

void loop() {
  // Check if data is available to read from Bluetooth Serial
  if (SerialBT.available()) {
    char command = SerialBT.read();  // Read the incoming byte
    Serial.println("Command received: " + String(command));

    if (command == 'B') {
      //  SerialBT.println("Received B: Rotating motors in Pattern A");
      Serial.println("Executing Pattern B");

      // Pattern A: All motors ON, then OFF in sequence
      digitalWrite(motor1, HIGH);
      digitalWrite(motor2, HIGH);
      digitalWrite(motor3, HIGH);
      digitalWrite(motor4, HIGH);
      digitalWrite(motor5, HIGH);
      delay(delayrange * delayTime);  // Keep all ON for a while

      digitalWrite(motor5, LOW);
      delay(delayTime);
      digitalWrite(motor4, LOW);
      delay(delayTime);
      digitalWrite(motor3, LOW);
      delay(delayTime);
      digitalWrite(motor2, LOW);
      delay(delayTime);
      digitalWrite(motor1, LOW);
      delay(delayTime);

      Serial.println("Pattern B complete");

    } else if (command == 'A') {
      //  SerialBT.println("Received A: Rotating motors in Pattern B");
      Serial.println("Executing Pattern A");

      // Pattern B: Motors ON and OFF alternately
      digitalWrite(motor1, HIGH);
      digitalWrite(motor3, HIGH);
      digitalWrite(motor5, HIGH);
      digitalWrite(motor2, HIGH);
      digitalWrite(motor4, HIGH);
      delay(1000);

      digitalWrite(motor1, LOW);
      digitalWrite(motor3, LOW);
      digitalWrite(motor5, LOW);
      digitalWrite(motor2, LOW);
      digitalWrite(motor4, LOW);

      Serial.println("Pattern A complete");

    } else if (command == '1') {
      //   SerialBT.println("Received 1: Setting delay range to 1");
      Serial.println("Setting delayrange = 1");
      delayrange = 1;
      delay(100);

    } else if (command == '2') {
      //   SerialBT.println("Received 2: Setting delay range to 2");
      Serial.println("Setting delayrange = 2");
      delayrange = 2;
      delay(100);

    } else if (command == '3') {
      //   SerialBT.println("Received 3: Setting delay range to 3");
      Serial.println("Setting delayrange = 3");
      delayrange = 3;
      delay(100);

    } else if (command == '4') {
      //   SerialBT.println("Received 4: Setting delay range to 4");
      Serial.println("Setting delayrange = 4");
      delayrange = 4;
      delay(100);
    }
  }
  // Print the current delayrange every loop for debugging
  Serial.println("Current delayrange: " + String(delayrange));
  delay(1000);  // Slight delay to avoid flooding the serial monitor
}
