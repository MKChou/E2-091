#include "mbed.h"

class LEDcontrol {
public:
    LEDcontrol(PinName pin, PinName pin1)
        : _pin(new DigitalOut(pin)),
          _pin1(new DigitalOut(pin1)),
          _firstActive(true),
          _state(false) {
        *_pin = 0;
        *_pin1 = 0;
    }

    ~LEDcontrol() {
        delete _pin;
        delete _pin1;
    }

    void change() {
        _firstActive = !_firstActive;
        *_pin = 0;
        *_pin1 = 0;
    }

    void flip() {
        _state = !_state;
        if (_firstActive) {
            *_pin = _state;
            *_pin1 = 0;
        } else {
            *_pin = 0;
            *_pin1 = _state;
        }
    }

private:
    DigitalOut *_pin;
    DigitalOut *_pin1;
    bool _firstActive;
    bool _state;
};

LEDcontrol led(LED1, LED2);
Timeout t;

void switch_led() {
    led.change();
}

int main() {
    t.attach(&switch_led, 3.0f);
    while (true) {
        led.flip();
        ThisThread::sleep_for(200);
    }
}

