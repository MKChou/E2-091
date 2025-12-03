#include "mbed.h"
#include "ultrasonic.h"
#include "rtos.h"

// 設置顯示控制和數據線的腳位
DigitalOut com[2] = {D0, D1}; // 兩位數七段需要兩個控制腳位
DigitalOut seg[8] = {D4, D5, D6, D7, D8, D9, D10, D11}; // A, B, C, D, E, F, G, DP
DigitalOut led[3] = {LED1, LED2, LED3}; // LED 指示燈

int disp[2] = {0, 0}; // 兩位數顯示
int d; // 儲存距離
int table[11][8] = {
    {1, 1, 1, 1, 1, 1, 0, 0},  // 0: A,B,C,D,E,F亮
    {0, 1, 1, 0, 0, 0, 0, 0},  // 1: B,C亮
    {1, 1, 0, 1, 1, 0, 1, 0},  // 2: A,B,D,E,G亮
    {1, 1, 1, 1, 0, 0, 1, 0},  // 3: A,B,C,D,G亮
    {0, 1, 1, 0, 0, 1, 1, 0},  // 4: B,C,F,G亮
    {1, 0, 1, 1, 0, 1, 1, 0},  // 5: A,C,D,F,G亮
    {1, 0, 1, 1, 1, 1, 1, 0},  // 6: A,C,D,E,F,G亮
    {1, 1, 1, 0, 0, 0, 0, 0},  // 7: A,B,C亮
    {1, 1, 1, 1, 1, 1, 1, 0},  // 8: A,B,C,D,E,F,G全亮
    {1, 1, 1, 1, 0, 1, 1, 0},  // 9: A,B,C,D,F,G亮
    {0, 0, 0, 0, 0, 0, 0, 0}   // 10: 全灭（用于不显示）
};  
Thread m1;
Thread m2;
Thread m3;
Thread m4;
Thread m5;
// 超音波感測距離回調函數
void dist(int distance)
{
    d = distance / 10; // 將距離從毫米轉換為公分
    printf("Distance changed to %dcm\r\n", d);   
}

// 將距離數字分解為兩位數
void packing(int num)
{   
    // 根據數字是否小於10來設置顯示數字
    if (num < 10) {
        disp[0] = -1; // 設置為 -1，表示不顯示十位數字的0
        disp[1] = num; // 個位數顯示數字
    } else {
        disp[0] = (num / 10) % 10; // 十位數
        disp[1] = num % 10;        // 個位數
    }
}

// 映射數字到顯示器段碼
void mapping(int disp)
{
    // 如果disp為-1，表示不顯示（全灭）
    int index = (disp == -1) ? 10 : disp;
    for(int i = 0; i < 8; i++) {
        seg[i] = table[index][i];
    }
}

// 初始化超音波感測器
ultrasonic mu(D2, D3, .1, 1, &dist);    // 設置觸發引腳和回波引腳

// LED 模式函數
void mode1() { //全亮
    while(1){
        osSignalWait(0x1, osWaitForever);
        led[0] = 1;  // LED1 (綠燈)
        led[1] = 1;  // LED2 (藍燈)
        led[2] = 1;  // LED3 (紅燈)
    }
}
void mode2() {  //亮紅燈
    while(1){
        osSignalWait(0x1, osWaitForever);
        led[0] = 0;
        led[1] = 0;
        led[2] = 1;  // LED3 (紅燈)
    }
}
void mode3() { //亮藍燈
    while(1){
        osSignalWait(0x1, osWaitForever);
        led[0] = 0;
        led[1] = 1;  // LED2 (藍燈)
        led[2] = 0;
    }
}
void mode4() { //亮綠燈
    while(1){
        osSignalWait(0x1, osWaitForever);
        led[0] = 1;  // LED1 (綠燈)
        led[1] = 0;
        led[2] = 0;
    }
} 
void mode5() { //不亮燈
    while(1){
        osSignalWait(0x1, osWaitForever);
        led[0] = 0;
        led[1] = 0;
        led[2] = 0;
    }
}
// 主函數
int main() {   
    mu.startUpdates();  //開始測量距離
    //啟動所有執行緒,控制對應函數
    m1.start(mode1);
    m2.start(mode2);
    m3.start(mode3);
    m4.start(mode4);
    m5.start(mode5);
    while(1) {   
        mu.checkDistance(); // 檢查距離更新
        packing(d); // 將距離分解成數字
        // 顯示兩位數字
        for(int i = 0; i < 2; i++) {
            com[i] = 0;       
            mapping(disp[i]);
            wait(0.001); // 每位顯示0.001秒           
            com[i] = 1;
        }
        // 根據距離設置 LED
        if(d <= 10) {
            osSignalSet(m1.get_id(), 0x1);  // 距離 <= 10 cm: LED全亮
        }
        else if(d <= 15) {
            osSignalSet(m2.get_id(), 0x1);  // 距離 <= 15 cm: 亮紅燈
        }
        else if(d <= 20) {
            osSignalSet(m3.get_id(), 0x1);  // 距離 <= 20 cm: 亮藍燈
        }
        else if(d <= 25) {
            osSignalSet(m4.get_id(), 0x1);  // 距離 <= 25 cm: 亮綠燈
        }
        else {
            osSignalSet(m5.get_id(), 0x1);  // 距離 > 25 cm: LED不亮
        } 
    }
}
