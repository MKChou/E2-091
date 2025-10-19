#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- 硬體腳位定義 ---
int trigPin = 12; // 超音波模組的 Trig 接腳
int echoPin = 11; // 超音波模組的 Echo 接腳

// --- LCD 初始化 ---
// I2C 位址可能是 0x3F 或 0x27，請依您的模組確認
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- 變數定義 ---
float duration;         // 用來儲存 Echo 接腳接收到高電位的時長 (單位: 微秒)
int cm;                 // 換算後的距離 (單位: 公分)
float soundspeed = 0.034; // 音速 (單位: 公分/微秒)
float c;

void setup() {
  // 初始化序列埠，方便除錯
  Serial.begin(9600);

  // 設定 Trig 為輸出，Echo 為輸入
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // LCD 初始化與開啟背光
  lcd.init();
  lcd.backlight();
}

void loop(){

  // --- 步驟 1: 發射超音波 ---
  // 為了確保發出乾淨的脈衝，先將 trigPin 設為低電位
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  // 發送一個 10 微秒的高電位脈衝來觸發超音波模組
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // --- 步驟 2: 接收返回訊號並計算距離 ---
  // 使用 pulseIn() 函式來測量 echoPin 維持高電位的時間，
  // 這個時間就是超音波從發射到返回所花費的時間
  duration = pulseIn(echoPin, HIGH);

  // 距離 = (時間 * 音速) / 2
  // 因為聲音是去再回來，所以時間需要除以 2
  
  c = duration * soundspeed / 2;
  if(c<1000 && c>0){
    cm=c;
  }
  
  // --- 步驟 3: 顯示在序列埠監控視窗 ---
  Serial.print("Distance: ");
  Serial.print(cm);
  Serial.println(" cm");

  // --- 步驟 4: 顯示在 LCD 螢幕上 ---
  lcd.clear();             // 清除螢幕，避免舊資料殘留
  lcd.setCursor(0, 0);     // 將游標設定到第一行 (行0) 的開頭 (欄0)
  lcd.print("Distance:");    // 在第一行顯示 "Distance:"
  
  lcd.setCursor(0, 1);     // 將游標設定到第二行 (行1) 的開頭 (欄0)
  lcd.print(cm);             // 顯示計算出來的公分
  lcd.print(" cm");         // 在後面加上單位 " cm"

  // 延遲 500 毫秒再進行下一次測量
  delay(500);
}