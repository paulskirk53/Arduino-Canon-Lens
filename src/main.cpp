// code written by Paul Kirk
// the code is uploaded to the Canon Lens mega2560 MCU, which accepts commands from the Canon lens driver
// the commands accepted and processed are:
// Move - with a parameter which defines the integer focuser position from  1 to 30,000
// Halt - to stop movement
// the code as always is on github under paulskirk53 projects.
//

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
  stepper.setCurrentPosition (15000);   // halfway...
  // may need to define a max and min position, or this may be a feature of the driver.
  
}  // end setup



void loop() 
{

if (ASCOM.available() > 0) // request from ASCOM Driver
{
  String ReceivedData = ASCOM.readStringUntil('#');
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


}

if (!stop)
{
stepper.run();
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