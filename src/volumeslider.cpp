#include "volumeslider.h"

#include "math.h"

void VOLUME_SLIDER::init()
{
    EMA_S = *analogSliderValue;
}

void VOLUME_SLIDER::applyFilterIteration()
{
    EMA_S = (EMA_a * (*analogSliderValue)) + ((1 - EMA_a) * EMA_S);
}

void GLOBAL_VOLUME_SLIDER::updateLED()
{
    int analogVal = (EMA_S / 10) * 10;

    float ledamount_float = static_cast<float>(analogVal) / (1024.0 / static_cast<float>(leds.num_leds));
    int blackleds = leds.num_leds - static_cast<int>(ledamount_float);

    // set all to black
    for (int i{0}; i < leds.num_leds; ++i)
    {
        leds.leds[i].setHSV(hue, sat, 0);
    }

    // turn on all fully lit leds

    // forwards
    //  for (int i{0}; i < ledamount; ++i)
    //  {
    //      leds.leds[i].setHSV(hue, sat, brightness);
    //  }

    // backwards
    for (int i{leds.num_leds}; i > blackleds; --i)
    {
        leds.leds[i - 1].setHSV(hue, sat, maxbrightness);
    }

    // turn on the last led dimmed
    float dim = ledamount_float - floor(ledamount_float);
    // leds.leds[static_cast<int>(ledamount_float)].setHSV(hue, sat, brightness * dim); //forwards
    leds.leds[blackleds - 1].setHSV(hue, sat, maxbrightness * dim); // backwards
}

void GLOBAL_VOLUME_SLIDER::setHSV(int hue, int sat, int maxbrightness)
{
    this->hue = hue;
    this->sat = sat;
    this->maxbrightness = maxbrightness;

    for (int i{0}; i < leds.num_leds; ++i)
    {
        leds.leds[i].setHSV(hue, sat, maxbrightness);
    }
}

void APP_VOLUME_SLIDER::updateLED()
{
    int analogVal = (EMA_S / 10) * 10;

    int brightness = analogVal / (1024 / (maxbrightness - minbrightness));
    brightness += minbrightness;
    led->setHSV(hue, sat, brightness);
};

void APP_VOLUME_SLIDER::updateBtn()
{
    if (buttonValues[2] == 0 && lastButtonsValues[2] == 1 && millis() - btn_time > btn_debounce)
    {
        this->mainButtonToggled = !this->mainButtonToggled;
        if (this->mainButtonToggled)
        {
            this->oldhue = this->hue;
            this->hue = 0;
        }
        else
        {
            this->hue = 87;
        }

        btn_time = millis();
    }
}

void APP_VOLUME_SLIDER::setHSV(int hue, int sat, int minbrightness, int maxbrightness)
{
    this->hue = hue;
    this->oldhue = hue;
    this->sat = sat;
    this->oldsat = sat;
    this->minbrightness = minbrightness;
    this->maxbrightness = maxbrightness;
}
