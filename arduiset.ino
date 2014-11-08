// cicero44bc 23:10 08 Nov 2014 - www.breakfastlabs.com
//
// This sketch safely resets an Arduino using code and only one wire. It does not use watchdog.
// 
// Purpose: I found when using a continuous servo I was getting very slight drift, even after finding 'centre'. 
// I confirmed if the servo is not attached in code, it would not drift or flutter at all. Detach was not working, 
// this became a quick fix and a chance to try something new for me with using built in persistent storage to 
// allow the Arduino to determine if it is booting after a reset, i.e. not cold booting.
//
// Method: During setup() the sketch checks if this is the first boot by checking the value of a byte in persistant 
// storage, i.e. EEPROM (reference in code comments), if so it runs an action (openClose() in this case - to
// accommodate a requirement for my aquarium). A value is then set in EEPROM (so it is persistent after reset and
// a 'reboot' can be therefore be determined), then the Arduino is reset. On second boot the EEPROM is then checked
// in this case the Arduino then does nothing, and the 'warm boot flag' byte is reset in EEPROM.
//
// The EEPROM is purposely only checked once and a corresponding boolean in RAM checked thereafter because EEPROM
// has limited R/W's.
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
// Thanks to ALL at ultimateReef.net for your help on the aquarium front, esp. promazin, McQueen25,
// stevesplace78 and foo who have helped me in the past.

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
  if (checkSecondBoot()) {  // Check EEPROM to confirm if this is this is the first boot.
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
