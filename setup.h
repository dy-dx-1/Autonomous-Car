// Setting up all the pins, necessary variables and running void setup//

 ////////////////////////////////////////////////////////////////////////////////////////////////TODO: clean up obstacle detection - remove redundant (parts in slave) 
//////////////Initializing libraries 

#include <IRremote.h>


///////////PINS 

int m1dir1 = 2; 
int commpin = 3; 

int m1dir2 = 4; 
int speed1 = 10; 
int speed2 = 5; // PWM inputs for speed of motors 1 and 2 and digital inputs for direction of motor 1 

int m2dir1 = 7; 
int m2dir2 = 8;                 // Digital inputs for direction of motor 2 
int recvPin = 9;                                      // IR receiver pin (not Vcc or Gnd ofc) 
                                     /////////////////////////////////////////////////////////////////was 11 before tests of for1 motor || 11 is digital with IR lib    
int warning_led = 13;                                         // LED that flashes when the IR command is not recognized    
int on_led = 17;                                        // LED that flashed when car on                              
int info_led = 19;                                      // 19 corresponds to the A5 pin (analog pins A0 to A5 can be used as digital pins on the nano according to google!) (A6 and A7 can't though) [A0 -> pin14 ; A5->pin19]
                                          // LED that flashes to transmit info when moving (changing direction, advancing, turning)  
int listen_pin = 14;                // Analog pin that will listen to the info transmitted back by slave arduino 

//////////////////////////// Creating library objects                           
IRrecv remote(recvPin);                         
decode_results cmd;

////////////////////////////////// obstacle detection 
int delta; 
bool listening; 

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
int moving = 0;                 // 1 when in movement, 0 when idle 

int mtime; // Will hold the int version of command (used in moving_orders.h) 
unsigned long tstart; // Will hold the first boundary of our time count || is used in motors.h and obstacle_detection to calculate different intervals 
/////////////////////////////////////////////////////////////////////// void setup 
void setup() {
  Serial.begin(9600);

  //FOR IR REMOTE  
  pinMode(recvPin, INPUT);
  pinMode(warning_led, OUTPUT); 
  pinMode(on_led, OUTPUT); 
  pinMode(info_led, OUTPUT); 
  remote.enableIRIn();

  pinMode(speed1, OUTPUT); 
  pinMode(speed2, OUTPUT); 
  pinMode(m1dir1, OUTPUT); 
  pinMode(m1dir2, OUTPUT); 
  pinMode(m2dir1, OUTPUT); 
  pinMode(m2dir2, OUTPUT); 

  pinMode(listen_pin, INPUT); 
  pinMode(commpin, OUTPUT); 
}
