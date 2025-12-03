#include"mbed.h"
#include"rtos.h"
#include <cstdlib>
#include <ctime>

DigitalIn but(USER_BUTTON);
DigitalOut led[6] = {D8, D9, D10, D11, D12, D13};
int j = 0; //追蹤led的位置
int i = 0; //按鈕模式記數
Mutex modeMutex; // 用於保護模式變數的互斥鎖
Mutex ledMutex;  // 用於保護LED操作的互斥鎖

Thread m1;
Thread m2;
Thread m3;
Thread m4;

//模式一: 單顆跑馬燈 (從左到右)
void mode1(){
    while (1) {
        modeMutex.lock();
        int currentMode = i;
        modeMutex.unlock();
        
        if (currentMode == 0) {
            ledMutex.lock();
            // 關閉所有LED
            for (int k = 0; k < 6; k++) {
                led[k] = 0;
            }
            // 點亮當前位置的LED
            led[j] = 1;
            // 更新位置，從上次中斷位置繼續
            j = (j + 1) % 6;
            ledMutex.unlock();
            
            ThisThread::sleep_for(500);
        } else {
            ThisThread::sleep_for(100); // 非當前模式時短暫休眠
        }
    }
}

//模式二:奇數跑馬燈 (index 0, 2, 4)
void mode2(){
    int oddPositions[3] = {0, 2, 4}; // 奇數顆LED對應的index
    static int oddIndex = 0; // 追蹤奇數LED陣列的位置（使用static保持狀態）
    static bool initialized = false; // 標記是否已初始化位置
    
    while (1) {
        modeMutex.lock();
        int currentMode = i;
        int currentJ = j; // 讀取當前j值
        modeMutex.unlock();
        
        if (currentMode == 1) {
            // 如果剛切換到模式2，需要從上次j位置找到對應的奇數位置
            if (!initialized) {
                // 找到j對應的奇數位置索引
                for (int k = 0; k < 3; k++) {
                    if (oddPositions[k] >= currentJ) {
                        oddIndex = k;
                        break;
                    }
                }
                initialized = true;
            }
            
            ledMutex.lock();
            // 關閉所有LED
            for (int k = 0; k < 6; k++) {
                led[k] = 0;
            }
            // 點亮當前奇數位置的LED
            led[oddPositions[oddIndex]] = 1;
            // 更新奇數位置索引
            oddIndex = (oddIndex + 1) % 3;
            ledMutex.unlock();
            
            ThisThread::sleep_for(500);
        } else {
            initialized = false; // 離開模式時重置標記
            ThisThread::sleep_for(100);
        }
    }
}

//模式三: 偶數跑馬燈 (index 1, 3, 5)
void mode3(){
    int evenPositions[3] = {1, 3, 5}; // 偶數顆LED對應的index
    static int evenIndex = 0; // 追蹤偶數LED陣列的位置（使用static保持狀態）
    static bool initialized = false; // 標記是否已初始化位置
    
    while (1) {
        modeMutex.lock();
        int currentMode = i;
        int currentJ = j; // 讀取當前j值
        modeMutex.unlock();
        
        if (currentMode == 2) {
            // 如果剛切換到模式3，需要從上次j位置找到對應的偶數位置
            if (!initialized) {
                // 找到j對應的偶數位置索引
                for (int k = 0; k < 3; k++) {
                    if (evenPositions[k] >= currentJ) {
                        evenIndex = k;
                        break;
                    }
                }
                initialized = true;
            }
            
            ledMutex.lock();
            // 關閉所有LED
            for (int k = 0; k < 6; k++) {
                led[k] = 0;
            }
            // 點亮當前偶數位置的LED
            led[evenPositions[evenIndex]] = 1;
            // 更新偶數位置索引
            evenIndex = (evenIndex + 1) % 3;
            ledMutex.unlock();
            
            ThisThread::sleep_for(500);
        } else {
            initialized = false; // 離開模式時重置標記
            ThisThread::sleep_for(100);
        }
    }
}

//模式四:隨機跑馬燈
void mode4(){
    // 初始化隨機數生成器
    srand(time(NULL));
    int lastRandom = 0;
    
    while (1) {
        modeMutex.lock();
        int currentMode = i;
        modeMutex.unlock();
        
        if (currentMode == 3) {
            ledMutex.lock();
            // 關閉所有LED
            for (int k = 0; k < 6; k++) {
                led[k] = 0;
            }
            // 隨機選擇一個LED點亮
            int randomLed;
            do {
                randomLed = rand() % 6;
            } while (randomLed == lastRandom && 6 > 1); // 避免連續兩次相同（如果有多個LED）
            
            led[randomLed] = 1;
            lastRandom = randomLed;
            ledMutex.unlock();
            
            ThisThread::sleep_for(500);
        } else {
            ThisThread::sleep_for(100);
        }
    }
}

int main() {
    // 初始化：關閉所有LED
    for (int k = 0; k < 6; k++) {
        led[k] = 0;
    }
    
    // 啟動所有執行緒
    m1.start(mode1);
    m2.start(mode2);
    m3.start(mode3);
    m4.start(mode4);
    
    int lastButtonState = 0;
    
    while (1) {
        int buttonState = but.read();
        
        // 檢測按鈕按下（上升沿檢測）
        if (buttonState == 1 && lastButtonState == 0) {
            ledMutex.lock();
            // 切換時先清空所有led
            for (int k = 0; k < 6; k++) {
                led[k] = 0;
            }
            ledMutex.unlock();
            
            modeMutex.lock();
            i++; // 增加模式計數器
            if (i == 4) {
                i = 0; // 如果超過最大模式數，重置計數器
            }
            modeMutex.unlock();
            
            printf("當前模式 : %d\r\n", i);
            
            // 防抖動延遲
            ThisThread::sleep_for(200);
        }
        
        lastButtonState = buttonState;
        ThisThread::sleep_for(50); // 主循環短暫休眠
    }
}
