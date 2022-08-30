// code written by Paul Kirk
// the code is uploaded to the Canon Lens mega2560 MCU, which accepts commands from the Canon lens driver
// the commands accepted and processed are:
// Move - with a parameter which defines the integer focuser position from  1 to 30,000
// Halt - to stop movement
// the code as always is on github under paulskirk53 projects.
//
//todo - remove ascom test prints

#include <Arduino.h>
#include <AccelStepper.h>

#define stepPin 7
#define dirPin  8
#define ASCOM Serial

//fwd declarations
void move(int position);
void halt();

AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin, true);

bool stop =false;

void setup() 
{
  ASCOM.begin(19200);
  pinMode (dirPin, OUTPUT);
  pinMode (stepPin, OUTPUT);
  stepper.setMaxSpeed(300);               // copied from the dome stepper routine
  stepper.setAcceleration(140);
  stepper.setCurrentPosition (0);        // this is the accelstepper library default.... 
    
}  // end setup



void loop() 
{


if (ASCOM.available() > 0) // request from ASCOM Driver
{
  String ReceivedData = ASCOM.readStringUntil('#');

  if (ReceivedData.indexOf("querymcu", 0) > -1) //

  {
    ASCOM.print("focuser#");
  }


 if (ReceivedData.indexOf("move", 0) > -1) //
    {
      stop = false;
      ReceivedData.remove(0, 4); // strip 1st three chars
      int position = ReceivedData.toInt();
      // todo check if position is in range
      move(position);   // move the stepper motor to the position given
    }

  if (ReceivedData.indexOf("halt", 0) > -1) //
  {

    stop = true;

  }
  
  if (ReceivedData.indexOf("statrep", 0) > -1) //
    {
      if (stepper.distanceToGo() != 0)
      {
        ASCOM.print("Moving#");
       // ASCOM.println(stepper.currentPosition());
      }
      else
      {
        ASCOM.print("notmoving#");
      }
    }

}  // endif ASCOM available

if (!stop)
  {
  stepper.run();
  }
  else
  {
    stepper.moveTo(stepper.currentPosition());    // stop the motor at its current position
    stepper.run();                                // only steps if there's a need 
    
  }

}  // end void loop

void move(int position)
{


  stepper.moveTo(position);


}
void halt()
{

  stepper.stop();
}