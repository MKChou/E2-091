const int SEG_COM[4] = {10, 11, 12, 13};

const int SEG_data[10][8] = {
  {1, 1, 1, 1, 1, 1, 0, 0},
  {0, 1, 1, 0, 0, 0, 0, 0},
  {1, 1, 0, 1, 1, 0, 1, 0},
  {1, 1, 1, 1, 0, 0, 1, 0},
  {0, 1, 1, 0, 0, 1, 1, 0},
  {1, 0, 1, 1, 0, 1, 1, 0},
  {1, 0, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 0, 1, 1, 0}
};

int disp[4];
int number = 0;
unsigned long lastUpdate = 0;
const long interval = 2000;

void setup() {
  Serial.begin(9600);

  for (int i = 2; i <= 13; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
}

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastUpdate >= interval) {
    number += 2;
    if (number > 9999) {
      number = 0;
    }
    lastUpdate = currentTime;
    Serial.println(number);
  }
  
  number_transfer(number);
  
  for(int i=0; i<=3; i++){
    digitalWrite(SEG_COM[i], LOW);
    SEG_Drive(disp[i]);
    delay(5);
    digitalWrite(SEG_COM[i], HIGH);
  }
}

void number_transfer(int Num){
  disp[0] = Num % 10;
  disp[1] = (Num / 10) % 10;
  disp[2] = (Num / 100) % 10;
  disp[3] = (Num / 1000) % 10;
}

void SEG_Drive(char number){
  for(int i=2; i<=9; i++){
    if(SEG_data[number][i-2] == 1){
      digitalWrite(i, HIGH);
    } else {
      digitalWrite(i, LOW);
    }
  }
}
