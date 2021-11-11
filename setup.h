// Setting up all the pins, necessary variables and running void setup 
//////////////Initializing libraries 
//#include "Arduino.h" 
#include <IRremote.h>
#include <Servo.h>

//#ifndef __included_remote_h
//#define __included_remote_h 

///////////PINS 
int m1dir1 = 2; 
int speed1 = 3; 
int m1dir2 = 4; 
int speed2 = 5; // PWM inputs for speed of motors 1 and 2
int s_Pin = 6;                                          // Servo pin 
int m2dir1 = 7; 
int m2dir2 = 8; // Digital inputs for direction 
int recvPin = 9;                                      // IR receiver pin (not Vcc or Gnd ofc) 
int echoPin = 11;                                     // Trigger and Echo pin for sensor  
int trigPin = 12;     
int redPin = 13;                                         // LED that flashes when the IR command is not recognized                                 

//////////////////////////// Creating library objects 
Servo servo;
IRrecv remote(recvPin);
decode_results cmd;

////////////////////////////////// obstacle detection 
int angles [] = {130, 100, 60, 30, 0};        // Array of angles that the servo checks (~complete left, 45 left, center, 45 right, complete right) 
int scan[5] = {};                                       // creating array of distances that we will record in each one of  the positions
int scanning_indexes [] = {2, 3, 1, 4, 0};  // array holding the indexes in scan to check in order (first middle, then right, then left, then farthest right and farthest left
int travelTime;                                         // TravelTime for the sensor ping               
float total_dist = 0.0;
float distance_cm;                                 // distance_cm will be added to total dist each measurement before taking the average 
int average_dist;                                   // average because we take multiple measurements to estimate a distance at a given position 
int obj_location;                                     // Temp variable that is used during path selection process to lighten code 

int chosen_path;                                   // Variable holding the path to follow after running the obstacle detection 

/////////////////////////////////////////////// IR
String command;                                   // Variable holding the command that the IR receiver got, returned by the IR function "get_command()" 

///////////////////////////////////////////////////////Motors 
int cspeed = 0;  
int Is_On = 0; // Car is off at the start of code so speed is 0. Is_On is false at the start since we have to press the button on the controller to turn it on || 0 represents false and 1 true (using ints cause switch case doesn't allow bools) 
int moving_front = 1;       // Car starts in forward mode, when this is 0 then we are in reverse 
int paused = 0;               // This variable turns to 1 when the car is turned on but idle 

/////////////////////////////////////////////////////////////////////// void setup 
void setup() {
  Serial.begin(9600);

  // FOR OBSTACLE DETECTION 
  pinMode(trigPin, OUTPUT); // va send le signal
  pinMode(echoPin, INPUT); // on va listen ici
  servo.attach(s_Pin);

  //FOR IR REMOTE  
  pinMode(recvPin, INPUT);
  pinMode(redPin, OUTPUT); 
  remote.enableIRIn();

  pinMode(speed1, OUTPUT); 
  pinMode(speed2, OUTPUT); 
  pinMode(m1dir1, OUTPUT); 
  pinMode(m1dir2, OUTPUT); 
  pinMode(m2dir1, OUTPUT); 
  pinMode(m2dir2, OUTPUT); 
}
//#endif
