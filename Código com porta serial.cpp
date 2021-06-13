#define EN 8
#define X_DIR 5
#define X_STP 2

int delayTime = 800;
int stps=1200;

void step(boolean dir, byte dirPin, byte stepperPin, int steps)
{
  digitalWrite(dirPin, dir);
  delay(100);
  while(1)
  {
    digitalWrite(stepperPin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(stepperPin, LOW);
    delayMicroseconds(delayTime);
    char c = Serial.read();
    if(c == 'p')
    {
      break;
    }
    else if(c == 'f')
    {
      step(false, X_DIR, X_STP, stps);
    }
    else if(c == 't')
    {
      step(true, X_DIR, X_STP, stps);
    }
    
  }
}

void setup()
{
  pinMode(X_DIR, OUTPUT); pinMode(X_STP,OUTPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(EN,LOW);
  Serial.begin(9600);
}
void loop()
{
  char c = Serial.read();
  if (c == 'f')
  {
     step(false, X_DIR, X_STP, stps);
  }
  delay(100);  
  if(c == 't')
  {
     step(true, X_DIR, X_STP, stps);
  }
  delay(100);
  if(c == 'p')
  {
      delay(1000);
  }

}
