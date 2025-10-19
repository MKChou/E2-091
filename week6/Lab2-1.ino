//步進馬達角度控制
//按鈕A：按1下時，馬達順轉45度；長按2秒以上，馬達順轉360度
//按鈕B：按1下時，馬達逆轉60度；長按2秒以上，馬達逆轉245度

int t = 10;  // 換相所需的時間
int Count;
int but[2] = {3, 4};

unsigned long debounceDelay = 200; // 去彈跳時間（毫秒）
unsigned long lastDebounceTime[2] = {0, 0};  // 上次去彈跳時間
int lastButtonState[2] = {HIGH, HIGH};  // 上一次按鈕狀態
int buttonState[2] = {HIGH, HIGH};  // 當前按鈕狀態
unsigned long pressStartTime[2] = {0, 0}; // 按下的開始時間

void setup() {
  Serial.begin(9600);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(but[0], INPUT_PULLUP);
  pinMode(but[1], INPUT_PULLUP);
}

void loop() {
  // 處理每個按鈕
  for (int i = 0; i < 2; i++) {
    int reading = digitalRead(but[i]);
    // 去彈跳邏輯
    if (reading != lastButtonState[i]) {
      lastDebounceTime[i] = millis();
    }
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (reading != buttonState[i]) {
        buttonState[i] = reading;

        // 按鈕狀態改變時
        if (buttonState[i] == LOW) {
          pressStartTime[i] = millis(); // 記錄按鈕被按下的開始時間
          Count = 0;
        } else {
          unsigned long pressDuration = millis() - pressStartTime[i];
          if (pressDuration >= 2000) {
            // 長按
            if(i == 0){
              // 按鈕A長按：順轉360度
              rotate(360);
            } else {
              // 按鈕B長按：逆轉245度
              rotate(-245);
            }
          } else {
            // 短按
            if(i == 0){
              // 按鈕A短按：順轉45度
              rotate(45);
            } else {
              // 按鈕B短按：逆轉60度
              rotate(-60);
            }
          }
        }
      }
    }
    lastButtonState[i] = reading;
  }
}

void rotate(int angle){
  float currentAngle = 0;
  int steps = 0;
  
  // 計算所需步數：每步約0.087890625度，所以360度需要4096步
  int totalSteps = abs(angle) * 4096 / 360;
  
  if(angle > 0){
    // 順時針旋轉
    while(steps < totalSteps){
      // 單相激磁順序
      digitalWrite(7, 0); 
      digitalWrite(8, 0); 
      digitalWrite(9, 0); 
      digitalWrite(10, 1); 
      delay(t);  
      
      digitalWrite(7, 0); 
      digitalWrite(8, 0); 
      digitalWrite(9, 1); 
      digitalWrite(10, 0); 
      delay(t);  
      
      digitalWrite(7, 0); 
      digitalWrite(8, 1); 
      digitalWrite(9, 0); 
      digitalWrite(10, 0); 
      delay(t); 
      
      digitalWrite(7, 1); 
      digitalWrite(8, 0); 
      digitalWrite(9, 0); 
      digitalWrite(10, 0); 
      delay(t);  
      
      steps += 4;
      currentAngle += 4 * 0.087890625;
    }
  } else {
    // 逆時針旋轉
    while(steps < totalSteps){
      // 單相激磁順序（反向）
      digitalWrite(7, 1); 
      digitalWrite(8, 0); 
      digitalWrite(9, 0); 
      digitalWrite(10, 0); 
      delay(t);  
      
      digitalWrite(7, 0); 
      digitalWrite(8, 1); 
      digitalWrite(9, 0); 
      digitalWrite(10, 0); 
      delay(t); 
      
      digitalWrite(7, 0); 
      digitalWrite(8, 0); 
      digitalWrite(9, 1); 
      digitalWrite(10, 0); 
      delay(t); 
      
      digitalWrite(7, 0); 
      digitalWrite(8, 0); 
      digitalWrite(9, 0); 
      digitalWrite(10, 1); 
      delay(t);  
      
      steps += 4;
      currentAngle -= 4 * 0.087890625;
    }
  }
  
  // 顯示步數
  Serial.print("Steps: ");
  Serial.println(steps);
  
  // 停止所有線圈
  digitalWrite(7, 0);
  digitalWrite(8, 0);
  digitalWrite(9, 0);
  digitalWrite(10, 0);
}
