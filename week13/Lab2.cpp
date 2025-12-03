#include "mbed.h"
#include "rtos.h"

DigitalOut led[3] = {LED1, LED2, LED3};
char c;

Thread m1;
Thread m2;
Thread m3;
Thread m4;

void mode1(){ //控制綠燈
    while(1){
         //等待信號 0x1。當接收到信號 0x1 時，線程繼續執行
        osSignalWait(0x1, osWaitForever);
        led[0] = 1;  //亮綠燈
        led[1] = 0;
        led[2] = 0;
    }
}
void mode2(){ //控制藍燈
    while(1){
         //等待信號 0x1。當接收到信號 0x1 時，線程繼續執行
        osSignalWait(0x1, osWaitForever);
        led[0] = 0;
        led[1] = 1;  //亮藍燈
        led[2] = 0;
    }
}
void mode3(){ //控制紅燈
    while(1){
         //等待信號 0x1。當接收到信號 0x1 時，線程繼續執行
        osSignalWait(0x1, osWaitForever);
        led[0] = 0;
        led[1] = 0;
        led[2] = 1;  //亮紅燈
    }
}
void mode4(){ //LED全亮
    while(1){
         //等待信號 0x1。當接收到信號 0x1 時，線程繼續執行
        osSignalWait(0x1, osWaitForever);
        led[0] = 1;  //全亮
        led[1] = 1;
        led[2] = 1;
    }
}

int main()
{
    led[0] = led[1] = led[2] = 0;  //初始化所有LED
    //啟動所有執行緒,控制對應函數
    m1.start(mode1);
    m2.start(mode2);
    m3.start(mode3);
    m4.start(mode4);
    Serial pc(USBTX, USBRX);  //創建 Serial 用於串列埠通信

    while (true) {
        c = pc.getc(); //從串列埠讀取字元,並儲存於變數中
        pc.putc(c);    //將字元回傳顯示在終端上
        pc.putc('\r');
        pc.putc('\n');
        // 根據讀取到的字符設置相應的線程信號
        if(c == 'g' || c == 'G'){
            osSignalSet(m1.get_id(), 0x1);  //發送信號給綠燈線程
        }
        else if(c == 'b' || c == 'B'){
            osSignalSet(m2.get_id(), 0x1);  //發送信號給藍燈線程
        }
        else if(c == 'r' || c == 'R'){
            osSignalSet(m3.get_id(), 0x1);  //發送信號給紅燈線程
        }
        else if(c == 'a' || c == 'A'){
            osSignalSet(m4.get_id(), 0x1);  //發送信號給全亮線程
        }
        else {
        printf("ERROR\r\n");  //若字元輸入錯誤則顯示"ERROR"
        }
    }
}
