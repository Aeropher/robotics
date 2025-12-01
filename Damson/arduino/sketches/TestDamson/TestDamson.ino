/*
 * TestDamson.ino
 *
 * Basic test sketch to verify ProjectDamson library compiles and runs.
 * This is equivalent to the original Freenove sketch but uses the renamed library.
 *
 * Usage:
 * 1. Run build.bat to deploy the library
 * 2. Open this sketch in Arduino IDE
 * 3. Select Board: Arduino Mega 2560
 * 4. Select correct COM port
 * 5. Upload
 *
 * The robot should boot to its default position.
 * You can then control it via:
 * - Processing app (Damson/processing/)
 * - Freenove phone app (connect to WiFi "Damson", password "Freenove")
 * - Serial commands at 115200 baud
 */

#include <ProjectDamson.h>

ProjectDamson damson;

void setup() {
  // Signature startup: blink LED 5 times fast to confirm Damson firmware
  pinMode(13, OUTPUT);
  for (int i = 0; i < 5; i++) {
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(100);
  }

  // Start with communication enabled (Serial + WiFi)
  damson.Start(true);
}

void loop() {
  // Handle incoming commands from Serial/WiFi
  damson.Update();
}
