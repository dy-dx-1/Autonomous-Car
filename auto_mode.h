// will move this to main if needed

void autopilot() {
  // advancing 
  cspeed = 215;
  analogWrite(speed1, cspeed);
  analogWrite(speed2, cspeed);
  delay(2000); /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////using 2 sec for now but will update so that its 1m
  analogWrite(speed1, 0);
  analogWrite(speed2, 0);       // Stopping
 // stop, now time to take measures 
 
  digitalWrite(commpin, HIGH);   // Telling slave arduino to move the servo and find position
  delay(300);
  digitalWrite(commpin, LOW); // Turning it off since slave only needs to see its high for a moment
  // now the slave arduino should be moving the servo and doing the measurements, this will take at least a couple seconds
  // we will start listening for when the response from slave arduino will be coming back
  listening = digitalRead(listen_pin);   // pin should be low since the slave arduino hasn't started sending back teh response yet 
  while (listening == LOW) {
    listening = digitalRead(listen_pin); // we are waiting for servo to do its thing 
    Serial.print("We are still waiting for servo, this loops until signal is not low: "); 
    Serial.println(listening); 
  }
  tstart = millis();              // Record starting time when we get a signal from the other arduino 
  while (listening != LOW) {
    listening = digitalRead(listen_pin); // we are waiting for the signal to stop
  }
  delta = millis() - tstart;                                // calculate the time that passed while the pin was high
  Serial.println("Signal stopped, interval calculated"); 
  if (delta >= 0 && delta < 200) {
    chosen_path = 0;                  //////////////////////////////////////////right now using 1 here so that the blink is visible but we will assign to zero directly in main code
  }
  else if (delta >= 200 && delta < 400) {
    chosen_path = 1;  
  }
  else if (delta >= 400 && delta < 600) {
    chosen_path = 2;
  }
  else if (delta >= 600 && delta < 800) {
    chosen_path = 3;
  }
  else if (delta >= 800 && delta < 1000) {        // on le laisse comme ca (strict < limite) car a la limite, il n'y a pas bcp de chance que il y ait un obstacle dans la position adjacente en plus qu'on soit au max de l'ecart avec 200ms de delta, + pour le dernier, si on depasse on aura 5 so refaire measuremetn
    chosen_path = 4;
  }
  else if (delta >= 1000 && delta <= 1200) {
    chosen_path = 5;                    // reminder, this means that we have to retake measurement or go back since the arduino did not find a viable path
  }
  else {
    chosen_path = 10;                                   // this would only get returned if we somehow get a bad return timing from slave arduino we will check for this
    Serial.println("Communication error, couldn't match interval");
  }
  Serial.println("Communication ended"); 
  switch (chosen_path) { 
    case 0:
      Serial.println("Going path 0, 130deg"); 
      break; 
    case 1:
      Serial.println("Going path 1, 100deg"); 
      break; 
    case 2: 
      Serial.println("Going path 2, 60deg"); 
      break; 
    case 3: 
      Serial.println("Going path 3, 30deg"); 
      break; 
    case 4: 
      Serial.println("Going path 4, 0deg"); 
      break; 
    case 5: 
      Serial.println("None of the paths seem clear, going back and trying again"); 
      break; 
    case 10: 
      Serial.println("We got a communication error so trying again"); 
      break; 
  }

  Serial.println("Sequence done "); 
}

// for some reason when no paths are available (path 5, not a comm error) the function stops and freezes completerly instead of relooping trhough main 
