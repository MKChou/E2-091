// 2-1

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

void setup() {
  Serial.begin(115200); // 設置串口通信的波特率為 115200
  
  // 初始化 MPU6050 感測器並檢查連接
  Serial.println("正在初始化 MPU6050...");
  if (accelgyro.testConnection()) {
    Serial.println("MPU6050 連接成功！");
  } else {
    Serial.println("MPU6050 連接失敗！請檢查接線。");
    while(1); // 如果連接失敗，停止程式
  }
  
  accelgyro.initialize(); // 初始化 MPU6050 感測器

  // 設定加速度計的範圍為 8g。此時範圍為 -16g 到 16g，每 g 對應 4096 單位
  accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_8);
  
  Serial.println("開始監測步數...");
  Serial.println("當前閾值: " + String(thres) + "g");
  Serial.println("---");
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


  // 輸出實時數據（可選，用於調試）
  // 取消下面的註解可以看到實時加速度值
  /*
  Serial.print("X: "); Serial.print(accelX, 2);
  Serial.print("  Y: "); Serial.print(accelY, 2);
  Serial.print("  Z: "); Serial.print(accelZ, 2);
  Serial.print("  | 角度 X: "); Serial.print(angleX, 1);
  Serial.print("  Y: "); Serial.print(angleY, 1);
  Serial.print("  Z: "); Serial.println(angleZ, 1);
  */

  // 檢測步數：當 Z 軸加速度超過閾值時計數
  if(accelZ > thres ){
    delay(1000); // 防止重複計數
    count++;
    Serial.print("步數: ");
    Serial.print(count);
    Serial.print("  (Z軸加速度: ");
    Serial.print(accelZ, 2);
    Serial.println("g)");
  }

  delay(10); // 等待 10 毫秒
}