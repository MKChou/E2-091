const int Led[5] = {8, 9, 10, 11, 12};
const int sw_up = 2;
int pos = 1;

void setup() {
for (int i = 0; i < 5; i++) {
  pinMode(Led[i], OUTPUT);
  digitalWrite(Led[i], LOW);
}

digitalWrite(Led[pos], HIGH);
digitalWrite(Led[(pos+1)%5], HIGH);
pinMode(sw_up, INPUT_PULLUP);
}

void loop() {
bool swstate_up = digitalRead(sw_up);

if (swstate_up == LOW) {
    digitalWrite(Led[pos], LOW);
    digitalWrite(Led[(pos+1)%5], LOW);
    
    pos = (pos - 1 + 5) % 5;
    
    digitalWrite(Led[pos], HIGH);
    digitalWrite(Led[(pos+1)%5], HIGH);
    
    delay(200);
}
}