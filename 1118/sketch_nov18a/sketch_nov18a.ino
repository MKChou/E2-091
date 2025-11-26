//步進馬達角度控制
//按鈕A：按1下時，馬達順轉45度；長按2秒以上，馬達順轉360度
//按鈕B：按1下時，馬達逆轉60度；長按2秒以上，馬達逆轉245度

int t = 50;  // 換相所需的時間（毫秒）- 增加延遲確保電機有足夠時間響應
int Count;
int but[2] = {3, 4};

unsigned long debounceDelay = 50; // 去彈跳時間（毫秒）
int lastButtonState[2] = {HIGH, HIGH};  // 上一次按鈕狀態
int buttonState[2] = {HIGH, HIGH};  // 當前按鈕狀態
unsigned long pressStartTime[2] = {0, 0}; // 按下的開始時間
bool buttonPressed[2] = {false, false}; // 按鈕是否正在被按下
unsigned long lastDebounceTime[2] = {0, 0}; // 去彈跳計時

void setup() {
  Serial.begin(9600);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(but[0], INPUT_PULLUP);
  pinMode(but[1], INPUT_PULLUP);
  
  // 初始化時停止所有線圈
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  
  Serial.println("系統初始化完成");
  Serial.println("開始電機測試...");
  delay(1000);
  
  // 測試電機：順時針轉幾步
  testMotor();
  
  Serial.println("電機測試完成，等待按鈕輸入...");
}

void loop() {
  // 處理每個按鈕
  for (int i = 0; i < 2; i++) {
    int reading = digitalRead(but[i]);
    
    // 非阻塞去彈跳邏輯
    if (reading != lastButtonState[i]) {
      lastDebounceTime[i] = millis();
    }
    
    // 如果狀態穩定超過去彈跳時間
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      // 如果穩定狀態與記錄的狀態不同
      if (reading != buttonState[i]) {
        buttonState[i] = reading;
        
        if (buttonState[i] == LOW) {
          // 按鈕被按下
          if (!buttonPressed[i]) {
            pressStartTime[i] = millis();
            buttonPressed[i] = true;
            Serial.print("按鈕");
            Serial.print(i == 0 ? "A" : "B");
            Serial.println(" 按下");
          }
        } else {
          // 按鈕被釋放
          if (buttonPressed[i]) {
            unsigned long pressDuration = millis() - pressStartTime[i];
            Serial.print("按鈕");
            Serial.print(i == 0 ? "A" : "B");
            Serial.print(" 釋放，持續時間: ");
            Serial.print(pressDuration);
            Serial.println(" ms");
            
            // 判斷長按或短按並執行動作
            if (pressDuration >= 2000) {
              // 長按
              Serial.print(">>> 執行");
              Serial.print(i == 0 ? "按鈕A" : "按鈕B");
              Serial.println("長按動作");
              if(i == 0){
                // 按鈕A長按：順轉360度
                rotate(180);
              } else {
                // 按鈕B長按：逆轉245度
                rotate(-245);
              }
            } else if (pressDuration >= 30) {
              // 短按（至少30ms）
              Serial.print(">>> 執行");
              Serial.print(i == 0 ? "按鈕A" : "按鈕B");
              Serial.println("短按動作");
              if(i == 0){
                // 按鈕A短按：順轉45度
                rotate(20);

                
              } else {
                // 按鈕B短按：逆轉60度
                rotate(-60);
              }
            } else {
              Serial.print(">>> 按鈕");
              Serial.print(i == 0 ? "A" : "B");
              Serial.println("按下時間太短，忽略");
            }
            
            buttonPressed[i] = false;
            pressStartTime[i] = 0;
          }
        }
      }
    }
    
    lastButtonState[i] = reading;
  }
}



void rotate(int angle){
  Serial.println("========================================");
  Serial.print("開始旋轉，角度: ");
  Serial.println(angle);
  
  // 計算所需步數：每步約0.087890625度，所以360度需要4096步
  // 使用 unsigned long 避免溢出，並確保計算正確
  unsigned long totalSteps = (unsigned long)abs(angle) * 4096UL / 360UL;
  
  Serial.print("總步數: ");
  Serial.println(totalSteps);
  
  // 如果步數為0，直接返回
  if(totalSteps == 0) {
    Serial.println("警告：步數為0，不執行旋轉");
    return;
  }
  
  // 安全檢查：如果步數異常大，可能是計算錯誤
  if(totalSteps > 100000) {
    Serial.print("錯誤：步數異常大 (");
    Serial.print(totalSteps);
    Serial.println(")，可能是計算錯誤");
    return;
  }
  
  unsigned long steps = 0;
  
  if(angle > 0){
    // 順時針旋轉
    Serial.println("方向：順時針旋轉");
    Serial.println("開始執行...");
    while(steps < totalSteps){
      // 單相激磁順序：10 -> 9 -> 8 -> 7
      digitalWrite(7, LOW); 
      digitalWrite(8, LOW); 
      digitalWrite(9, LOW); 
      digitalWrite(10, HIGH); 
      delay(t);  
      
      digitalWrite(7, LOW); 
      digitalWrite(8, LOW); 
      digitalWrite(9, HIGH); 
      digitalWrite(10, LOW); 
      delay(t);  
      
      digitalWrite(7, LOW); 
      digitalWrite(8, HIGH); 
      digitalWrite(9, LOW); 
      digitalWrite(10, LOW); 
      delay(t); 
      
      digitalWrite(7, HIGH); 
      digitalWrite(8, LOW); 
      digitalWrite(9, LOW); 
      digitalWrite(10, LOW); 
      delay(t);  
      
      steps += 4;
      
      // 每50步顯示一次進度（小角度時更頻繁顯示）
      if(steps % 50 == 0 || steps == 4) {
        Serial.print("進度: ");
        Serial.print(steps);
        Serial.print("/");
        Serial.print(totalSteps);
        Serial.print(" (");
        Serial.print((steps * 100) / totalSteps);
        Serial.println("%)");
      }
    }
  } else {
    // 逆時針旋轉
    Serial.println("方向：逆時針旋轉");
    Serial.println("開始執行...");
    while(steps < totalSteps){
      // 單相激磁順序（反向）：7 -> 8 -> 9 -> 10
      digitalWrite(7, HIGH); 
      digitalWrite(8, LOW); 
      digitalWrite(9, LOW); 
      digitalWrite(10, LOW); 
      delay(t);  
      
      digitalWrite(7, LOW); 
      digitalWrite(8, HIGH); 
      digitalWrite(9, LOW); 
      digitalWrite(10, LOW); 
      delay(t); 
      
      digitalWrite(7, LOW); 
      digitalWrite(8, LOW); 
      digitalWrite(9, HIGH); 
      digitalWrite(10, LOW); 
      delay(t); 
      
      digitalWrite(7, LOW); 
      digitalWrite(8, LOW); 
      digitalWrite(9, LOW); 
      digitalWrite(10, HIGH); 
      delay(t);  
      
      steps += 4;
      
      // 每50步顯示一次進度（小角度時更頻繁顯示）
      if(steps % 50 == 0 || steps == 4) {
        Serial.print("進度: ");
        Serial.print(steps);
        Serial.print("/");
        Serial.print(totalSteps);
        Serial.print(" (");
        Serial.print((steps * 100) / totalSteps);
        Serial.println("%)");
      }
    }
  }
  
  // 顯示實際步數（確保不會溢出）
  Serial.print("完成，實際步數: ");
  Serial.println((long)steps);
  
  // 停止所有線圈
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  
  Serial.println("旋轉完成");
  Serial.println("========================================");
}
