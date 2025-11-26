#include "mbed.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

int main()
{
    DigitalOut* leds[] = { &led1, &led2, &led3 };

    while (1) {
        for (DigitalOut* led : leds) {
            led1 = 0;
            led2 = 0;
            led3 = 0;

            *led = 1;           // 單向依序點亮
            wait(0.2);          // 調整間隔可改變速度
        }
    }
}