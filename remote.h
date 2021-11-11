//#ifndef __included_setup_h
//#define __included_setup_h
//#include "Arduino.h"

void get_command() {
  while (remote.decode(&cmd) == 0) {
    // waiting for command
  }
  delay(5);
  if (cmd.value == 0xFFFFFFFF) {
    command = "REPEAT";    // Repeat means keeping the button pressed or pressing it for too long
  }
  else if (cmd.value == 0xFFA25D || cmd.value == 0x24AE7D4C || cmd.value == 0xE318261B) {
    command = "POWER";     // On Off button
  }
  else if (cmd.value == 0xFF629D || cmd.value == 0x511DBB) {
    command = "SPEED+";      // Volume up button
  }
  else if (cmd.value == 0xFF22DD || cmd.value == 0x52A3D41F || cmd.value == 0xA552BC26 || cmd.value == 0x9D7716BF || cmd.value == 0xEBCE01C5 || cmd.value == 0x340FA58E || cmd.value == 0x9E31C879 || cmd.value == 0x49CA3C98 || cmd.value == 0x5E13583A) {
    command = "LEFT";          // Skip left button (backwards skip)
  }
  else if (cmd.value == 0x20FE4DBB || cmd.value == 0xDAE5E079 || cmd.value == 0x1BA7C4D4 || cmd.value == 0x4BDA058D || cmd.value == 0xFFC23D || cmd.value == 0xB88A1A4F) {
    command = "RIGHT";       // Skip forward button
  }
  else if (cmd.value == 0xF076C13B || cmd.value == 0xFFE01F ) {
    command = "REVERSE";   //Arrow pointing down button
  }
  else if (cmd.value == 0xA3C8EDDB || cmd.value == 0xFFA857) {
    command = "SPEED-";     // Volume - button
  }
  else if (cmd.value == 0xE5CFBD7F || cmd.value == 0xFF906F) {
    command = "FORWARD";     // Arrow pointing up button
  }
  else {
    Serial.print("Error, code : ");
    Serial.println(cmd.value, HEX);
    digitalWrite(redPin, HIGH);
    delay(400);
    digitalWrite(redPin, LOW);
    command = "NONE";
  }
  remote.resume();
}
//#endif
