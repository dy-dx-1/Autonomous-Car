#include "motors.h"
// jusqua mntn le include est ici parce qu'on run des fonctions ext dans le header. c'est pas recommandé mais ca clear up le code (le mettre dans remote.h serait trop cluttered et le mettre dans main aussi) si jamais il faut acceder a motors.h dans main, il faudra bouger tout ce code la bas
// et drop l'idée de modulariser cette partie spécifiquement
void process_command() {
  switch (Is_On) {
    case 0:
      if (command == "POWER"){
        on_off_switch(); 
      }
      else if (command == "FORWARD") {
        direction_front(); 
      }
      else if (command == "REVERSE") {
        direction_back(); 
      }
      else {
        Serial.println("Car is off, turn it on to pass this command");
        //ADD BLINKING LED OR SOMETIHNG
      }
      break;
    case 1:
      if (command == "REPEAT") {
        // do nothing because theres no use to the repeat function in this car project
      }
      else if (command == "POWER") {
        on_off_switch();
      }
      else if (command == "SPEED+") {
        augment_speed();
      }
      else if (command == "SPEED-") {
        lower_speed();
      }
      else if (command == "FORWARD") {
        direction_front(); 
      }
      else if (command == "REVERSE") {
        direction_back(); 
      }
      break; 
  }

}
