// Cicero44bc 15:24 08 Nov 2014 - www.breakfastlabs.com
//
// This sketch allows a safe reset of the Arduino using code and one wire from a digital pin to the reset pin.
// I found when using a continuous servo I was getting very slight drift. I discovered that if the servo is not
// attached in code, therefore not receiving any instructions it would not drift or flutter at all.
//
// This sketch checks if this is the first boot, runs and action if so (openClose() in this case - to accommodate
// a requirement for my aquarium), sets a value in EEPROM (so it is persistent after reset) resets the Arduino,
// checks EEPROM if its first boot, does nothing (in this case) when this is confirmed as the 'second boot' and
// resets the value in EEPROM for next time.
//
// Notes
//
// When programing your Arduino be sure to disconnect the wire to RST if you want your Arduino to auto-reset
// after the upload completes.
//
// Outputs to serial at 115200 for debugging.
//
// Credits
//
// Reset method - http://weblog.jos.ph/development/arduino-reset-hack/
//
// Shout out - Thanks to ALL at ultimateReef.net for your help.

#include <Servo.h>
#include <EEPROM.h>
 
Servo servo;
int servoPin = 9;
int resetPin = 7; // Digital Pin 7, connected to RST
int bootCheckPointer = 1; // Checks position one.
bool secondBoot = false;
           
void setup()
{
  digitalWrite(resetPin, HIGH); // Pin must be set to HIGH immediately on boot.
  pinMode(resetPin,OUTPUT);     // Declare it as an output AFTER it's HIGH
  if (checkSecondBoot()) {  // Check EEPROM to confirm if this is this is the first boot - Checking a Variable first is better due to save the EEPROM (Limited R/W's).
    secondBoot = true;
    resetEEPROM();
  }
  if (!secondBoot) {     // Only attach the servo if it is the first boot. On a second boot servo will not be attached.
    servo.attach(servoPin);
  }
  Serial.begin(115200);

}
 
 
void loop()
{
  Serial.println("loop start");
  if (!secondBoot) { 
    Serial.println("FirstBoot. Now set EEPROM value, open and Close then reset...");
      setSecondBoot(); // Set EEPROM pointer
      openClose();
      resetArduino();
  }
  else
  {
    Serial.println("Second boot");
  } 
}

boolean checkSecondBoot() { // http://arduino.cc/en/Reference/EEPROMRead
	if (EEPROM.read(bootCheckPointer) != 255)
	{
		return true; // This is the second boot.
	}
	else
	{
		return false; // This is the first boot.
	}
}

void openClose() {
  servo.write(0);
  Serial.println("Open");
  delay(1000);
  servo.write(86);
  Serial.println("Stay Open for 60secs");
  delay(60000);
  servo.write(180);
  Serial.println("Close");
  delay(1000);
  servo.write(86);
  Serial.println("Stay Closed");
}

void resetEEPROM() { // http://arduino.cc/en/Reference/EEPROMWrite
  EEPROM.write(bootCheckPointer, 255); // (ADDRESS, VALUE)
  Serial.println("EEPROM values reset.");
}

void setSecondBoot() {
  EEPROM.write(bootCheckPointer, 123); // (ADDRESS, VALUE)
  Serial.println("SecondBoot set");
}

void resetArduino() {
  Serial.println("ResetArduino");
  digitalWrite(resetPin, LOW);
}
