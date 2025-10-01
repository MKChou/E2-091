#include <BluetoothSerial.h> // 引入Bluetooth函式庫

BluetoothSerial BT; // 宣告一個藍牙通訊物件，名字為BT
String msg = "";    // 儲存透過藍牙接收到的訊息
#define LED_PIN 2   // ESP32上面小顆藍色LED的腳位

#define GROUP_ID 7  // 請將問號改成你們的組別數字 

void setup(){
  // 定義Baud rate
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  // 設定藍牙名稱為ESP32_?(根據自己的組別id)，並啟用藍牙
  BT.begin("ESP32_"+String(GROUP_ID));
}




void loop(){
  if(BT.available()){
    msg = BT.readString();
    // 判斷msg是否為hi，若是則在手機的bluetooth terminal印出句子"Group id is ???"
    BT.println("Our group id is 7");


    if(msg == "hi"){
      BT.println("Our group id is 7");
    }
    // 判斷msg是否為on，若是則點亮NodeMCU32s上面的藍色LED燈
    // 並在手機的bluetooth terminal印出句子"LED is turned on"
    else if(msg == "on"){
      digitalWrite(LED_PIN, HIGH);
      BT.println("LED is turned on");
    }
    // 判斷msg是否為off，若是則熄滅NodeMCU32s上面的藍色LED燈
    // 並在手機的bluetooth terminal印出句子"LED is turned off"
    else if(msg == "off"){
      digitalWrite(LED_PIN, LOW);
      BT.println("LED is turned off");
    }
    // 判斷msg是否為blink，若是則讓NodeMCU32s上面的藍色LED燈閃爍5次(亮暗間隔1s)
    // 並在手機的bluetooth terminal印出句子"LED is blinking"
    else if(msg == "blink"){
      BT.println("LED is blinking");
      for(int i = 0; i < 5; i++){
        digitalWrite(LED_PIN, HIGH);
        delay(1000);
        digitalWrite(LED_PIN, LOW);
        delay(1000);
      }
    }
    // 若msg都不是上面的字串，在手機的bluetooth terminal印出"Sorry :("
    else {
      BT.println("Sorry :(");
    }
  }
    // ===== 電腦 Serial Monitor -> ESP32 -> 手機 (聊天) =====
    if(Serial.available()){
      String input = Serial.readString(); 
      // 移除多餘換行
      input.trim();
      // 傳給手機
      BT.println("PC : " + input);
      // 自己電腦端確認
      Serial.println("Sent to phone: PC : " + input);
    }
}
