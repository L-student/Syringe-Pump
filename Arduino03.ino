#include <AccelStepper.h>
// First we create the constants that we will use throughout our code
//////////////////////1/////////////////
#define MOTOR_STEPS 200
#define MICROSTEPS 32
#define TOTAL_STEPS 6400
#define X_SPEED 2000 
#define X_ACCEL 5000.0 
#define X_DIR     5  
#define EN        8       
#define X_STP     2       
#define BAUD_RATE 230400  
///////////////////2/////////////////
long receivedMMdistance = 0; //distance in mm from the computer
long receivedDelay = 0; //delay between two steps, received from the computer
long receivedAcceleration = 0; //acceleration value from computer
char receivedCommand; //character for commands
bool newData, runallowed = false;
AccelStepper stepper(AccelStepper::DRIVER, X_STP, X_DIR);

void setup() {
  Serial.begin(BAUD_RATE);
  stepper.setMaxSpeed(X_SPEED);
  stepper.setAcceleration(X_ACCEL);
  Serial.println("<Arduino is ready>");
  stepper.disableOutputs();

}
void loop() {
 
  checkSerial(); //check serial port for new commands
  continuousRun2(); //method to handle the motor
}
void continuousRun2() {
  if (runallowed == true)
  {
    if (abs(stepper.currentPosition()) < receivedMMdistance) {
      stepper.enableOutputs(); //enable pins
      stepper.run(); //step the motor (this will step the motor by 1 step at each loop)
    }
    else {
      runallowed = false; //disable running -> the program will not try to enter this if-else anymore
      stepper.disableOutputs(); // disable power
      Serial.print("POS: ");
      Serial.println(stepper.currentPosition()); // print pos -> this will show you the latest relative number of steps
      stepper.setCurrentPosition(0); //reset the position to zero
      Serial.print("POS: ");
      Serial.println(stepper.currentPosition()); // print pos -> this will show you the latest relative number of steps; we check here if it is zero for real
    }
  } else {
    return;
  }
}
 
void checkSerial() {  
  if (Serial.available() > 0) {
    receivedCommand = Serial.read(); // this will read the command character
    newData = true; //this creates a flag
  }
 
  if (newData == true) {
    //START - MEASURE
    if (receivedCommand == 's') {
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
    if (receivedCommand == 'o') {
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
    if (receivedCommand == 'c') {
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
    if (receivedCommand == 'n') {
      runallowed = false; //disable running
      stepper.setCurrentPosition(0); // reset position
      Serial.println("STOP "); //print action

      stepper.stop(); //stop motor
      stepper.disableOutputs(); //disable power
    }
 
    //SET ACCELERATION
    if (receivedCommand == 'a') {
      runallowed = false; //we still keep running disabled, since we just update a variable
      receivedAcceleration = Serial.parseFloat(); //receive the acceleration from serial

      stepper.setAcceleration(receivedAcceleration); //update the value of the variable
      Serial.println("ACC Updated "); //confirm update by message
 
    }
  }
  newData = false;
}
