#include "motors.h"
#include "moving_orders.h" 
// jusqua mntn le include est ici parce qu'on run des fonctions ext dans le header. c'est pas recommandé mais ca clear up le code (le mettre dans remote.h serait trop cluttered et le mettre dans main aussi) si jamais il faut acceder a motors.h dans main, il faudra bouger tout ce code la bas
// et drop l'idée de modulariser cette partie spécifiquement
void process_command() {
  switch (Is_On) {
    case 0:
      if (command == "POWER") {
        on_off_switch();
      }
      else {
        Serial.println("Car is off, turn it on to pass this command");
        //ADD BLINKING LED OR SOMETIHNG
      }
      break;                    // If the car is off, only command that will be accepted is POWER
    case 1:
      switch (moving) {
        case 1:
          if (command == "POWER") {
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
          }
          break;
        case 0:
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
            if (moving_front == 1) {
              Serial.println("Already going front"); 
            }
            else {
              go_forward();   
            }
          }
          else if (command == "REVERSE") {
            if (moving_front == 0) {
              Serial.println("Already going in reverse"); 
            }
            else { 
              go_reverse();
            } 
          }
          else if (command == "AUTO"){ 
            autopilot(); //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
          }
          else if (command == "NONE") {
            Serial.println("Connection error"); 
          }
          else {   // this means that we received a moving command because all commands are checked in the idle but turned on mode. Since we also check for a NONE command indicating a connection error, the only possible 'else' could be a command from 1 to 9 
            moving_commands(); 
          }
          break;
      }

  }

}
