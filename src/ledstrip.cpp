#include "ledstrip.h"

void LED_STRIP::showrandom()
{
    for (int i = 0; i < num_leds; ++i)
    {
        leds[i] = CHSV(random8(), 255, 255); // hue, saturation, value
    }
    FastLED.show();
}
