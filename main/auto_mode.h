/// Tells slave arduino to detect obstacles and get's the proper path to follow from it. ///

void detect_obstacles()
{
  digitalWrite(commpin, HIGH); // Indicating slave arduino to start obstacle detection process.
  delay(300);
  digitalWrite(commpin, LOW); // Turning it off since slave arduino only needs to see its high for a moment.

  // Now the slave arduino should be moving the servo and doing the measurements, this will take at least a couple of seconds.
  // We will start listening for a response from the slave arduino.

  listening = digitalRead(listen_pin); // Pin should be low since the other arduino hasn't finished the scan yet.
  while (listening == LOW)
  {                                      // Will turn high when the other arduino is done.
    listening = digitalRead(listen_pin); // We are just waiting the other arduino to finish.
  }
  tstart = millis(); // Record starting time the moment we get a signal from the other arduino.
  while (listening != LOW)
  {
    listening = digitalRead(listen_pin); // Waiting for the signal to stop.
  }
  delta = millis() - tstart; // As soon as the signal stops, calculate how much time the pin stayed high.
  if (delta >= 0 && delta < 200)
  { // Associating the time that the pin stayed high to the path that the slave arduino found. See slave arduino code.
    chosen_path = 0;
  }
  else if (delta >= 200 && delta < 400)
  {
    chosen_path = 1;
  }
  else if (delta >= 400 && delta < 600)
  {
    chosen_path = 2;
  }
  else if (delta >= 600 && delta < 800)
  {
    chosen_path = 3;
  }
  else if (delta >= 800 && delta < 1000)
  {
    chosen_path = 4;
  }
  else if (delta >= 1000 && delta <= 1200)
  {
    chosen_path = 5; // This means that we did not find a viable path. See slave arduino code.
  }
  else
  { // If we get here, the timing sent was not recognize so there is a communication problem. We will check for a chosen_path=10 in this case in main.
    chosen_path = 10;
    Serial.println("Communication error, couldn't match interval");
  }
  Serial.print("Communication ended, chosen_path is: ");
  Serial.println(chosen_path);
}