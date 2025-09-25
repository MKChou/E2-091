// 密碼鎖實驗 - 不使用外部函式庫版本
// 實驗目的：學習使用Keypad及製作密碼鎖

// Keypad 硬體連接設定
const byte rowPins[4] = {A0, A1, A2, A3};    // 連接到按鍵矩陣的行腳位
const byte colPins[4] = {13, 9, A4, A5};     // 連接到按鍵矩陣的列腳位

// 七段顯示器連接設定 (共陰極)
const int segmentPins[7] = {2, 3, 4, 5, 6, 7, 8}; // a, b, c, d, e, f, g
const int digitPins[4] = {10, 11, 12, -1}; // 控制四個數字位 (只使用前3位)

// 七段顯示器數字編碼 (共陰極，0為亮)
const byte digitCode[10] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};

// 按鍵矩陣映射
const char keymap[4][4] = {
  {'1','2','3','A'},
  {'4','5','6','B'}, 
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// 狀態定義
enum SystemState {
  NORMAL_INPUT,
  CHANGE_PASSWORD_OLD,
  CHANGE_PASSWORD_NEW,
  VERIFICATION_CODE
};

// 全域變數
char correctPassword[9] = "20021216";  // 正確密碼（8位數）
char inputBuffer[9];        // 輸入暫存區（8位數 + 結束符）
int bufferIndex = 0;        // 當前輸入位置
int wrongCount = 0;         // 錯誤次數計數器
const char prompt[] = "Please enter password";
SystemState currentState = NORMAL_INPUT;
int verificationCode = 0;   // 驗證碼
char newPassword[9];        // 新密碼暫存
char tempPassword[9];       // 暫存輸入的密碼（用於驗證碼驗證後檢查）
unsigned long displayTimer = 0;  // 顯示計時器

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));  // 初始化隨機種子
  
  // 設定行腳位為輸出，初始為高電位
  for (int i = 0; i < 4; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH);
  }
  
  // 設定列腳位為輸入，啟用內部上拉電阻
  for (int i = 0; i < 4; i++) {
    pinMode(colPins[i], INPUT_PULLUP);
  }
  
  // 設定七段顯示器腳位
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
    digitalWrite(segmentPins[i], LOW); // 共陰極，初始關閉
  }
  for (int i = 0; i < 3; i++) { // 只使用前3位
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], LOW); // 初始關閉所有位
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
  
  // 在驗證碼狀態下持續顯示驗證碼
  if (currentState == VERIFICATION_CODE) {
    displayVerificationCode();
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
  switch (currentState) {
    case NORMAL_INPUT:
      handleNormalInput(key);
      break;
    case CHANGE_PASSWORD_OLD:
      handleOldPasswordInput(key);
      break;
    case CHANGE_PASSWORD_NEW:
      handleNewPasswordInput(key);
      break;
    case VERIFICATION_CODE:
      handleVerificationInput(key);
      break;
  }
}

// 處理正常密碼輸入
void handleNormalInput(char key) {
  if (key >= '0' && key <= '9') {
    // 數字鍵輸入
    if (bufferIndex < 8) {
      inputBuffer[bufferIndex] = key;
      bufferIndex++;
      Serial.println(key);
    }
  }
  else if (key == 'A' || key == 'B' || key == 'C') {
    // 非法輸入
    Serial.println("illegal input");
  }
  else if (key == 'D') {
    // 修改密碼模式
    Serial.println();
    Serial.println("Enter old password:");
    currentState = CHANGE_PASSWORD_OLD;
    resetInput();
  }
  else if (key == '*') {
    // 送出密碼
    Serial.println();
    checkPassword();
  }
  else if (key == '#') {
    // 重置輸入
    Serial.println();
    Serial.println("re-enter password");
    resetInput();
  }
}

