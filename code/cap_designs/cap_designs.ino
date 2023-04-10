#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <FastLED.h>

#include "logos.h"

#define LED_PIN 9
#define N_LEDS 84
#define N_DESIGNS 6
#define ADDRESS 0x0
#define BRIGHTNESS 100

int design = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    design = EEPROM.read(ADDRESS);
    design = design % N_DESIGNS;
    EEPROM.write(ADDRESS, (design + 1) % N_DESIGNS);
    // design = 3;
    strip.begin();
    strip.setBrightness(BRIGHTNESS);
    strip.show();
    Serial.begin(9600);
}

void loop() {
    if (design == 0) {
        chase(strip.Color(255, 255, 255));  // White
    } else if (design == 1) {
        chase(strip.Color(255, 255, 0));  // Yellow
    } else if (design == 2) {
        chase(strip.Color(0, 0, 255));  // Blue
    } else if (design == 3) {
        gt_logo(strip.Color(255, 255, 0), strip.Color(0, 0, 255));
    } else if (design == 4) {
        chase_gt_logo(strip.Color(255, 255, 0), strip.Color(0, 0, 255));
    } else if (design == 5) {
        moving_alternating_colors(strip.Color(255, 255, 0), strip.Color(0, 0, 255), 1000);  // Yellow and Blue
    } 
}


// Design 5: Moving alternating colors in G and T
static void moving_alternating_colors(uint32_t c1, uint32_t c2, int duration) {
    for (uint16_t shift = 0; shift < 2; shift++) {
        strip.clear();
        for (uint16_t i = 0; i < sizeof(g_shape) / sizeof(g_shape[0]); i++) {
            strip.setPixelColor(g_shape[i], ((i + shift) % 2 == 0) ? c1 : c2);
        }
        for (uint16_t i = 0; i < sizeof(t_shape) / sizeof(t_shape[0]); i++) {
            strip.setPixelColor(t_shape[i], ((i + shift) % 2 == 0) ? c2 : c1);
        }
        strip.show();
        delay(duration);
    }
}


static void chase(uint32_t c) {
    for (uint16_t i = 6; i < N_LEDS + 6; i++) {
        strip.setPixelColor(i % N_LEDS, c);  // Draw new pixel
        strip.setPixelColor(i - 6, 0);       // Erase pixel a few steps back
        strip.show();
        delay(15);
    }
}

static void gt_logo(uint32_t c1, uint32_t c2) {
    strip.clear();
    for (uint16_t i = 0; i < sizeof(g_shape) / sizeof(g_shape[0]); i++) {
        int index = g_shape[i];

        strip.setPixelColor(index, c2);  // Draw new pixel
    }
    strip.show();
    delay(1000);

    strip.clear();
    for (uint16_t i = 0; i < sizeof(t_shape) / sizeof(t_shape[0]); i++) {
        int index = t_shape[i];
        strip.setPixelColor(index, c1);  // Draw new pixel
    }
    strip.show();

    delay(1000);
}

static void chase_gt_logo(uint32_t c1, uint32_t c2) {
    int i = 0;
    int j = 0;
    strip.clear();
    while (true) {
        i = i % (sizeof(g_shape) / sizeof(g_shape[0]));
        j = j % (sizeof(t_shape) / sizeof(t_shape[0]));
        int i_index = g_rev[i];
        int j_index = t_shape[j];

        if (j - 15 < 0) {
            j += (sizeof(t_shape) / sizeof(t_shape[0]));
        }
        strip.setPixelColor(j_index, c1);         // Draw new pixel
        strip.setPixelColor(t_shape[j - 15], 0);  // Erase pixel a few steps back

        if (i - 35 < 0) {
            i += (sizeof(g_shape) / sizeof(g_shape[0]));
        }
        strip.setPixelColor(i_index, c2);       // Draw new pixel
        strip.setPixelColor(g_rev[i - 35], 0);  // Erase pixel a few steps back

        strip.show();
        delay(15);
        i--;
        j++;
    }
}
