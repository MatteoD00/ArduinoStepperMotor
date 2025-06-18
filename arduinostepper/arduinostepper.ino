#include <Arduino.h>
#include "A4988.h"

#define MOTOR_STEPS 200 // number of steps for 360°
#define DIR_PIN 2
#define STEP_PIN 3
//#define LED_PIN 13

A4988 stepper(MOTOR_STEPS, DIR_PIN, STEP_PIN);

void setup() {
  Serial.begin(115200);
  while(!Serial){
    Serial.begin(115200);
    delay(1000);
  }
  pinMode(LED_BUILTIN, OUTPUT);
  int rpm_int = 0;
  String rpm_str = "";
  while (rpm_int < 1){
    Serial.println("Please define RPM value (int)");
    while (Serial.available() == 0){
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500); 
    }
    rpm_str = Serial.readStringUntil('\n');
    rpm_int = rpm_str.toInt();
  }
  stepper.begin(rpm_int);
  Serial.println("Set target motor RPM to "+rpm_str);
  Serial.println("Write TEST to verify +360° and -360°");
  Serial.println("Write TURN followed by number of degrees (signed direction) to move the motor");
  Serial.println("Write CLOSE to end serial communication");
  Serial.println("\n!! End of startup - you can now turn on the voltage supply !!");
}

void loop() {
  while (Serial.available() == 0){
    // You can blink an LED or print dots if you want
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }
  String message = Serial.readStringUntil('\n'); // read until new line
  message.toUpperCase();
  if(message.startsWith("TURN ")){
    message.remove(0,5);
    int deg_rot = message.toInt();
    String output = "Turning motor by " + message;
    output += " degrees";
    Serial.println(output);
    stepper.rotate(deg_rot);
  }
  else if(message == "TEST"){
    Serial.println("Turning +360 and -360 degrees in a sequence");
    delay(1000);    
    // Tell motor to rotate 360 degrees
    stepper.rotate(360);
    delay(1000);
    //Serial.println("Turning -360");
    stepper.rotate(-360);
  }
  else if(message == "CLOSE"){
    Serial.println("!! End of serial communication !!");
    Serial.end();
    return;
  }
  else Serial.println("!Invalid command!");
  delay(1000);
}
