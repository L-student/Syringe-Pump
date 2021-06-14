#define EN 8
#define X_DIR 5
#define X_STP 2

int delayTime = 800;
//int stps = 1200;

void step(boolean dir)
{
  digitalWrite(X_DIR, dir);
  delay(100);
  while(1)
  {
    digitalWrite(X_STP, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(X_STP, LOW);
    delayMicroseconds(delayTime);
    char c = Serial.read();
    if(c == 'p')
    {
      break;
    }
    else if(c == 'f')
    {
      step(false);
    }
    else if(c == 't')
    {
      step(true);
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
    step(false);
  }
  delay(100);  
  else if(c == 't')
  {
    step(true);
  }
  delay(100);

}
