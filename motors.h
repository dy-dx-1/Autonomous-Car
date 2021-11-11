void front_def() {                           // defining the configs for front and reverse movement, these will be run inside of the actual front and reverse fonctions 
  digitalWrite(m1dir1, HIGH);                // only there to lighten the code when running the other functions 
  digitalWrite(m1dir2, LOW);
  digitalWrite(m2dir1, HIGH);
  digitalWrite(m2dir2, LOW);
}

void back_def() {
  digitalWrite(m1dir1, LOW);
  digitalWrite(m1dir2, HIGH);
  digitalWrite(m2dir1, LOW);
  digitalWrite(m2dir2, HIGH);
}

void on_off_switch() {
  switch (Is_On) {
    case 1:          // If it's on, means we pressed the button to turn it off
      cspeed = 0;
      Is_On = 0;
      break;
    case 0:         // If it's off, we wanted to turn it on :
      cspeed = 210;         //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Our turning on speed is 210 so that it starts moving slowly (MIN STAR5T SPEED)
      Is_On = 1;
      front_def(); 
      break;
  }
  analogWrite(speed1, cspeed);
  analogWrite(speed2, cspeed);
}


void augment_speed() {
  if (cspeed == 255) {
    //TODO: add flashing light or something to indicate max speed
    Serial.println("Max speed atteinte");
  }
  else {
    cspeed = cspeed + 20;
    analogWrite(speed1, cspeed);
    analogWrite(speed2, cspeed);
  }
}

void lower_speed() {    // AJUSTER JUMP POUR QUE CE SOIT ACTIF JUSTE EN DÉMARRANT
  if (cspeed == 95) {
    //TODO: add flashing light or something to indicate min speed
    Serial.println("Min speed atteinte");
  }
  else if ((cspeed - 20) <= 115) {                                /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////MIN 'JUMP'  VALUE NEED TO CHECK AND OR CHANGE
    cspeed = cspeed - 20;
    analogWrite(speed1, 255);
    analogWrite(speed2, 255);
    delay(500);                                                              ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  TO CHECK
    analogWrite(speed1, cspeed);
    analogWrite(speed2, cspeed);
  }
  else {
    cspeed = cspeed - 20;
    analogWrite(speed1, cspeed);
    analogWrite(speed2, cspeed);
  }
}

void direction_front() {
  if (Is_On == 0) {
    // do nothing special
  }
  else if (moving_front == 1) {                                                     ////CHECK THAT WE ARE USING THIS VARIABLE AT OTHER PLACES!!
    // if it's on, we need to stop it first and then change
    Serial.println("Already going front");
  }
  else if (moving_front == 0) {
    analogWrite(speed1, 0);
    analogWrite(speed2, 0);
    front_def();
    moving_front = 1;
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////CHECK THAT THIS WORKS AND DO IT FOR REVERSE
}

void direction_back() {
  back_def(); 
}
