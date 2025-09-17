int Led[5] = {8, 9, 10, 11, 12};

void setup()  
{
  for (int i = 0; i < 5; i++)
  {
  pinMode(Led[i], OUTPUT);
  }
}
 
void loop()
{
  digitalWrite(8, HIGH);
  delay(300);
  digitalWrite(9, HIGH);
  delay(300);
  digitalWrite(10, HIGH);
  delay(300);
  digitalWrite(11, HIGH);
  delay(300);
  digitalWrite(12, HIGH);
  delay(300);

  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  


  digitalWrite(12, HIGH);
  delay(300);
  digitalWrite(11, HIGH);
  delay(300);
  digitalWrite(10, HIGH);
  delay(300);
  digitalWrite(9, HIGH);
  delay(300);
  digitalWrite(8, HIGH);
  delay(300);


  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
}