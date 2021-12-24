/// Main file. ///

// Including header files. 
#include "setup.h"
#include "remote.h"
#include "motors.h"
#include "moving_orders.h"
#include "auto_mode.h"

void process_command()
{                // Associating commands that the IR receives to car commands | this function is to see the general flow of the car's instructions.
  switch (Is_On) // Switch the allowed commands depending on if car is on.
  {
  case 0: // Allow these commands if the car is OFF.
    if (command == "REPEAT")
    {
      Serial.println("repeat"); // Ignoring repeats, doesn't call anything but alerts me by printing.
    }
    else if (command == "POWER")
    {
      on_off_switch();
    }
    else
    {
      Serial.println("Car is off, turn it on to pass this command");
      warning_blink();
    }
    break; // If the car is off, only command that will be accepted is POWER, else we run warning_blink()
  case 1:  // If the car is ON, then we check if it's moving, and allow commands based on that.
    switch (moving)
    {
    case 1: // Allow these commands if we are moving.
      if (command == "REPEAT")
      {
        Serial.println("repeat"); // As always, ignoring repeats.
      }
      else if (command == "POWER")
      {
        on_off_switch();
      }
      else if (command == "PAUSE")
      {
        pause();
      }
      else if (command == "SPEED+")
      {
        augment_speed();
      }
      else if (command == "SPEED-")
      {
        lower_speed();
      }
      else
      {
        Serial.println("Command not available while car moving");
        warning_blink(); // If another command is passed, send warning.
      }
      break;
    case 0: // Allow these commands if the car is not moving.
      if (command == "REPEAT")
      {
        Serial.println("repeat");
      }
      else if (command == "POWER")
      {
        on_off_switch();
      }
      else if (command == "SPEED+")
      {
        augment_speed();
      }
      else if (command == "SPEED-")
      {
        lower_speed();
      }
      else if (command == "PAUSE")
      {
        pause();
      }
      else if (command == "RIGHT")
      {
        turn_right();
      }
      else if (command == "LEFT")
      {
        turn_left();
      }
      else if (command == "FORWARD") // If we want to move forward, we first check if we are already going forward
      {
        switch (moving_front) // Moving_front is 1 when we are set to go forward.
        {
        case 1: // If we already are set to move forward, don't change it
          Serial.println("We are already set to move forward");
          warning_blink();
          break;
        case 0: // If we are not set to move forward, then change it
          info_blink();
          go_forward();
          break;
        }
      }
      else if (command == "REVERSE") // Same concept as for foward, but checking reverse.
      {
        switch (moving_front)
        {
        case 1: // If we are set to move forward, change it to reverse
          info_blink();
          go_reverse();
          break;
        case 0:
          Serial.println("We are already set to go in reverse");
          warning_blink(); // If we are already set to move in reverse, don't change it
          break;
        }
      }
      else if (command == "AUTO") // To enter autopilot mode
      {
        while (true)
        { // Start infinite loop so that autopilot keeps repeating indefinetly, until we stop it manually.
          // Not advancing straight away so that the car starts by scanning the environement first, also this let's us stop the loop after the car has moved to destination
          cspeed = 255;       // Fixing speed because else the turning values would be off depending on speed.
          go_forward();       // Making sure that we are going FORWARD.
          detect_obstacles(); // Runs the obstacle detection process, chosen_path holds the info to continue.
          while (chosen_path == 5)
          { // If we get path 5, the sensor couldn't find a proper path (see "auto_mode.h") so we will try to go back a bit and take the measurements again until theres a proper path.
            go_reverse();
            move_motors();
            delay(600);
            analogWrite(speed1, 0);
            analogWrite(speed2, 0);
            go_forward(); // Setting it to go_forward in case next run of detect obstacles finds a proper path (it will advance with go forward instead of being stuck in reverse).
            detect_obstacles();
          }
          if (chosen_path == 10) // If we get '10', that means there was a communication error (see "auto_mode.h").
          {
            Serial.println("COMMUNICATION ERROR, STOPPING AUTOPILOT");
            warning_blink(); // alerting me that the communication is having problems
            break;           // exiting autopilot, no need to use handbrake sincewe are directly in the while(true) loop
          }
          else
          { // if we get here then, we got a proper path to follow, we will turn in the proper direction, advance and then stop.
            Serial.print("We got a path to follow: ");
            Serial.println(chosen_path);
            switch (chosen_path)
            { // Now we will assign a different turning time and prepare the turning of the motors depending on the direction we want to follow, using same mtime variable that turning() uses
            case 0:
              arm_left();
              mtime = 16.7;
              break;
            case 1:
              arm_left();
              mtime = 7.8;
              break;
            case 2:
              mtime = 0;
              break;
            case 3:
              arm_right();
              mtime = 5.2;
              break;
            case 4:
              arm_right();
              mtime = 10.2;
              break;
            }
            turn();        // Since mtime is a global variable, turning will use it.
            delay(300);    // Giving time to the car to stop properly after the turn.
            move_motors(); // Moving.
            delay(1000);   // Advancing for a second. ~30cm with a 9v battery at almost full charge.
            analogWrite(speed1, 0);
            analogWrite(speed2, 0); // Stopping.
          }
          // after the an autopilot run is done, we will give us 1.5seconds to send a command and stop it if we want
          digitalWrite(info_led, HIGH); // Signaling that the period to exit out of autopilot is going on.
          tstart = millis();
          while ((millis() - tstart) <= 1500) // This period goes on for 1.5s.
          {
            if (remote.decode(&cmd)) // Not using while since we don't want to get caught in loop while counting the 1.5s.
            {
              if (cmd.value == 0xFFA25D || cmd.value == 0x24AE7D4C || cmd.value == 0xE318261B) // If it's On/Off button.
              {
                remote.resume(); // remote.resume() called before we go to handbrake so that we are ready to listen
                on_off_switch();
                goto handbrake;
              }
              else if (cmd.value == 0xFFE21D || cmd.value == 0xEE886D7F || cmd.value == 0x754D75F6 || cmd.value == 0x8BBB7589)
              {
                remote.resume();
                pause();        // FUNC/STOP
                goto handbrake; // Using goto is a bad practice, only using it here to get out of the nested loop quickly
              }
              else
              {
                // If anything else than these 2 buttons are called, continue looping and make sure to resume listening for IR codes.
                remote.resume();
              }
            }
          }
          digitalWrite(info_led, LOW); // Signaling that the "turning off" period is over.
        }
      }
      else if (command == "NONE")
      {
        Serial.println("Connection error"); // No need to blink since we already did it in remote.h, putting this so moving_commands() doesn't get called by accident in the else{}.
      }
      else
      { // This means that we received a moving command because all other commands are checked in the idle but turned on mode.
        // Since we also check for a NONE command indicating a connection error, the only possible 'else' could be a command from 0 to 9.
        moving_commands();
      }
      break;
    }
  }
handbrake:   // We jump here if goto get's called, which means we get out of autopilot.
           ; // Equivalent of pass in Python.
}

void loop()
{
  get_command();     // Await and get a command from the IR receiver.
  process_command(); // Process the command and run the related functions.
  Serial.print("Moving: ");
  Serial.println(moving);
  Serial.print("Cspeed: ");
  Serial.println(cspeed);
  Serial.print("Direction: ");
  Serial.println(moving_front);
  Serial.println("------------------------------------------------------------------");
}
