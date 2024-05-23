#include "FastLED.h"

class LED_STRIP
{
public:
    LED_STRIP(CRGB *leds, int num_leds) : leds(leds), num_leds(num_leds) {}

    void showrandom();

    CRGB *leds;
    int num_leds{0};
};
