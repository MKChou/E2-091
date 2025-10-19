#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int trigPin = 12;
int echoPin = 11;
int ledPin = 9;  // 設定LED的腳位
float c;

float duration;
float cm;
float soundspeed = 0.034;
int brightness;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop(){

  // 超音波設置
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // 計算距離
  duration = pulseIn(echoPin, HIGH);
  
  c = duration * soundspeed / 2;
  if(c<200 && c>0){
    cm=c;
  }

  // 控制LED
  if (cm<40){
    // 距離小於40cm時，物體越近亮度越大
    int value = round((40-cm)/40.0*255);
    analogWrite(ledPin, value);
  }
  else{
    // 距離大於40cm時，LED開始閃爍
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }

  // 顯示在序列埠
  Serial.print("Distance: ");
  Serial.print(cm);
  Serial.println(" cm");
  
  delay(200);
}
