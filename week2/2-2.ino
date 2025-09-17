const int button = 2;
const int led = 13;
bool led_state = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
bool lastButtonState = HIGH;
bool buttonState;

void setup() {
pinMode(button, INPUT_PULLUP);
pinMode(led, OUTPUT);
digitalWrite(led, led_state);
}

void loop() {
int reading = digitalRead(button);

if (reading != lastButtonState) {
  lastDebounceTime = millis();
}

if ((millis() - lastDebounceTime) > debounceDelay) {
  if (reading != buttonState) {
    if (reading == LOW && buttonState == HIGH) {
      led_state = !led_state;
      digitalWrite(led, led_state);
    }
    buttonState = reading;
  }
}

lastButtonState = reading;
}
