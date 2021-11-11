
void send_Ping() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH); //Priming the sensor
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);  // Sending the signal -- sensor also inherently sets echo high until signal gets back
  travelTime = pulseIn(echoPin, HIGH);  //pulse in measures how long echo stayed HIGH
}

int get_distance() {
  total_dist = 0;         // resetting total distance each time we measure it (so that the average is not influenced)
  for (int n = 0; n <= 44; n++) {                  // Take the average of 44 measurements
    send_Ping();
    if (travelTime > 0) {
      distance_cm = ((travelTime - 74.691805951896) / 5.43163473298) / 10;
      total_dist =  distance_cm + total_dist;
    }
    else {                 // If we get a bad reading (negative ping time)
      total_dist = 0; // this makes it so average dist goes to 0 --> we will check for that during decision process
      break;
    }
    delay(25); //will be used multiple times so delay serves to separate readings -¨> more stability
  }
  average_dist = total_dist / 45;   // distance in cm
  return average_dist;
}

void perform_measurements() {   // int so that we don't get void value error, maybe just copy paste choose_path inside of here to return it one shot
  for (int pos = 0; pos <= 4; pos++) {
    delay(20);
    servo.write(angles[pos]);
    delay(1700); //leaving time for the servo to move
    scan[pos] = get_distance(); // adding measured distance to the array
    Serial.print("Distance at ");
    Serial.print(angles[pos]);
    Serial.print(" :");
    Serial.println(average_dist);
    // At the end of this for loop have the scan[] array that holds the distances in positions {130, 100, 60, 30, 0} in their respective indexes.
  }
}

int detect_obstacles() {          // Checks the angles and returns a chosen path 
  perform_measurements(); 
  for (int index = 0; index <= 4; index++) {        // First we check values from the center and then going to the
    obj_location = scan[scanning_indexes[index]];    // scanning indexes returns the proper index to check in scan, in the appropiate order || reminder: scanning indexes is {2, 3, 1, 4, 0}
    if (obj_location == 0) {
      // We ignore the wrong readings that returned 0 distance || chance of all being 0 is basically none so idk if I should add handling for that case
    }
    else if (obj_location > 100) {  // If object is further than 1meter, we consider it safe to continue since we are advancing by 1m each time
      // TODO: first try to go in front, we need to define the maximum farthest obstacle distance so that we continue going forward (first need to define our meters covered before cehcking for obstacles)
      // If going forward impossible, we check the nearest sides in a certian order and if that is not possible, continue until finding good path.
      return scanning_indexes[index]; // return the index of the chosen path in {130, 100, 60, 30, 0}
    }
    else {
      if (index == 4) {  // if we are at the last iteration of for loop and we are here then
        return 5; // returning an index that doesn't exist, we'll check for this in the main code so that we go back or something
      }
      else { // do nothing if we are still checking
      }
    }
  }
}
