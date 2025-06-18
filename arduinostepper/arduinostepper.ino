#include <Arduino.h>
#include "A4988.h"

#define MOTOR_STEPS 200 // number of steps for 360°
#define DIR_PIN 2
#define STEP_PIN 3

A4988 stepper(MOTOR_STEPS, DIR_PIN, STEP_PIN);

void setup() {
  Serial.begin(115200);
  while(!Serial){
    delay(1000);
  }
  // Set target motor RPM to 1RPM and microstepping to 1 (full step mode)
  Serial.println("Set target motor RPM to 30RPM");
  stepper.begin(30);
  Serial.println("Write TEST to verify +360° and -360°")
  Serial.println("Write TURN followed by number of degrees (signed direction) to move the motor");
  Serial.println("Write CLOSE to end serial communication");
}

void loop() {
  String message = Serial.readStringUntil('\n'); // read until new line
  if(message.startsWith("TURN ")){
    message.remove(0,5)
    int deg_rot = message.toInt()
    stepper.rotate(deg_rot)
  }
  else if(message == "TEST"){
    Serial.println("Turning +360 and -360");
    delay(3000);    
    // Tell motor to rotate 360 degrees
    stepper.rotate(360);
    delay(1000);
    //Serial.println("Turning -360");
    stepper.rotate(-360);
  }
  else if(message == "CLOSE"){
    Serial.end();
    return;
  }
  delayMicroseconds(1000);
}
