const int SEG_COM[4] = {10, 11, 12, 13};
const int SEG_data[10][8] = {{1, 1, 1, 1, 1, 1, 0, 0},
                      {0, 1, 1, 0, 0, 0, 0, 0},
                      {1, 1, 0, 1, 1, 0, 1, 0},
                      {1, 1, 1, 1, 0, 0, 1, 0},
                      {0, 1, 1, 0, 0, 1, 1, 0},
                      {1, 0, 1, 0, 1, 1, 1, 0},
                      {1, 0, 1, 1, 1, 1, 1, 0},
                      {1, 1, 1, 0, 0, 0, 0, 0},
                      {1, 1, 1, 1, 1, 1, 1, 0},
                      {1, 1, 1, 0, 0, 1, 1, 0}};

int disp[4] = {5, 2, 0, 2};

void setup() {
  for(int i=2; i<=13; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
}

void loop() {
  for(int i=0; i<=3; i++){
    digitalWrite(SEG_COM[i], LOW);
    SEG_Drive(disp[i]);
    delay(5);
    digitalWrite(SEG_COM[i], HIGH);
  }
}

void SEG_Drive(char number){
  for(int i=2; i<= 13; i++){
    if(SEG_data[number][i-2] == 1){
      digitalWrite(i,HIGH);
    } else {
      digitalWrite(i, LOW);
    }
  }
}
