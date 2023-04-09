#include <Adafruit_NeoPixel.h>

class Strip {
   public:
    uint8_t effect;
    uint8_t effects;
    uint16_t effStep;
    unsigned long effStart;
    Adafruit_NeoPixel strip;
    Strip(uint16_t leds, uint8_t pin, uint8_t toteffects, uint16_t striptype) : strip(leds, pin, striptype) {
        effect = -1;
        effects = toteffects;
        Reset();
    }
    void Reset() {
        effStep = 0;
        effect = (effect + 1) % effects;
        effStart = millis();
    }
};

struct Loop {
    uint8_t currentChild;
    uint8_t childs;
    bool timeBased;
    uint16_t cycles;
    uint16_t currentTime;
    Loop(uint8_t totchilds, bool timebased, uint16_t tottime) {
        currentTime = 0;
        currentChild = 0;
        childs = totchilds;
        timeBased = timebased;
        cycles = tottime;
    }
};

Strip strip_0(84, 9, 84, NEO_GRB + NEO_KHZ800);
struct Loop strip0loop0(1, false, 1);

//[GLOBAL_VARIABLES]

void setup() {
    strip_0.strip.begin();
    strip_0.strip.setBrightness(100);
}

void loop() { strips_loop(); }

void strips_loop() {
    if (strip0_loop0() & 0x01) strip_0.strip.show();
}

uint8_t strip0_loop0() {
    uint8_t ret = 0x00;
    switch (strip0loop0.currentChild) {
        case 0:
            ret = strip0_loop0_eff0();
            break;
    }
    if (ret & 0x02) {
        ret &= 0xfd;
        if (strip0loop0.currentChild + 1 >= strip0loop0.childs) {
            strip0loop0.currentChild = 0;
            if (++strip0loop0.currentTime >= strip0loop0.cycles) {
                strip0loop0.currentTime = 0;
                ret |= 0x02;
            }
        } else {
            strip0loop0.currentChild++;
        }
    };
    return ret;
}

uint8_t strip0_loop0_eff0() {
    // Strip ID: 0 - Effect: Rainbow - LEDS: 60
    // Steps: 150 - Delay: 20
    // Colors: 3 (255.0.0, 0.255.0, 0.0.255)
    // Options: rainbowlen=60, toLeft=true,
    if (millis() - strip_0.effStart < 2 * (strip_0.effStep)) return 0x00;
    float factor1, factor2;
    uint16_t ind;
    for (uint16_t j = 0; j < 84; j++) {
        ind = strip_0.effStep + j * 2.5;
        switch ((int)((ind % 150) / 50)) {
            case 0:
                factor1 = 1.0 - ((float)(ind % 150 - 0 * 50) / 50);
                factor2 = (float)((int)(ind - 0) % 150) / 50;
                strip_0.strip.setPixelColor(j, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2);
                break;
            case 1:
                factor1 = 1.0 - ((float)(ind % 150 - 1 * 50) / 50);
                factor2 = (float)((int)(ind - 50) % 150) / 50;
                strip_0.strip.setPixelColor(j, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2);
                break;
            case 2:
                factor1 = 1.0 - ((float)(ind % 150 - 2 * 50) / 50);
                factor2 = (float)((int)(ind - 100) % 150) / 50;
                strip_0.strip.setPixelColor(j, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2);
                break;
        }
    }
    if (strip_0.effStep >= 150) {
        strip_0.Reset();
        return 0x03;
    } else
        strip_0.effStep++;
    return 0x01;
}
