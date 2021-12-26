/// Obstacle detection, this is ran by the slave arduino when it receives a signal from main arduino. Checks for obstacles and returns a viable path. ///
#include <NewPing.h>
#include <Servo.h>

#define TRIGGER_PIN 10
#define ECHO_PIN 11
#define MAX_DISTANCE 400 // Setting our max distance to 400cm, when a distance bigger is measured, NewPing returns 0.

int listening_pin = A0; // Pin where we listen to the signal from the main arduino.
int s_Pin = 6;          // Servo control pin.
int send_pin = 8;       // Pin that will be used to communicate the final path to main arduino¸

int angles[] = {130, 100, 60, 30, 0};     // Array of angles that the servo checks ~(complete left, 45 left, center, 45 right, complete right).
int scanning_indexes[] = {2, 3, 1, 4, 0}; // array holding the indexes in angles to check in priority order (first middle, then right, then left, then farthest right and farthest left.

bool listen_pin_status; // Indicates if the master arduino has sent a signal or not.
float ping_time;        // Ping time measured by the HC-SR04.
int obj_location;       // Distance measured by the HC-SR04 at a certain angle.
int chosen_path;        // Variable holding the path to follow after running the obstacle detection.

Servo servo;                                        // Creating servo object.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // Creating sonar object.

///// SETUP
void setup()
{
  Serial.begin(9600);
  servo.attach(s_Pin); // Attaching servo to pin
  pinMode(listening_pin, INPUT);
  pinMode(send_pin, OUTPUT);
  pinMode(13, OUTPUT); // Indicates when the obstacle detection is running. There is a white LED hooked up to it (in addition to built in LED).
}

int detect_obstacles()
{
  // We check the distance measured in each pos (in priority order) and break if OK.
  for (int pos = 0; pos <= 4; pos++)
  {
    delay(20);
    servo.write(angles[scanning_indexes[pos]]); // Holds the index for the position (in the order to check) and then passes it to servo.
    delay(1500);                                // Leaving time for servo to move .
    ping_time = sonar.ping_median(10) / 2;      // Taking the median of  10 pings, /2 to find the distance from sensor to obj (else we get distance of the whole trip of the sound).
    obj_location = ping_time * 0.0344;          // Converting ms to cm, assuming a 344m/s speed for sound (at 21C).
    Serial.print("Obj_location: ");
    Serial.println(obj_location);
    if (obj_location > 60 || obj_location == 0)
    {                               // If the object is farther away than 60cm, we will continue here, OR if the value is 0 (newping sets 0 for a distance thats < than MAX_DISTANCE).
      return scanning_indexes[pos]; // As soon as we get a viable path (free for >60cm), we stop the function and return the path.
    }
    else
    { // If we get a distance that is < than our limit, don't do anything or
      if (pos == 4)
      {           // return 5 if we are at the last iteration (this means that we got 0 in all of our runs),
        return 5; // 5 means that we coudn't find any viable path.
      }
    }
  }
}
/*
Explanation for the if/elses in detect_obstacles():
First, the for loop iterates through the positions to check in priority order (front, right, left, ...)
During each iteration, servo is moved, and then a distance is estimated, then this distance is immediately checked for viability:
if the distance is more than our limit, we STOP and return the position
if it isnt, there are 2 possible cases, either it'S a 0 (meaning there was an issue during the measuring) or it'S <limit. [It can't be negative because
if the distance was negative, the ping would have been too so it would have been returned as 0 by our get_distance() function]
In any of those 2 cases, we just continue iterating to check the other positions, but BEFORE we check that we are not at the last iteration of the for loop
, if we were, that necessarily means all of the positions where non viable since we would have stopped the function if they were viable.
*/

void loop()
{
  digitalWrite(13, LOW); // Making sure this is turned off when we are still awaiting master signal.
  listen_pin_status = digitalRead(listening_pin);

  if (listen_pin_status != LOW)
  { // If we get a signal from master arduino, detect obstacles and return pos.
    Serial.println("Master Arduino sent signal");
    digitalWrite(13, HIGH); // Turning on white LED to indicate that we received a signal.
    Serial.println("Detecting obstacles");
    chosen_path = detect_obstacles(); // Running detect_obstacles() that will move the servo and return the appropiate path.
    Serial.print("Chosen path: ");
    Serial.println(chosen_path);  // Setting white LED HIGH to indicate we are done with the task.
    digitalWrite(send_pin, HIGH); // Send a signal through send_pin()
    switch (chosen_path)
    { // This switch implements the delay corresponding to the path to take by deciding how much time it takes for the sent signal to stop.
    case 0:
      delay(100);
      break;
    case 1:
      delay(300);
      break;
    case 2:
      delay(500);
      break;
    case 3:
      delay(700);
      break;
    case 4:
      delay(900);
      break;
    case 5:
      delay(1100);
      break;
    }
    digitalWrite(send_pin, LOW); // Turning off send_pin --> ending the signal and obstacle detection.
    Serial.println("pin is OFF");
  }
}