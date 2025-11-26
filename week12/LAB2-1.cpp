#include "mbed.h"

AnalogOut breathled(PA_4);
Ticker breath_ticker;
volatile float brightness = 0.0f;
volatile int direction = 1;
volatile bool update_needed = false;

void breath_step() {
    // 每次ticker呼叫時調整亮度，2秒完成一次漸亮或漸暗
    brightness += 0.01f * static_cast<float>(direction);

    if (brightness <= 0.0f) {
        brightness = 0.0f;
        direction = 1;
    } else if (brightness >= 1.0f) {
        brightness = 1.0f;
        direction = -1;
    }

    update_needed = true;
}

int main() {
    // 0.02秒觸發一次breath_step，2秒內可完成0→1或1→0的漸變
    breath_ticker.attach(&breath_step, 0.02f);

    while (true) {
        if (update_needed) {
            update_needed = false;
            breathled.write(brightness);
        }

        ThisThread::sleep_for(5);
    }
}