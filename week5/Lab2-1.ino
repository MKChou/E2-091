#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();
  lcd.clear();
  lcd.noBacklight();

  // 開機閃爍三次
  for(int i = 0; i < 3; i++) {
    lcd.backlight();  // 開啟背光
    delay(250);
    lcd.noBacklight();// 關閉背光
    delay(250);
  }


  lcd.backlight();
}

void loop() {
  String text = "Arduino LCD I2C Demo!"; // 想要跑馬燈的文字
  int textLength = text.length();
  
  // 每次顯示前先清空 LCD
  lcd.clear();
  
  lcd.print(text);
  // delay(1000);
    for(int i=0;i<16;i++)
  {
   lcd.scrollDisplayRight();
   
  }
  for(int i=0;i<2*textLength;i++)
  {
   lcd.scrollDisplayLeft();
   delay(300);
  }


}