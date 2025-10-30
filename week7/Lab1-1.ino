#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// I2C LCD 設定，位址 0x27，16欄 2列
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- 請在此處修改您的 Wi-Fi 資訊 ---
const char* ssid = "pekka";       // Wi-Fi 網路名稱
const char* password = "pekka1208"; // Wi-Fi 密碼

// NTP 伺服器設定
const char *ntpServer = "pool.ntp.org";
int section = 8; // 預設時區 UTC+8
long gmtOffset_sec = section * 3600;
const int daylightOffset_sec = 0; // 日光節約時間偏移，台灣通常為 0

// 按鈕腳位設定 (ESP32 開發板上的 BOOT 按鈕)
const int buttonPin = 0;
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

// 用於每秒更新時間顯示的計時器
unsigned long previousMillis = 0;
const long interval = 1000;

// --- 函式宣告 ---
void syncTime();
void handleButtonPress();
void printFullTimeInfo();
void displayTime();


void setup() {
    Serial.begin(115200);

    // 【修正】初始化 LCD 並開啟背光
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("NTP Clock");

    // 設定按鈕腳位為輸入，並啟用內部上拉電阻
    pinMode(buttonPin, INPUT_PULLUP);

    // 進行首次時間同步
    syncTime();
}

void loop() {
    // 檢查按鈕是否被按下 (含防彈跳處理)
    handleButtonPress();


    // 【新增】每秒更新 LCD 上的時間顯示，讓時鐘動起來
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        displayTime();
    }
}

/**
 * @brief 連接到 Wi-Fi 並從 NTP 伺服器同步時間
 */
void syncTime() {
    // 啟動 Wi-Fi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connecting to");
    lcd.setCursor(0, 1);
    lcd.print(ssid);
    Serial.print("Connecting to Wi-Fi...");

    // 等待 Wi-Fi 連接成功
    int wait_count = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        wait_count++;
        // 新增超時判斷，避免無限等待
        if (wait_count > 20) {
            Serial.println("\nWi-Fi connection failed.");
            lcd.clear();
            lcd.print("Wi-Fi Failed");
            return;
        }
    }
    Serial.println("\nWi-Fi Connected.");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wi-Fi Connected");
    lcd.setCursor(0, 1);
    lcd.print("Syncing time...");
    
    // 根據當前時區設定本地時間來源
    gmtOffset_sec = section * 3600;
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    // 取得並完整顯示一次時間
    printFullTimeInfo();

    // 為了節省電力，同步完成後斷開 Wi-Fi (這是根據你原程式碼的意圖)
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    Serial.println("Wi-Fi disconnected.");
}

/**
 * @brief 處理按鈕按下事件，用於切換時區
 */
void handleButtonPress() {
    int reading = digitalRead(buttonPin);

    // // 防彈跳 (debounce) 處理
    // if (reading != lastButtonState) {
    //     lastDebounceTime = millis();
    // }

    // if ((millis() - lastDebounceTime) > debounceDelay) {
        // 當按鈕狀態穩定且被按下時 (從 HIGH 變到 LOW)
        // if (reading == LOW && lastButtonState == HIGH) {
          if(reading==LOW){
            section++;
            // 時區範圍從 -12 到 +14
            if (section > 14) {
                section = -12;
            }
            Serial.printf("Timezone changed to UTC%+d\n", section);
            
            // 【優化】按下按鈕後，只更新時區設定，不重新連線 Wi-Fi
            gmtOffset_sec = section * 3600;
            configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
            
            // 立即使用新的時區設定來更新顯示
            printFullTimeInfo();
            delay(300);
        }
    // }
    // lastButtonState = reading;
}


/**
 * @brief 【修正】首次同步或變更時區後，格式化並完整更新 LCD 顯示
 */
void printFullTimeInfo() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        lcd.clear();
        lcd.print("Time sync fail");
        return;
    }
    
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

    // 使用 strftime 將日期格式化成 "YYYY-MM-DD" 字串
    char dateLine[17];
    strftime(dateLine, sizeof(dateLine), "%Y-%m-%d", &timeinfo);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(dateLine); // 顯示日期
    
    // 立即呼叫 displayTime 來顯示時間部分
    displayTime();
}

/**
 * @brief 【新增】只更新 LCD 上的時間部分 (每秒執行一次)
 * 它直接從 ESP32 內部的 RTC 獲取時間，不需要 Wi-Fi 連線
 */
void displayTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return; // 如果獲取失敗，則不更新
    }
    
    // 使用 strftime 和 sprintf 將時間格式化成 "HH:MM:SS UTC+X" 字串
    char timeLine[17]; 
    char timeStr[9];
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);
    sprintf(timeLine, "%s UTC%+d", timeStr, section);

    lcd.setCursor(0, 1);
    lcd.print(timeLine); // 顯示時間與時區
}