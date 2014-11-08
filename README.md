arduiset
========

cicero44bc 23:10 08 Nov 2014 - www.breakfastlabs.com

This sketch safely resets an Arduino using code and only one wire. It does not use watchdog.

Purpose: I found when using a continuous servo I was getting very slight drift, even after finding 'Centre’. I confirmed if the servo is not attached in code, it would not drift or flutter at all. Detach was not working, this became a quick fix and a chance to try something new for me with using built in persistent storage to allow the Arduino to determine if it is booting after a reset, i.e. not cold booting.

Method: During setup() the sketch checks if this is the first boot by checking the value of a byte in persistent storage, i.e. EEPROM (reference in code comments), if so it runs an action (openClose() in this case - to accommodate a requirement for my aquarium). A value is then set in EEPROM (so it is persistent after reset and a 'reboot' can be therefore be determined), then the Arduino is reset. On second boot the EEPROM is then checked, in this case the Arduino then does nothing, and the 'warm boot flag' byte is reset in EEPROM.

The EEPROM is purposely only checked once and a corresponding boolean in RAM checked thereafter because EEPROMhas limited R/W's.

Notes

When programing your Arduino be sure to disconnect the wire to RST if you want your Arduino to auto-reset after the upload completes.

Outputs to serial at 115200 for debugging.

Credits

Reset method - http://weblog.jos.ph/development/arduino-reset-hack/

Thanks to ALL at ultimateReef.net for your help on the aquarium front, esp. promazin, McQueen25,stevesplace78 and foo who have helped me in the past.
