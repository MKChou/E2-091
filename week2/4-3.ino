// 七段顯示器引腳配置
// 根據您的連接：a->Pin2, b->Pin3, c->Pin4, d->Pin5, e->Pin6, f->Pin7, g->Pin8
// 如果段a不亮，嘗試調整段引腳順序
// 常見的調整：將段順序改為 a, f, g, e, d, c, b
// 段順序: a, f, g, e, d, c, b (對應 Pin2, Pin7, Pin8, Pin6, Pin5, Pin4, Pin3)
const int segmentPins[] = {2, 7, 8, 6, 5, 4, 3};
const int commonPin = 9;  // 共陰極 (Pin9)

const int rowPins[] = {A0, A1, A2};
const int colPins[] = {13, 12, 11};

char keys[3][3] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'}
};

// 數字 0-9 的七段顯示模式 (共陰極: 1=亮, 0=滅)
// 順序: [a, b, c, d, e, f, g]
bool digitPatterns[10][7] = {
  {1,1,1,1,1,1,0},  // 0: a,b,c,d,e,f
  {0,1,1,0,0,0,0},  // 1: b,c
  {1,1,0,1,1,0,1},  // 2: a,b,d,e,g
  {1,1,1,1,0,0,1},  // 3: a,b,c,d,g
  {0,1,1,0,0,1,1},  // 4: b,c,f,g
  {1,0,1,1,0,1,1},  // 5: a,c,d,f,g
  {1,0,1,1,1,1,1},  // 6: a,c,d,e,f,g
  {1,1,1,0,0,0,0},  // 7: a,b,c
  {1,1,1,1,1,1,1},  // 8: 全部段
  {1,1,1,1,0,1,1}   // 9: a,b,c,d,f,g
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
  // 測試段順序：取消下面這行的註解來逐一點亮每個段
  // testSegments();
}

void loop() {
  char key = scanKeypad();
  
  if (key != 0) {
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

// 測試函數：逐一點亮每個段，用於確認段順序
// 在 setup() 中調用此函數來測試
void testSegments() {
  Serial.println("測試段順序：");
  for (int i = 0; i < 7; i++) {
    Serial.print("段 ");
    Serial.print(i);
    Serial.print(" (Pin ");
    Serial.print(segmentPins[i]);
    Serial.println(")");
    // 清除所有段
    clearDisplay();
    delay(500);
    // 點亮當前段
    digitalWrite(segmentPins[i], HIGH);
    delay(2000);
  }
  clearDisplay();
}
