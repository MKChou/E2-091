const byte ROWS = 4;
const byte COLS = 4;

const byte colPins[COLS] = {2, 3, 4, 5};
const byte rowPins[ROWS] = {A0, A1, A2, A3};

const char keymap[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

char lastKey = 0;
unsigned long lastKeyTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(115200);
  
  for(int i = 0; i < ROWS; i++) {
    pinMode(rowPins[i], INPUT_PULLUP);
  }
  
  for(int i = 0; i < COLS; i++) {
    pinMode(colPins[i], OUTPUT);
    digitalWrite(colPins[i], HIGH);
  }
}

void loop() {
  char key = scanKeypad();
  
  if(key != 0) {
    Serial.println(key);
  }
  
  delay(10);
}

char scanKeypad() {
  for(int col = 0; col < COLS; col++) {
    digitalWrite(colPins[col], LOW);
    
    for(int row = 0; row < ROWS; row++) {
      int analogValue = analogRead(rowPins[row]);
      
      if(analogValue < 512) {
        char currentKey = keymap[row][col];
        
        if(currentKey != lastKey || (millis() - lastKeyTime) > debounceDelay) {
          lastKey = currentKey;
          lastKeyTime = millis();
          
          while(analogRead(rowPins[row]) < 512) {
            delay(10);
          }
          
          digitalWrite(colPins[col], HIGH);
          return currentKey;
        }
      }
    }
    
    digitalWrite(colPins[col], HIGH);
  }
  
  return 0;
}
