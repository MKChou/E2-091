#include <SPI.h>
char buff [50];              // 接收資料的緩衝區
volatile byte indx;          // 緩衝區索引
volatile boolean process;    // 是否完成接收一筆資料的旗標

void setup() {
  Serial.begin (9600);
  pinMode(MISO, OUTPUT);      // MISO 腳位設定為輸出（Slave 往 Master 傳送資料用）
  SPCR |= _BV(SPE);           // 啟動 SPI 並設定為從屬裝置 (Slave) 模式
  indx = 0;                   // 將緩衝區索引歸零
  process = false;            // 初始狀態沒有資料需要處理
  SPI.attachInterrupt();      // 當 SPI 收到資料時觸發中斷
}

// SPI 接收中斷服務函式 (Slave 每次接收到 1 byte 都會進來這裡)
ISR (SPI_STC_vect){               
   byte c = SPDR;                 // 從 SPI 資料暫存器讀取 1 個位元組
   if (indx < sizeof buff) {      
      buff [indx++] = c;          // 將接收到的資料存入緩衝區
      if (c == '\r')              // 如果遇到結尾符號 '\r'，表示一筆資料結束
         process = true;          // 設定旗標，表示可以處理資料
   }
}

void loop() {
 if (process) {
    // 處理資料的程式碼寫在這裡
     Serial.print("收到資料: ");
      Serial.println(buff); // 把收到的字串印出來

      // *** 處理完畢後，必須重設索引和旗標，才能接收下一筆資料 ***
      indx = 0;          // 索引歸零
      memset(buff, 0, sizeof(buff)); // 清空緩衝區 (可選，但建議)
      process = false;   // 旗標設回 false
 }
}