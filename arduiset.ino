#include <Servo.h>
#include <EEPROM.h>
 
Servo myservo;
int servoPin = 9;
int resetPin = 7;
int bootCheckPointer = 1;
bool secondBoot = false;
           
void setup()
{
  digitalWrite(resetPin, HIGH); //We need to set it HIGH immediately on boot
  pinMode(resetPin,OUTPUT);     //We can declare it an output ONLY AFTER it's HIGH
  if (!checkSecondBoot()) {
  myservo.attach(servoPin);
  }
  Serial.begin(115200);

}
 
 
void loop()
{
  //Serial.println("resetPin:" && resetPin && ":servoPin:" && servoPin + ".");
  //  Serial.println(resetPin);
  //  Serial.println(servoPin);
//  Serial.println(EEPROM.read(bootCheckPointer));
  Serial.println("loop start");
  if (!secondBoot) {
//    Serial.println("If !secondBoot");
//    Serial.println(checkSecondBoot());
    if (!checkSecondBoot()) { // If this is the first boot
    Serial.println("FirstBoot");
  //    Serial.println(checkSecondBoot());
      setSecondBoot(); // Set EEPROM pointer
      openClose();
      resetArduino();
    }
    else
    {
      resetEEPROM();
      secondBoot = true;
      Serial.println("secondBoot = True");
    //  Serial.println(checkSecondBoot());
    }
  }
    
}

boolean checkSecondBoot() {
	if (EEPROM.read(bootCheckPointer) != 255)
	{
		return true; // This is the second boot.
Serial.println("checkSecondBoot = true");
	}
	else
	{
		return false; // This is the first boot.
Serial.println("checkSecondBoot = false");
	}
}

void openClose() {
  myservo.write(0);
  delay(1000);
  myservo.write(88); 
  Serial.println("  myservo.write(180);");
  delay(60000);
  myservo.write(180);
  delay(1000);
  myservo.write(88);
  Serial.println("  myservo.write(0);");
  delay(1500);
  myservo.write(88);
    Serial.println("  myservo.write(87);");
  delay(1000);
    Serial.println("openClose END");
}

void resetEEPROM() {
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
