// 2-2 bonus

#include <Wire.h>        // 引入 Wire 函式庫，用於 I2C 通信
#include <I2Cdev.h>      // 引入 I2Cdev 函式庫，用於簡化 I2C 設備的操作
#include <MPU6050.h>     // 引入 MPU6050 函式庫，用於操作 MPU6050 傾斜感測器
#include <math.h>        // 引入 math 函式庫，用於數學計算，如角度轉換

//友善提醒:接線要注意，如果確定程式碼沒問題，極度建議把杜邦線換掉試試看，晃動過程中很容易訊號不穩

// MPU-6050設定
MPU6050 accelgyro; // 創建一個 MPU6050 類別的實例，預設 I2C 位址為 0x68
int16_t ax, ay, az, gx, gy, gz; // 定義變數來存儲加速度計和陀螺儀的數據

// 全域變數設定
 // 儲存上一次計算的 X 軸角度
 // 儲存上一次計算的 Y 軸角度
 // 儲存上一次計算的 Z 軸角度
 // 設定 xy 平面上的角度變化閾值 (度)
 // 設定 z 軸上的角度變化閾值 (度)
int stepCount = 0;          // 記錄步數的計數器
bool stepDetected = false;  // 標記是否已檢測到步數
  int count=0;
  float thres=1.6;



  const int SEG_data[10][8] = {{1, 1, 1, 1, 1, 1, 0, 0},  //"0"  //資料線
                      {0, 1, 1, 0, 0, 0, 0, 0},  //"1"
                      {1, 1, 0, 1, 1, 0, 1, 0},  //"2"
                      {1, 1, 1, 1, 0, 0, 1, 0},  //"3"
                      {0, 1, 1, 0, 0, 1, 1, 0},  //"4"
                      {1, 0, 1, 1, 0, 1, 1, 0},  //"5"
                      {1, 0, 1, 1, 1, 1, 1, 0},  //"6"
                      {1, 1, 1, 0, 0, 0, 0, 0},  //"7"
                      {1, 1, 1, 1, 1, 1, 1, 0},  //"8"
                      {1, 1, 1, 0, 0, 1, 1, 0}};  //"9"


void initdisp(){
    for(int i=3; i<=9; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);  //共陽極
  }
}


void disp(int pin,int num){
  for(int i=3;i<=9;i++){
    if(SEG_data[num][i-3]==1){
      digitalWrite(i,HIGH);
    }else{
      digitalWrite(i,LOW);
    }
  }
}

void setup() {
  Serial.begin(115200); // 設置串口通信的波特率為 115200
  accelgyro.initialize(); // 初始化 MPU6050 感測器

  // 設定加速度計的範圍為 8g。此時範圍為 -16g 到 16g，每 g 對應 4096 單位
  accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_8);

  initdisp();
  pinMode(11,OUTPUT);
}

void loop() {
  // 獲取 MPU6050 感測器的六個數據：三個加速度和三個角速度
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // 計算加速度大小並轉換為 g 值
  // 設定為 8g 時，每 g 對應 4096 單位，因此需要除以 4096
  float accelX = ax / 4096.0; // X 軸加速度 (g)
  float accelY = ay / 4096.0; // Y 軸加速度 (g)
  float accelZ = az / 4096.0; // Z 軸加速度 (g)

  // 使用 arctangent 函數計算角度值
  // angleX: X 軸的角度，通過 Y 和 Z 軸的加速度計算
  // angleY: Y 軸的角度，通過 X 和 Z 軸的加速度計算
  // angleZ: Z 軸的角度，通過 X 和 Y 軸的加速度計算
  float angleX = atan2(accelX, sqrt(accelY * accelY + accelZ * accelZ)) * 180 / PI;
  float angleY = atan2(accelY, sqrt(accelX * accelX + accelZ * accelZ)) * 180 / PI;
  float angleZ = atan2(sqrt(accelX * accelX + accelY * accelY), accelZ) * 180 / PI;

  if(count<10){
    disp(11,count);
  }
  if(accelZ>thres ){
    delay(1000);
    count++;
 
    Serial.print("walk");
    Serial.print(count);
    Serial.print("\n");
  }


  delay(10); // 等待 100 毫秒，這個延遲可以調整以適應實際需要
}