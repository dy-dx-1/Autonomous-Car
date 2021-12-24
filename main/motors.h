/// All general car commands, excluding moving commands. ///
// speed1 is the motor on the left and speed2 is the motor on the right. (With respect to vector representing distance from battery --> servo).
void info_blink()
{
  digitalWrite(info_led, HIGH);
  delay(300);
  digitalWrite(info_led, LOW);
  delay(300);
  digitalWrite(info_led, HIGH);
  delay(300);
  digitalWrite(info_led, LOW); // Blinking the blue LED to indicate that a command has been passed properly and/or we are awaiting more input.
}

void warning_blink()
{
  digitalWrite(warning_led, HIGH);
  delay(200);
  digitalWrite(warning_led, LOW);
  delay(200);
  digitalWrite(warning_led, HIGH);
  delay(200);
  digitalWrite(warning_led, LOW); // Blinking the yellow LED to indicate a minor problem like trying to run a command that's unavailable in the current mode.
}

void move_motors()
{ // Simplifies the tuning of motors speeds (can change differences between the 2 in one place) and saves a bit of space.
  analogWrite(speed1, cspeed);
  analogWrite(speed2, cspeed - 40); // Else, car has a tendancy to turn left.
}

void go_forward()
{ // go_forward() and go_reverse() set the pins to the proper config for the task and update moving_front.
  digitalWrite(m1dir1, HIGH);
  digitalWrite(m1dir2, LOW);
  digitalWrite(m2dir1, HIGH);
  digitalWrite(m2dir2, LOW);
  moving_front = 1;
}

void go_reverse()
{
  digitalWrite(m1dir1, LOW);
  digitalWrite(m1dir2, HIGH);
  digitalWrite(m2dir1, LOW);
  digitalWrite(m2dir2, HIGH);
  moving_front = 0;
}

void get_turning_command()
{ // Not using "command = get_command();" so that I don't have to import the remote module multiple times.
  while (remote.decode(&cmd) == 0)
  {
  // waiting for command
  ignore_repeat:   // We will go here if a repeat command is called.
                 ; // "pass"
  }
  delay(5);
  if (cmd.value == 0xFFFFFFFF)
  {                  // If we get a repeat, we want to ignore it, I can see a way to not use a goto but it shouldn't be much of an issue for this.
    delay(50);       // Making sure the signal has ended before jumping back into the loop.
    remote.resume(); // Starting to listen to commands again.
    goto ignore_repeat;
  }
  else if (cmd.value == 0xFF30CF)
  { // mtime is equivalent to the numerical value of the button (buttons '1' to '9')
    mtime = 1;
  }
  else if (cmd.value == 0xFF18E7)
  {
    mtime = 2;
  }
  else if (cmd.value == 0xFF7A85)
  {
    mtime = 3;
  }
  else if (cmd.value == 0x8C22657B || cmd.value == 0xFF10EF)
  {
    mtime = 4;
  }
  else if (cmd.value == 0xFF38C7 || cmd.value == 0x488F3CBB)
  {
    mtime = 5;
  }
  else if (cmd.value == 0x449E79F || cmd.value == 0x9D961E14 || cmd.value == 0xFF5AA5)
  {
    mtime = 6;
  }
  else if (cmd.value == 0x723E4430 || cmd.value == 0x68DB5DA4 || cmd.value == 0x5EB25D54 || cmd.value == 0xA585353A || cmd.value == 0x172A9B79 || cmd.value == 0x1CD0A4F || cmd.value == 0x44824F97 || cmd.value == 0xD3F443CB || cmd.value == 0xFF42BD)
  {
    mtime = 7;
  }
  else if (cmd.value == 0xFF1C3656 || cmd.value == 0x9F94FFA6 || cmd.value == 0xFF4AB5 || cmd.value == 0x465573243 || cmd.value == 0x1045675 || cmd.value == 0x255 || cmd.value == 0x465573243)
  {
    mtime = 8;
  }
  else if (cmd.value == 0xFF52AD || cmd.value == 0x3EC3FC1B)
  {
    mtime = 9;
  }
  else
  { // If another button is pressed than '1' to '9', mtime holds 10 to indicate this. We check for this in turn().
    mtime = 10;
  }
  remote.resume();
}

