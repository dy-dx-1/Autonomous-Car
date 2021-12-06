#include "setup.h"
#include "remote.h" 
#include "motors.h"
#include "moving_orders.h" 
#include "auto_mode.h"
// Servo.h et pins déjà déclarés dans main setup
// On va prendre 6 mesures d'angle

void process_command() {            // Associating commands that the IR receives to car commands | this function is to see the general flow of the car's instructions  
  switch (Is_On) {
    case 0:                                               // If the car is off 
      if (command == "REPEAT") { 
            Serial.println("repeat"); 
          }
      else if (command == "POWER") {
        on_off_switch();
      }
      else {
        Serial.println("Car is off, turn it on to pass this command");
        warning_blink(); 
      }
      break;                                                                                                   // If the car is off, only command that will be accepted is POWER
    case 1:                                           // If the car is on 
      switch (moving) {
        case 1:                                                       // If we are moving 
          if (command == "REPEAT") { 
            Serial.println("repeat"); 
          }
          else if (command == "POWER") {
            on_off_switch();
          }
          else if (command == "PAUSE") {
            pause();
          }
          else if (command == "SPEED+") {
            augment_speed();
          }
          else if (command == "SPEED-") {
            lower_speed();
          }
          else {
            Serial.println("Command not available while car moving"); 
            warning_blink();                                                                                      // Only these commands are accessible while the car is in movement 
          }
          break;
        case 0:                                                               // If the car is not moving 
          if (command == "REPEAT") {
            Serial.println("repeat"); 
          }
          if (command == "POWER") { 
            on_off_switch(); 
          }
          else if (command == "SPEED+") {
            augment_speed(); 
          }
          else if (command == "SPEED-") {
            lower_speed(); 
          }
          else if (command == "PAUSE") {
            pause(); 
          }
          else if (command == "RIGHT") {
            turn_right(); 
          }
          else if (command == "LEFT"){
            turn_left(); 
          }
          else if (command == "FORWARD") {
            switch(moving_front) {
              case 1:                                                                               // If we already are set to move forward, don't change it 
                Serial.println("We are already set to move forward"); 
                warning_blink(); 
                break; 
              case 0:                                                                               // If we are not set to move forward, then change it 
                go_forward(); 
                break; 
            }
          } 
          else if (command == "REVERSE") {
            switch (moving_front) { 
              case 1:                                                                                         // If we are set to move forward, change it to reverse 
                go_reverse(); 
                break; 
              case 0: 
                Serial.println("We are already set to go in reverse");                                  
                warning_blink();                                                                                  // If we are already set to move in reverse, don't change it 
                break;               
            }
          } 
          else if (command == "AUTO"){ 
            while (true) {                                                // start infinite loop so that autopilot keeps repeating indefinetly 
              while (remote.decode(&cmd) == 0) {          // while waiting for command, do the autopilot 
                Serial.println("Autopilot lol ");
                autopilot();   
              }
              // If we get out of this loop, we received a command, check if its a power or pause and if it isnt then continue looping through autopilot 
              if (cmd.value == 0xFFA25D || cmd.value == 0x24AE7D4C || cmd.value == 0xE318261B) {
                on_off_switch();     // On Off button
                break; 
              }
              else if (cmd.value == 0xFFE21D || cmd.value == 0xEE886D7F || cmd.value == 0x754D75F6 || cmd.value == 0x8BBB7589) {  
                pause();             // FUNC/STOP  
                break; 
              }
              else {   
                 // if anything else than these 2 functions are called, continue looping (like a boss ) 
              }
            } 
          }
          else if (command == "NONE") {
            Serial.println("Connection error");                        // No need to blink since we already did it in remote.h 
          }
          else {   // this means that we received a moving command because all commands are checked in the idle but turned on mode. Since we also check for a NONE command indicating a connection error, the only possible 'else' could be a command from 1 to 9 
            moving_commands(); 
          }
          break;
      }
  }
}

void loop() {
  get_command(); 
  process_command(); 
  Serial.print("Moving: "); 
  Serial.println(moving); 
  Serial.print("Cspeed: "); 
  Serial.println(cspeed); 
  Serial.print("Direction: "); 
  Serial.println(moving_front); 
  Serial.println("------------------------------------------------------------------"); 
  // Here we need to decide what to do with the command that we got so a lot of if/else 
  //move/ do stuff with the cmmand 
  //Add option to choose between auto drive or manual, by default manual 

  
}
