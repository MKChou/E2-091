#include <SPI.h>

const int SS_PIN = 10;  // Slave Select pin
int counter = 1;        // 計數器，從1開始
unsigned long previousMillis = 0;
const long interval = 1000;  // 1秒間隔

void setup() {
  Serial.begin(9600);
  
  // 初始化SPI為主設備
  SPI.begin();
  
  // 設置SS引腳為輸出
  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, HIGH);  // 默認拉高SS
  
  
}

void loop() {
  unsigned long currentMillis = millis();
  
  // 每隔1秒發送一次數據
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // 準備要發送的字串數據
    String dataToSend = String(counter) + "\r";  // 數字轉字串並加上結尾符號
    
    Serial.print("Sent: ");
    Serial.println(counter);
    
    // 開始SPI傳輸
    digitalWrite(SS_PIN, LOW);   // 選中從設備
    delay(1);  // 短暫延遲讓從設備準備好
    
    // 逐個位元組發送字串
    for (int i = 0; i < dataToSend.length(); i++) {
      SPI.transfer(dataToSend[i]);
      delay(1);  // 每個位元組間的小延遲
    }
    
    digitalWrite(SS_PIN, HIGH);  // 取消選中從設備
    
    // 遞增計數器，循環1-20
    counter++;
    if (counter > 20) {
      counter = 1;
      Serial.println("--- 重新開始循環 ---");
    }
    
    delay(10);  // 短暫延遲確保傳輸完成
  }
}
