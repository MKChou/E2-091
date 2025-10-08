#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <String.h>

// 初始化 I2C LCD，位址為 0x27，大小為 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int MAX_STUDENTS = 20;    // 最多儲存 20 筆學生資料
String studentIDs[MAX_STUDENTS];    // 儲存學號的陣列
String studentNames[MAX_STUDENTS];  // 儲存姓名的陣列
int studentCount = 0;               // 目前已儲存的學生數量

bool addMode = false;       // 是否進入新增模式
bool waitingName = false;   // 在新增模式中，是否正在等待輸入姓名
String tempID = "";         // 暫存輸入的學號

void setup() {
  Serial.begin(9600); // 啟動序列埠
  lcd.init();         // 初始化 LCD
  lcd.backlight();    // 開啟 LCD 背光
  
  // 程式開始時，進入預設的查詢模式
  showQueryPrompt();
}

void loop() {
  // 檢查序列埠是否有傳來資料
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n'); // 讀取一行資料 (直到換行符)
    input.trim(); // 去除字串前後的空白字元
    lcd.clear();

    // ---- 模式切換指令 ----
    // 如果輸入 "add"，則切換到新增模式
    if (input.equalsIgnoreCase("add")) {
      addMode = true;
      waitingName = false;
      tempID = "";
      showAddPromptID(); // 顯示新增模式提示 (輸入ID)
      return; // 結束本次 loop，等待下一次輸入
    }
    // 如果輸入 "query"，則切換回查詢模式
    else if (input.equalsIgnoreCase("query")) {
      addMode = false;
      showQueryPrompt(); // 顯示查詢模式提示
      return; // 結束本次 loop，等待下一次輸入
    }

    // ---- 根據目前模式處理輸入 ----
    if (!addMode) {
      // ========= 查詢模式 =========
      bool found = false;
      for (int i = 0; i < studentCount; i++) {
        if (studentIDs[i].equals(input)) {
          // 找到了學號
          lcd.setCursor(0, 0);
          lcd.print("ID: " + studentIDs[i]);
          lcd.setCursor(0, 1);
          lcd.print("Name: " + studentNames[i]);
          found = true;
          break; // 找到後就跳出迴圈
        }
      }
      
      if (!found) {
        // 遍歷完所有資料都沒找到
        lcd.setCursor(0, 0);
        lcd.print(input);
        lcd.setCursor(0, 1);
        lcd.print("Not Found");
      }
      // 短暫延遲後回到查詢提示畫面，方便下一次查詢
      delay(3000);
      showQueryPrompt();

    } else {
      // ========= 新增模式 =========
      if (!waitingName) {
        // --- 步驟 1: 接收學號 ---
        tempID = input; // 將輸入的內容暫存為學號
        waitingName = true; // 下一步要等待輸入姓名
        showAddPromptName(); // 顯示提示 (輸入姓名)
        
      } else {
        // --- 步驟 2: 接收姓名並儲存 ---
        if (studentCount < MAX_STUDENTS) {
          studentIDs[studentCount] = tempID;
          studentNames[studentCount] = input; // input 是剛輸入的姓名
          
          lcd.setCursor(0, 0);
          lcd.print("Added success");
          lcd.setCursor(0, 1);
          lcd.print(tempID);
          
          studentCount++; // 學生總數加 1
          
        } else {
          // 儲存空間已滿
          lcd.setCursor(0, 0);
          lcd.print("Database Full!");
        }
        
        // 新增完成後，自動切換回查詢模式
        delay(2000);
        addMode = false;
        waitingName = false;
        showQueryPrompt();
      }
    }
  }
}

// LCD 顯示查詢模式提示
void showQueryPrompt() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Query Mode");
  lcd.setCursor(0, 1);
  lcd.print("Input ID:");
}

// LCD 顯示新增模式等待ID
void showAddPromptID() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Add Mode");
  lcd.setCursor(0, 1);
  lcd.print("Input ID:");
}

// LCD 顯示新增模式等待姓名
void showAddPromptName() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Add Mode");
  lcd.setCursor(0, 1);
  lcd.print("Input Name:");
}