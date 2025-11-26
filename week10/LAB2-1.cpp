#include "mbed.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalIn button(USER_BUTTON);

int main()
{
    button.mode(PullNone);       // 依照要求使用浮接模式

    DigitalOut* leds[] = { &led1, &led2, &led3 };
    const int ledCount = sizeof(leds) / sizeof(leds[0]);

    int index = 0;
    int direction = 1;           // 1 表示順時針，-1 表示逆時針
    int previousButton = button.read();

    while (1) {
        // 先全暗，只亮當前索引
        led1 = 0;
        led2 = 0;
        led3 = 0;
        *(leds[index]) = 1;
        wait(0.2);

        int currentButton = button.read();

        if (currentButton == 1) {
            // 按下時維持當前 LED，不更新索引
            previousButton = currentButton;
            continue;
        }

        // 放開的瞬間才切換方向
        if (previousButton == 1 && currentButton == 0) {
            direction = -direction;
        }
        previousButton = currentButton;

        index = (index + direction + ledCount) % ledCount;
    }
}