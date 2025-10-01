#include <Keypad.h>  // 引用Keypad函式庫

#define ROWS 4   // 定義按鍵模組的列數
#define COLS 4   // 定義按鍵模組的行數

// Keypad定義
const byte rowPins[4] = {2, 3, 4, 5}; // Uno板的腳位
const byte colPins[4] = {6, 7, 8, 9}; // Uno板的腳位
// 定義Keypad每個鍵被按下後對應的符號
// hint: 在loop()會用這些符號來判斷使用者輸入什麼
const char keymap[ROWS][COLS] = {
  {'1', '2', '3', 'A'},  // A為加號
  {'4', '5', '6', 'B'},  // B為減號
  {'7', '8', '9', 'C'},  // C為乘號
  {'*', '0', '#', 'D'}   // D為除號，#為等於，*為清除
};

// 初始化一個Keypad object
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, ROWS, COLS);

// 把輸入的數字存成兩個string和一個獨立char
// e.g. 25 + 32
// usrInput[0]存`25`，usrInput[1]則存`32`，myOperator存`+`
String usrInput[2] = {""}; 
char myOperator = '?';
int idx = 0;

void setup(){
  // 設定Baud rate
  Serial.begin(9600);
}

void loop(){
  char key = myKeypad.getKey();
  // 輸入數字(0~9)
  if(key != NO_KEY && (key >= '0' && key <= '9')){ // 判斷key是否為數字
    // 把key存到對應idx的usrInput，並把key輸出至Serial Monitor
    usrInput[idx] += key;
    Serial.print(key);
  }
  // 輸入運算符號
  if(key != NO_KEY && (key == 'A' || key == 'B' || key == 'C' || key == 'D')){ // 判斷key是否為運算符號
    // 更新idx，並把key存到operator變數裡，最後將key輸出至Serial Monitor
    idx = 1;
    myOperator = key;
    if(key == 'A') Serial.print(" + ");
    else if(key == 'B') Serial.print(" - ");
    else if(key == 'C') Serial.print(" x ");
    else if(key == 'D') Serial.print(" / ");
  }
  // 輸入等於符號
  if(key == '#'){
    // 把key先輸出至Serial Monitor，再根據usrInput[0]、usrInput[1]和operator計算結果
    Serial.print(" = ");
    
    // 將字串轉換為數字進行計算
    float num1 = usrInput[0].toFloat();
    float num2 = usrInput[1].toFloat();
    float result = 0;
    
    // 根據運算符號進行計算
    if(myOperator == 'A') result = num1 + num2;      // 加法
    else if(myOperator == 'B') result = num1 - num2; // 減法
    else if(myOperator == 'C') result = num1 * num2; // 乘法
    else if(myOperator == 'D') result = num1 / num2; // 除法
    
    // 輸出結果，保留兩位小數
    Serial.println(result, 2);
    
    // 重新初始化變數
    usrInput[0] = "";
    usrInput[1] = "";
    myOperator = '?';
    idx = 0;
  }
  // 輸入重置符號
  if(key == '*'){
    // 把相關變數重新初始化，並印出Reset證明有初始化成功
    usrInput[0] = "";
    usrInput[1] = "";
    myOperator = '?';
    idx = 0;
    Serial.println("Reset");
  }
}
