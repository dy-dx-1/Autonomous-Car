void jumpstart() {
  analogWrite(speed1, 255);
  analogWrite(speed2, 255);
  delay(300);
}

void set_displacement() {    // Function that keeps the car moving for a chosen amount of seconds, while making the info led blink 
  const int blink_interval = 500;                 //blink every half a second
  
  tstart = millis();
  unsigned long t1 = 0;                             // These 3 vars are exceptionally declared here because I don't really see a plus putting them in setup
  unsigned long current_time;
  int info_led_state = LOW;                                     // Making sure that the LED is off before we start
  while ((millis() - tstart) <= (mtime * 1000)) {
    current_time = millis();
    if ((current_time - t1) >= blink_interval) {   // If the interval between the current time and the start time (first iteration is 0) is bigger or equal than the interval we set ---> change the led status
      t1 = current_time;                                     // setting t1 to (what will become old) current time so that in the next iteration we can recalculate the interval with a new current time
      if (info_led_state == HIGH) {
        info_led_state = LOW;
      }
      else {
        info_led_state = HIGH;
      }
      digitalWrite(info_led, info_led_state);
    }
  }
  digitalWrite(info_led, info_led_state);       // Turning off the info LED when done
  analogWrite(speed1, 0);
  analogWrite(speed2, 0);
  moving = 0;
}

void moving_commands() {
  moving = 1;
  mtime = command.toInt();                        // This conversion Str->int is necessary to use in the time counting and switch | it works since the function holding it is only called when we know that commands must hold ["1", "11"], 11 representing 'no stop' moving mode 
  if (cspeed <= 135) {                          // if low speed, use the jumpstart to be able to move
    jumpstart();
  }
  analogWrite(speed1, cspeed);
  analogWrite(speed2, cspeed);          // Starting the movement 
  if (mtime != 11) { 
    set_displacement();                           // If we didn't select constant (no stop) movement mode, use the timer to stop the movement after selectedd amount of time, moving is finally set to 0 
  }
  else { 
    digitalWrite(info_led, HIGH);           // Turn on the info led while constant movement, will turn off when we turn off or pause the car
      // no more commands since we don't want to turn off previous analog writes, moving doesn't get changed since set_displacement() is not called 
  }

}
