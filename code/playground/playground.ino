#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
#define N_LEDS 60
#define button 3
int buttonState = 0;  // variable for reading the pushbutton status
int counter = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, LED_PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
    strip.begin();
    // strip.setBrightness(120);
    strip.show();
    pinMode(button, INPUT_PULLUP);
    attachInterrupt(3, pin_ISR, CHANGE);
    counter = 0;
    Serial.begin(9600);
    randomSeed(analogRead(12));
}

void loop() {
    int x = random(3);
    while (counter == x) {
        x = random(3);
    }
    counter = x;
    if (counter == 0) {
        chase(strip.Color(255, 0, 0));  // Red
    } else if (counter == 1) {
        chase(strip.Color(0, 255, 0));  // Green
    } else {
        chase(strip.Color(0, 0, 255));  // Blue
    }
}

static void chase(uint32_t c) {
    for (uint16_t i = 0; i < N_LEDS + 4; i++) {
        strip.setPixelColor(i, c);      // Draw new pixel
        strip.setPixelColor(i - 4, 0);  // Erase pixel a few steps back
        strip.show();
        delay(10);
    }
    for (uint16_t i = N_LEDS - 4; i > 0; i--) {
        strip.setPixelColor(i, c);      // Draw new pixel
        strip.setPixelColor(i + 4, 0);  // Erase pixel a few steps back
        strip.show();
        delay(10);
    }
    strip.clear();
}

void pin_ISR() {
    if (buttonState == digitalRead(button)) {
        return;
    }
    buttonState = digitalRead(button);
    counter++;
    counter = counter % 3;
    buttonState = digitalRead(button);
}