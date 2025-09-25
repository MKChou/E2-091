// 密碼鎖實驗 - 不使用外部函式庫版本
// 實驗目的：學習使用Keypad及製作密碼鎖

// Keypad 硬體連接設定
const byte rowPins[4] = {3, 4, 5, 6};    // 連接到按鍵矩陣的行腳位
const byte colPins[4] = {7, 8, 9, 10};   // 連接到按鍵矩陣的列腳位

// 按鍵矩陣映射
const char keymap[4][4] = {
  {'1','2','3','A'},
  {'4','5','6','B'}, 
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// 全域變數
const char correctPassword[] = "20021216";  // 正確密碼（8位數）
char inputBuffer[9];        // 輸入暫存區（8位數 + 結束符）
int bufferIndex = 0;        // 當前輸入位置
int wrongCount = 0;         // 錯誤次數計數器
const char prompt[] = "Please enter password";

void setup() {
  Serial.begin(9600);
  
  // 設定行腳位為輸出，初始為高電位
  for (int i = 0; i < 4; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH);
  }
  
  // 設定列腳位為輸入，啟用內部上拉電阻
  for (int i = 0; i < 4; i++) {
    pinMode(colPins[i], INPUT_PULLUP);
  }
  
  delay(100);
  Serial.println(prompt);
  
  // 初始化輸入緩衝區
  clearBuffer();
}

void loop() {
  char key = readKeypad();
  
  if (key != 0) { // 有按鍵被按下
    handleKeyPress(key);
    delay(200); // 防止按鍵彈跳
  }
}

// 讀取按鍵矩陣
char readKeypad() {
  for (int row = 0; row < 4; row++) {
    // 設定當前行為低電位，其他行為高電位
    for (int i = 0; i < 4; i++) {
      digitalWrite(rowPins[i], HIGH);
    }
    digitalWrite(rowPins[row], LOW);
    
    // 檢查每一列
    for (int col = 0; col < 4; col++) {
      if (digitalRead(colPins[col]) == LOW) {
        // 找到被按下的按鍵
        return keymap[row][col];
      }
    }
  }
  
  // 沒有按鍵被按下
  return 0;
}

// 處理按鍵輸入
void handleKeyPress(char key) {
  if (key >= '0' && key <= '9') {
    // 數字鍵輸入
    if (bufferIndex < 8) {
      inputBuffer[bufferIndex] = key;
      bufferIndex++;
      Serial.println(key);  // 改為 println 讓每個數字輸入後都換行
    }
  }
  else if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
    // 非法輸入
    Serial.println("illegal input");
  }
  else if (key == '*') {
    // 送出密碼
    Serial.println();
    checkPassword();
    resetInput();
  }
  else if (key == '#') {
    // 重置輸入
    Serial.println();
    Serial.println("re-enter password");
    resetInput();
  }
}

// 驗證密碼
void checkPassword() {
  inputBuffer[bufferIndex] = '\0'; // 添加字串結束符
  
  if (strcmp(inputBuffer, correctPassword) == 0) {
    // 密碼正確
    Serial.println("Unlock!");
    wrongCount = 0; // 重置錯誤計數器
  } else {
    // 密碼錯誤
    Serial.println("Wrong passwords!");
    wrongCount++;
    
    if (wrongCount == 3) {
      Serial.println("forget the password?");
      wrongCount = 0; // 重置錯誤計數器
    }
  }
}

// 重置輸入
void resetInput() {
  bufferIndex = 0;
  clearBuffer();
}

// 清空緩衝區
void clearBuffer() {
  for (int i = 0; i < 9; i++) {
    inputBuffer[i] = 0;
  }
}
