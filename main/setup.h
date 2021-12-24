/// Setting up all the pins, the necessary variables for the code and running void setup. ///

#include <IRremote.h> // This is included here so that we can initialize the IRrecv object.

////////////////////////////// PINS

int m1dir1 = 2;  // m1dir1, m1dir2, m2dir1 and m2dir2 are the pins that set the direction for motors 1 and 2.
int commpin = 3; // Pin that sends signal to slave arduino to start obstacle detection.
int m1dir2 = 4;
int speed1 = 10; // Equivalence to motors IRL on line 2 of "motors.h"
int speed2 = 5;  // PWM inputs for speed of motors 1 and 2.
int m2dir1 = 7;
int m2dir2 = 8;

int recvPin = 9; // IR receiver data pin.

int warning_led = 11; // LED that flashes for minor warnings like trying to pass an unavailable command.
int error_led = 13;   // LED that flashes when an IR command is not recognized.
int listen_pin = 14;  // Analog pin that will listen to the info transmitted back by slave arduino
int on_led = 17;      // LED that is on when the car is on.
int info_led = 19;    // 19 corresponds to the A5 pin (analog pins A0 to A5 can be used as digital pins on the nano) (A6 and A7 can't though) [A0 -> pin14 ; A5->pin19]
                      // info_LED that flashes to transmit different types of information & status's(changing direction, advancing, turning, detecting obstacles, etc).

////////////////////////////// Creating objects from IR library & command variable.
IRrecv remote(recvPin);
decode_results cmd;
String command; // Variable holding the command that the IR receiver got, returned by the IR function "get_command()".

////////////////////////////// Necessary variables for the autopilot. -- Used in "auto_mode.h".
int delta;       // Used to calculate the timings of the info sent between the arduinos.
bool listening;  // Indicates if we are waiting for a response from the slave arduino.
int chosen_path; // Variable holding the path to follow after running the obstacle detection.

////////////////////////////// Necessary variables for notor operation. Used in "motors.h" and "moving_orders.h"
int cspeed = 0;       // Speed of the car. Is 0 at the start.
int Is_On = 0;        // 0 means OFF and 1 means ON. Not using bool because switch/case didn't allow it. When code starts car is OFF so value of 0.
int moving_front = 1; // Car starts in "forward" mode, when this is 0 then we are in reverse.
int moving = 0;       // 1 when in movement, 0 when idle.

float mtime;          // Will hold the int version of command (used in moving_orders.h). Needs to be a float because the turning times in autopilot are not ints.
unsigned long tstart; // Will hold the first boundary of our time count, is used in "motors.h" and "auto_mode.h" to calculate different intervals

////////////////////////////// SETUP
void setup()
{
  Serial.begin(9600);

  // FOR IR REMOTE
  pinMode(recvPin, INPUT);
  remote.enableIRIn();

  // LED'S
  pinMode(warning_led, OUTPUT);
  pinMode(on_led, OUTPUT);
  pinMode(info_led, OUTPUT);
  pinMode(error_led, OUTPUT);

  // MOTORS
  pinMode(speed1, OUTPUT);
  pinMode(speed2, OUTPUT);
  pinMode(m1dir1, OUTPUT);
  pinMode(m1dir2, OUTPUT);
  pinMode(m2dir1, OUTPUT);
  pinMode(m2dir2, OUTPUT);

  // COMMUNICATION WITH SLAVE ARDUINO
  pinMode(listen_pin, INPUT);
  pinMode(commpin, OUTPUT);
}
