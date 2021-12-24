/// Getting commands from the IR remote and updating command variable accordingly. ///

void get_command()
{
  while (remote.decode(&cmd) == 0)
  {
    // Nothing happens while we are waiting for a command
  }
  delay(5);
  if (cmd.value == 0xFFFFFFFF)
  {
    command = "REPEAT"; // Repeat is sent by the remote when keeping the button pressed or pressing it for too long.
  }
  else if (cmd.value == 0xFFA25D || cmd.value == 0x24AE7D4C || cmd.value == 0xE318261B)
  {
    command = "POWER"; // On/Off button
  }
  else if (cmd.value == 0xFF629D || cmd.value == 0x511DBB)
  {
    command = "SPEED+"; // Volume up button
  }
  else if (cmd.value == 0xFF22DD || cmd.value == 0x52A3D41F || cmd.value == 0xA552BC26 || cmd.value == 0x9D7716BF || cmd.value == 0xEBCE01C5 || cmd.value == 0x340FA58E || cmd.value == 0x9E31C879 || cmd.value == 0x49CA3C98 || cmd.value == 0x5E13583A)
  {
    command = "LEFT"; // Skip left button (backwards skip)
  }
  else if (cmd.value == 0x20FE4DBB || cmd.value == 0xDAE5E079 || cmd.value == 0x1BA7C4D4 || cmd.value == 0x4BDA058D || cmd.value == 0xFFC23D || cmd.value == 0xB88A1A4F)
  {
    command = "RIGHT"; // Skip forward button
  }
  else if (cmd.value == 0xF076C13B || cmd.value == 0xFFE01F)
  {
    command = "REVERSE"; // Arrow pointing down button
  }
  else if (cmd.value == 0xA3C8EDDB || cmd.value == 0xFFA857)
  {
    command = "SPEED-"; // Volume - button
  }
  else if (cmd.value == 0xE5CFBD7F || cmd.value == 0xFF906F)
  {
    command = "FORWARD"; // Arrow pointing up button
  }
  else if (cmd.value == 0xFFE21D || cmd.value == 0xEE886D7F || cmd.value == 0x754D75F6 || cmd.value == 0x8BBB7589)
  {
    command = "PAUSE"; // FUNC/STOP
  }
  else if (cmd.value == 0xFFB04F || cmd.value == 0xF0C41643 || cmd.value == 0xB482A44E)
  {
    command = "AUTO"; // ST/REPT
  }
  else if (cmd.value == 0x97483BFB || cmd.value == 0xFF9867)
  {
    command = "11"; // EQ button
  }
  else if (cmd.value == 0xFF6897 || cmd.value == 0xC101E57B)
  {
    command = "10"; // 0 button
  }
  else if (cmd.value == 0xFF30CF)
  { // From here on down all the buttons correspond to their command value.
    command = "1";
  }
  else if (cmd.value == 0xFF18E7)
  {
    command = "2";
  }
  else if (cmd.value == 0xFF7A85)
  {
    command = "3";
  }
  else if (cmd.value == 0x8C22657B || cmd.value == 0xFF10EF)
  {
    command = "4";
  }
  else if (cmd.value == 0xFF38C7 || cmd.value == 0x488F3CBB)
  {
    command = "5";
  }
  else if (cmd.value == 0x449E79F || cmd.value == 0x9D961E14 || cmd.value == 0xFF5AA5)
  {
    command = "6";
  }
  else if (cmd.value == 0x723E4430 || cmd.value == 0x68DB5DA4 || cmd.value == 0x5EB25D54 || cmd.value == 0xA585353A || cmd.value == 0x172A9B79 || cmd.value == 0x1CD0A4F || cmd.value == 0x44824F97 || cmd.value == 0xD3F443CB || cmd.value == 0xFF42BD)
  {
    command = "7";
  }
  else if (cmd.value == 0xFF1C3656 || cmd.value == 0x9F94FFA6 || cmd.value == 0xFF4AB5 || cmd.value == 0x465573243 || cmd.value == 0x1045675 || cmd.value == 0x255 || cmd.value == 0x465573243)
  {
    command = "8";
  }
  else if (cmd.value == 0xFF52AD || cmd.value == 0x3EC3FC1B)
  {
    command = "9";
  }
  else
  { // If we get an unrecognizable command, we alert by blinking the red LED.
    Serial.print("Error, code : ");
    Serial.println(cmd.value, HEX);
    digitalWrite(error_led, HIGH);
    delay(400);
    digitalWrite(error_led, LOW);
    command = "NONE"; // This is handled in "main.ino".
  }
  remote.resume();
}