// 處理舊密碼輸入
void handleOldPasswordInput(char key) {
  if (key >= '0' && key <= '9') {
    if (bufferIndex < 8) {
      inputBuffer[bufferIndex] = key;
      bufferIndex++;
      Serial.println(key);
    }
  }
  else if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
    Serial.println("illegal input");
  }
  else if (key == '*') {
    Serial.println();
    inputBuffer[bufferIndex] = '\0';
    if (strcmp(inputBuffer, correctPassword) == 0) {
      Serial.println("password correct, enter the new password:");
      currentState = CHANGE_PASSWORD_NEW;
      resetInput();
    } else {
      Serial.println("Wrong old password!");
      Serial.println(prompt);
      currentState = NORMAL_INPUT;
      resetInput();
    }
  }
  else if (key == '#') {
    Serial.println();
    Serial.println(prompt);
    currentState = NORMAL_INPUT;
    resetInput();
  }
}

// 處理新密碼輸入
void handleNewPasswordInput(char key) {
  if (key >= '0' && key <= '9') {
    if (bufferIndex < 8) {
      inputBuffer[bufferIndex] = key;
      bufferIndex++;
      Serial.print(key);
    }
  }
  else if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
    Serial.println("illegal input");
  }
  else if (key == '*') {
    Serial.println();
    if (bufferIndex == 8) {
      inputBuffer[bufferIndex] = '\0';
      strcpy(correctPassword, inputBuffer);
      Serial.println("Password changed successfully!");
      Serial.println(prompt);
      currentState = NORMAL_INPUT;
      resetInput();
    } else {
      Serial.println("Password must be 8 digits!");
    }
  }
  else if (key == '#') {
    Serial.println();
    Serial.println(prompt);
    currentState = NORMAL_INPUT;
    resetInput();
  }
}

// 處理驗證碼輸入
void handleVerificationInput(char key) {
  if (key >= '0' && key <= '9') {
    if (bufferIndex < 3) {
      inputBuffer[bufferIndex] = key;
      bufferIndex++;
      Serial.print(key);
    }
  }
  else if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
    Serial.println("illegal input");
  }
  else if (key == '*') {
    Serial.println();
    inputBuffer[bufferIndex] = '\0';
    int inputCode = atoi(inputBuffer);
    if (inputCode == verificationCode) {
      // 驗證碼正確，現在檢查密碼
      if (strcmp(tempPassword, correctPassword) == 0) {
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
      Serial.println(prompt);
      currentState = NORMAL_INPUT;
      resetInput();
      clearDisplay(); // 清除顯示
    } else {
      Serial.println("Wrong verification code!");
      Serial.println(prompt); // 重新提示輸入密碼
      currentState = NORMAL_INPUT; // 回到密碼輸入階段
      resetInput();
      clearDisplay(); // 清除顯示
    }
  }
  else if (key == '#') {
    Serial.println();
    Serial.println("Enter verification code:");
    resetInput();
  }
}

// 驗證密碼
void checkPassword() {
  inputBuffer[bufferIndex] = '\0'; // 添加字串結束符
  
  // 將輸入的密碼暫存起來
  strcpy(tempPassword, inputBuffer);
  
  // 不管密碼正確與否，都先進入驗證碼階段
  Serial.println("Please enter verification code:");
  generateVerificationCode();
  currentState = VERIFICATION_CODE;
  resetInput();
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

// 生成驗證碼
void generateVerificationCode() {
  verificationCode = random(100, 1000); // 生成3位數驗證碼
  Serial.print("Verification code: ");
  Serial.println(verificationCode);
}

// 顯示驗證碼到七段顯示器
void displayVerificationCode() {
  int hundreds = verificationCode / 100;
  int tens = (verificationCode % 100) / 10;
  int units = verificationCode % 10;
  
  // 顯示百位數
  displayDigit(0, hundreds);
  delay(5);
  
  // 顯示十位數
  displayDigit(1, tens);
  delay(5);
  
  // 顯示個位數
  displayDigit(2, units);
  delay(5);
}

// 在指定位置顯示數字
void displayDigit(int position, int digit) {
  // 關閉所有位
  for (int i = 0; i < 3; i++) {
    digitalWrite(digitPins[i], HIGH);
  }
  
  // 設定七段顯示器編碼
  byte code = digitCode[digit];
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], (code >> i) & 0x01);
  }
  
  // 開啟指定位
  digitalWrite(digitPins[position], LOW);
}

// 清除顯示
void clearDisplay() {
  // 關閉所有位
  for (int i = 0; i < 3; i++) {
    digitalWrite(digitPins[i], HIGH);
  }
  
  // 關閉所有段
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], HIGH);
  }
}
