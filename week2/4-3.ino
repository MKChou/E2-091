const int segmentPins[] = {2, 3, 4, 5, 6, 7, 8};
const int commonPin = 9;

const int rowPins[] = {A0, A1, A2};
const int colPins[] = {13, 12, 11};

char keys[3][3] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'}
};

bool digitPatterns[10][7] = {
  {1,1,1,1,1,1,0},
  {0,1,1,0,0,0,0},
  {1,1,0,1,1,0,1},
  {1,1,1,1,0,0,1},
  {0,1,1,0,0,1,1},
  {1,0,1,1,0,1,1},
  {1,0,1,1,1,1,1},
  {1,1,1,0,0,0,0},
  {1,1,1,1,1,1,1},
  {1,1,1,1,0,1,1}
};

void setup() {
  Serial.begin(115200);
  
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  pinMode(commonPin, OUTPUT);
  digitalWrite(commonPin, LOW);
  
  for (int i = 0; i < 3; i++) {
    pinMode(rowPins[i], OUTPUT);
    pinMode(colPins[i], INPUT_PULLUP);
    digitalWrite(rowPins[i], HIGH);
  }
  
  displayDigit(0);
}

void loop() {
  char key = scanKeypad();
  
  if (key != 0) {
    Serial.println(key);
    
    int digit = key - '0';
    if (digit >= 1 && digit <= 9) {
      displayDigit(digit);
      Serial.println(digit);
    }
    
    delay(200);
  }
}

char scanKeypad() {
  for (int row = 0; row < 3; row++) {
    for (int i = 0; i < 3; i++) {
      digitalWrite(rowPins[i], HIGH);
    }
    digitalWrite(rowPins[row], LOW);
    
    for (int col = 0; col < 3; col++) {
      if (digitalRead(colPins[col]) == LOW) {
        delay(50);
        
        if (digitalRead(colPins[col]) == LOW) {
          while (digitalRead(colPins[col]) == LOW) {
            delay(10);
          }
          return keys[row][col];
        }
      }
    }
  }
  
  for (int i = 0; i < 3; i++) {
    digitalWrite(rowPins[i], HIGH);
  }
  
  return 0;
}

void displayDigit(int digit) {
  if (digit < 0 || digit > 9) {
    clearDisplay();
    return;
  }
  
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], digitPatterns[digit][i]);
  }
}

void clearDisplay() {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], LOW);
  }
}

void testDisplay() {
  for (int i = 0; i <= 9; i++) {
    Serial.println(i);
    displayDigit(i);
    delay(1000);
  }
  clearDisplay();
}