void turn()
{ // Does the actual turning for X amount of seconds after mtime is updated
  if (mtime != 10.0)
  { // Making sure that a button from '1' to '9' was pressed (see get_turning_command()).
    analogWrite(speed1, 255);
    analogWrite(speed2, 255); // Turning speed is fixed at max since at lower speed we may be risking stalling.
    delay(mtime * 100);       // Turning for mtime*100 in milliseconds because a turn in seconds was too long to be practical.
    analogWrite(speed1, 0);
    analogWrite(speed2, 0);
  }
  else
  {
    warning_blink(); // If none of these (1 to 9) buttons are pressed, send a warning blink and we'll just exit out of the turn function without doing anything.
  }
  // Now we need to reset the direction pins in the status that they had before the turn (we changed them in arm_right() or arm_left()).
  switch (moving_front)
  {
  case 1:
    go_forward();
    break;
  case 0:
    go_reverse();
    break;
  }
}

void arm_right()
{ // Preparing motors to turn right.
  digitalWrite(m1dir1, HIGH);
  digitalWrite(m1dir2, LOW);
  digitalWrite(m2dir1, LOW);
  digitalWrite(m2dir2, HIGH);
}

void arm_left()
{ // Preparing motors to turn left.
  digitalWrite(m1dir1, LOW);
  digitalWrite(m1dir2, HIGH);
  digitalWrite(m2dir1, HIGH);
  digitalWrite(m2dir2, LOW);
}

void turn_right()
{
  arm_right();
  digitalWrite(info_led, HIGH); // Setting it high to signal that we are awaiting turn time
  // Now we need to know for how long to turn so we will await a command (1 to 9) and turn for the corresponding amount of seconds, get_turning_command() get's that info.
  get_turning_command();
  digitalWrite(info_led, LOW); // Going low to show that we got a command.
  turn();                      // Doing the turn with the time that get_turning_command() got.
}

void turn_left()
{
  arm_left();
  digitalWrite(info_led, HIGH);
  get_turning_command();
  digitalWrite(info_led, LOW);
  turn();
}

void pause()
{
  cspeed = 0;
  moving = 0;                  // Setting moving related variables to 0.
  digitalWrite(info_led, LOW); // Making sure that info LED is turned off (could be on during constant moving for example).
  analogWrite(speed1, cspeed);
  analogWrite(speed2, cspeed);
}

void on_off_switch()
{
  switch (Is_On)
  {
  case 1:                      // If the car was on when we pressed the button, we wanted to turn it off.
    pause();                   // Car being off is basically just paused but with all other commands blocked except POWER.
    Is_On = 0;                 // Updating on status.
    digitalWrite(on_led, LOW); // Turning off the ON status led
    break;
  case 0:         // If it's off, we wanted to turn it on :
    cspeed = 215; // Starting the car at a medium speed.
    Is_On = 1;
    go_forward();               // Making sure that we turn on ready to go forward.
    digitalWrite(on_led, HIGH); // Turning on the ON status led.
    break;
  }
}

void augment_speed()
{ // Exceptionally for these functions, we have a check for moving here and not in the switch in main because these (functions) are used in both cases for moving 1 and 0 so we have to distinguish here.
  if (cspeed == 0 && Is_On == 1)
  { // If we are paused (speed is 0 but turned on).
    cspeed = 195;
    // So that the following 'else' turns cspeed into 215 (which is the starting speed.)
    // We do this because without it, when we are paused augment speed would allow you to have cspeeds that are
    // not multiples of 5 (limit checks would not work) and 2) or too low (speeds lower than 95).
  }
  if (cspeed == 255)
  { // Since we start at 215 and only add/remove by deltas of 20, we can get an exact max of 255.
    Serial.println("Max speed atteinte");
    warning_blink();
  }
  else
  {
    cspeed = cspeed + 20;
  }
  if (moving == 1)
  {                // This will only be passed if we are moving while this function is called, so only if we are in constant movement mode!
    move_motors(); // Else we would start moving when updating speed while idle.
  }
}

void lower_speed()
{
  if (cspeed <= 95)
  { // Here using <= in case we are paused (speed 0).
    Serial.println("Min speed atteinte");
    warning_blink();
  }
  else
  {
    cspeed = cspeed - 20;
  }
  if (moving == 1)
  {
    move_motors();
  }
}