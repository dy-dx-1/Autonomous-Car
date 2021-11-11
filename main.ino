#include "setup.h"
#include "obstacle_detection.h"
#include "remote.h" 
#include "commands.h" 

// Servo.h et pins déjà déclarés dans main setup
// On va prendre 6 mesures d'angle

void loop() {
  get_command(); 
  process_command(); 
  // Here we need to decide what to do with the command that we got so a lot of if/else 
  //move/ do stuff with the cmmand 
  //Add option to choose between auto drive or manual, by default manual 

  chosen_path = detect_obstacles(); 

  
}
