void info_blink() {
  digitalWrite(info_led, HIGH); 
  delay(300); 
  digitalWrite(info_led, LOW); 
  delay(300); 
  digitalWrite(info_led, HIGH); 
  delay(300); 
  digitalWrite(info_led, LOW);      // Blinking the led 
}

void warning_blink() {                      //DIFFERNT FROM BAD IR READING BECAUSE DOUBLE BLINK INSTEAD OF ONE 
  digitalWrite(warning_led, HIGH); 
  delay(300); 
  digitalWrite(warning_led, LOW); 
  delay(300); 
  digitalWrite(warning_led, HIGH); 
  delay(300); 
  digitalWrite(warning_led, LOW);      // Blinking the led 
}

void go_forward() {
  digitalWrite(m1dir1, HIGH);
  digitalWrite(m1dir2, LOW);
  digitalWrite(m2dir1, HIGH);
  digitalWrite(m2dir2, LOW);
  moving_front = 1;
  info_blink(); 
}

void go_reverse() {
  digitalWrite(m1dir1, LOW);
  digitalWrite(m1dir2, HIGH);
  digitalWrite(m2dir1, LOW);
  digitalWrite(m2dir2, HIGH);
  moving_front = 0;
  info_blink(); 
}

void turn_right() {                       /// Will probably have to tune turn right and left depending on connections ////////////////////////////////////////NEED TO ADD ACTUAL ACTIVATION OF MOTORS 
  info_blink(); 
  digitalWrite(m1dir1, HIGH);
  digitalWrite(m1dir2, LOW);
  digitalWrite(m2dir1, LOW);
  digitalWrite(m2dir2, HIGH);
}

void turn_left() {
  info_blink(); 
  digitalWrite(m1dir1, LOW);
  digitalWrite(m1dir2, HIGH);
  digitalWrite(m2dir1, HIGH);
  digitalWrite(m2dir2, LOW);
}

void pause() {
  cspeed = 0; 
  moving = 0; 
  digitalWrite(info_led, LOW); 
  analogWrite(speed1, cspeed); 
  analogWrite(speed2, cspeed); 
}

void on_off_switch() {
  switch (Is_On) {
    case 1:          // If it's on, means we pressed the button to turn it off
      pause();              // Car being off is basically just paused but with all other commands blocked except POWER 
      Is_On = 0;      
      digitalWrite(on_led, LOW);                    // Turning off the ON status led 
      break;
    case 0:         // If it's off, we wanted to turn it on :
      cspeed = 210;         //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Our turning on speed is 210 so that it starts moving slowly (MIN STAR5T SPEED)
      Is_On = 1;
      moving = 1;
      go_forward();       
      digitalWrite(on_led, HIGH);                               //Turnign on the ON status led 
      break;
  }
}

void augment_speed() {  // Exceptionally for thse functions, we have a check for moving here and not in the switch in 'commands.h' because these are used in both cases for moving 1 and 0 so we have to distinguish here 
  if (cspeed == 255) {
    //TODO: add flashing light or something to indicate max speed
    Serial.println("Max speed atteinte");
    warning_blink();    
  }
  else {
    cspeed = cspeed + 20;
  }
  if (moving == 1) {                    // analogWrite will only be passed if we are moving while this function is called, so only if we are in constant movement mode  
    analogWrite(speed1, cspeed); 
    analogWrite(speed2, cspeed); 
  } 

}

void lower_speed() {    
  if (cspeed == 95) {
    //TODO: add flashing light or something to indicate min speed
    Serial.println("Min speed atteinte");
    warning_blink(); 
  }
  else {
    cspeed = cspeed - 20;
  }
  if (moving == 1) {
    analogWrite(speed1, cspeed); 
    analogWrite(speed2, cspeed); 
  }
}
