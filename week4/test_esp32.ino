#include  <BluetoothSerial.h> //導入 Bluetooth 程式庫

BluetoothSerial BT; //宣告 BT 為藍芽通訊物件
char val;  //儲存接收到的資料變數
 
void setup() {
    Serial.begin(9600); //啟用 Serial port，baudrate 為 9600
    BT.begin("ESP32_7"); //設定藍芽名稱為 ESP32_組別號碼，並啟用藍芽
}

void loop() {
  //若收到「序列埠監控視窗」的資料，則送到藍牙終端
  if (Serial.available()) {
      val = Serial.read();
      BT.println(val);
  }
  
  //若收到藍牙終端的資料，則送到「序列埠監控視窗」
  if (BT.available()) {
      val = BT.read();
      Serial.println(val);
  }
}
