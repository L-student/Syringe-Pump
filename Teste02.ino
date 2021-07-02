#include <AccelStepper.h>

// First we create the constants that we will use throughout our code
//////////////////////1/////////////////
#define MOTOR_STEPS 200
#define MICROSTEPS 32
#define TOTAL_STEPS 6400
#define X_SPEED 2000 // X steps per second
#define X_ACCEL 5000.0 // X steps per second per second
#define X_DIR     5  
#define EN        8       // stepper motor enable, low level effective (note put jumper so automatic)
#define X_STP     2       // X axis, step pin
#define BAUD_RATE 230400  // the rate at which data is read
 
 ///////////////////2/////////////////
long receivedMMdistance = 0; //distance in mm from the computer
long receivedDelay = 0; //delay between two steps, received from the computer
long receivedAcceleration = 0; //acceleration value from computer
char receivedCommand; //character for commands
bool newData, runallowed = false;

// AccelStepper is the class we use to run all of the motors in a parallel fashion
// Documentation can be found here: http://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html
AccelStepper stepper(AccelStepper::DRIVER, X_STP, X_DIR);

// Now we declare variables we will use throughout our code (That could change!)
const int ledPin = 13;
// The buffer allows us to store bytes as they are read from the python program (64 bytes in size)
const byte buffSize = 64;
char inputBuffer[buffSize];
// We create inputs to this program by <...> where <> represent the startread and endread markers
const char startMarker = '<';
const char endMarker = '>';

byte bytesRecvd = 0;
boolean readInProgress = false;
boolean newDataFromPC = false;

void setup() {
  Serial.begin(BAUD_RATE);

  stepper.setMaxSpeed(X_SPEED);
  // stepper1.setSpeed(X_SPEED);
  stepper.setAcceleration(X_ACCEL);
  // tell the PC we are ready
  Serial.println("<Arduino is ready>");
  stepper.disableOutputs();

}

void loop()
{
 
  checkSerial(); //check serial port for new commands
 
  continuousRun2(); //method to handle the motor
 
}

 
void continuousRun2() //method for the motor
{
  if (runallowed == true)
  {
    if (abs(stepper.currentPosition()) < receivedMMdistance) //abs() is needed because of the '<'
    {
      stepper.enableOutputs(); //enable pins
      stepper.run(); //step the motor (this will step the motor by 1 step at each loop)
    }
    else //program enters this part if the required distance is completed
    {
     
      runallowed = false; //disable running -> the program will not try to enter this if-else anymore
      stepper.disableOutputs(); // disable power
      Serial.print("POS: ");
      Serial.println(stepper.currentPosition()); // print pos -> this will show you the latest relative number of steps
      stepper.setCurrentPosition(0); //reset the position to zero
      Serial.print("POS: ");
      Serial.println(stepper.currentPosition()); // print pos -> this will show you the latest relative number of steps; we check here if it is zero for real
    }
 
 
  }
  else //program enters this part if the runallowed is FALSE, we do not do anything
  {
    return;
 
  }
}
 
void checkSerial() //method for receiving the commands
{  
  //switch-case would also work, and maybe more elegant
 
  if (Serial.available() > 0) //if something comes
  {
    receivedCommand = Serial.read(); // this will read the command character
    newData = true; //this creates a flag
  }
 
  if (newData == true) //if we received something (see above)
  {
    //START - MEASURE
    if (receivedCommand == 's') //this is the measure part
    {
      //example s 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed
      runallowed = true; //allow running
     
 
      receivedMMdistance = Serial.parseFloat(); //value for the steps
      receivedDelay = Serial.parseFloat(); //value for the speed
 
      Serial.print(receivedMMdistance); //print the values for checking
      Serial.print(receivedDelay);
      Serial.println("Measure "); //print the action
      stepper.setMaxSpeed(receivedDelay); //set speed
      stepper.move(receivedMMdistance); //set distance
 
    }
    //START - OPEN
    if (receivedCommand == 'o') //OPENING
    {
      //example o 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed
      runallowed = true; //allow running
     
 
 
      receivedMMdistance = Serial.parseFloat(); //value for the steps
      receivedDelay = Serial.parseFloat(); //value for the speed
 
      Serial.print(receivedMMdistance); //print the values for checking
      Serial.print(receivedDelay);
      Serial.println("OPEN "); //print the action
      stepper.setMaxSpeed(receivedDelay); //set speed
      stepper.move(receivedMMdistance); //set distance
 
    }
 
    //START - CLOSE
    if (receivedCommand == 'c') //CLOSING - Rotates the motor in the opposite direction as opening
    {
      //example c 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed; will rotate in the other direction
      runallowed = true; //allow running
     
 
 
      receivedMMdistance = Serial.parseFloat(); //value for the steps
      receivedDelay = Serial.parseFloat(); //value for the speed
 
      Serial.print(receivedMMdistance);  //print the values for checking
      Serial.print(receivedDelay);
      Serial.println("CLOSE "); //print action
      stepper.setMaxSpeed(receivedDelay); //set speed
      stepper.move(-1 * receivedMMdistance); ////set distance - negative value flips the direction
 
    }
 
    //STOP - STOP
    if (receivedCommand == 'n') //immediately stops the motor
    {
      runallowed = false; //disable running
       
      stepper.setCurrentPosition(0); // reset position
      Serial.println("STOP "); //print action
      stepper.stop(); //stop motor
      stepper.disableOutputs(); //disable power
      
 
    }
 
    //SET ACCELERATION
    if (receivedCommand == 'a') //Setting up a new acceleration value
    {
      runallowed = false; //we still keep running disabled, since we just update a variable
     
      receivedAcceleration = Serial.parseFloat(); //receive the acceleration from serial
 
      stepper.setAcceleration(receivedAcceleration); //update the value of the variable
 
      Serial.println("ACC Updated "); //confirm update by message
 
    }
 
  }
  //after we went through the above tasks, newData becomes false again, so we are ready to receive new commands again.
  newData = false;
 
 
}
