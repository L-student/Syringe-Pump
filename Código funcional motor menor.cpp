#define EN 8

//Direction pin
#define X_DIR 5

//Step pin
#define X_STP 2

//A498
int delayTime = 800;
int stps=1200;

void step(boolean dir, byte dirPin, byte stepperPin, int steps)
{
  digitalWrite(dirPin, dir);
  delay(100);
  for (int i = 0; i< steps; i++)
  {
    digitalWrite(stepperPin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(stepperPin, LOW);
    delayMicroseconds(delayTime);
  }
}

void setup()
{
  pinMode(X_DIR, OUTPUT); pinMode(X_STP,OUTPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(EN,LOW);
}
void loop()
{
  step(false, X_DIR, X_STP, stps);
  delay(1000);
  step(true, X_DIR, X_STP, stps);
  delay(1000);
}


