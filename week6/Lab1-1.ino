#include <Wire.h>
#include <LiquidCrystal_I2C.h>
int trigPin = 12;
int echoPin = 11;
LiquidCrystal_I2C lcd(0x3F, 16, 2); // or 0x27

float duration; // 儲存echoPin高電位時長
int cm;
float cm2; // 換算成英吋
float soundspeed=0.034;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //lcd初始化
  lcd.init();
  lcd.backlight();
}

void loop(){

  //超音波設置
  digitalWrite(trigPin,LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);

  //計算距離
  duration = pulseIn(echoPin, HIGH);
  cm = duration * soundspeed / 2;
  cm2 = cm / 2.54; // 轉換成英吋
  //顯示在序列阜
  Serial.print("distance:");
  Serial.print(cm);
  Serial.println("cm");
  delay(500);
  //顯示在lcd上
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distance:");
  lcd.setCursor(0, 1);
  lcd.print(round(cm));
  lcd.print(" cm");
  lcd.setCursor(8, 1);
  lcd.print(round(cm2));
  lcd.print(" in");
}
