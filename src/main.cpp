#include <TaskScheduler.h>

#include "FastLED.h"

#include "volumeslider.h"

// Debugging macros

// Debug and Test options
// #define _DEBUG_
// #define _TEST_

#ifdef _DEBUG_
#define SerialD Serial
#define _PM(a)               \
    SerialD.print(millis()); \
    SerialD.print(": ");     \
    SerialD.println(a)
#define _PP(a) SerialD.print(a)
#define _PL(a) SerialD.println(a)
#define _PX(a) SerialD.println(a, HEX)
#else
#define _PM(a)
#define _PP(a)
#define _PL(a)
#define _PX(a)
#endif

// FastLED

#define DATA_PIN 5      // buttons led
#define NUM_LEDS 4      // buttons led
#define KNOB_DATA_PIN 3 // knob led
#define KNOB_NUM_LEDS 7 // knob led
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

#define MAXBRIGHTNESS 150
#define MINBRIGHTNESS 50

CRGB leds[NUM_LEDS];
CRGB knob_leds[KNOB_NUM_LEDS];

const int NUM_SLIDERS = 5;
const int analogInputs[NUM_SLIDERS] = {A6, A2, A1, A0, A3};
const int NUM_BUTTONS = 3;
const int buttonInputs[NUM_BUTTONS] = {8, 7, 6};

int analogSliderValues[NUM_SLIDERS];
int buttonValues[NUM_BUTTONS];
int lastButtonValues[NUM_BUTTONS];

LED_STRIP btn_led_strip(leds, NUM_LEDS);
LED_STRIP knob_led_strip(knob_leds, KNOB_NUM_LEDS);

GLOBAL_VOLUME_SLIDER globalVolume(knob_led_strip, &analogSliderValues[0]);

APP_VOLUME_SLIDER musicVolume(&leds[0], &analogSliderValues[1], buttonValues, lastButtonValues);
APP_VOLUME_SLIDER browserVolume(&leds[1], &analogSliderValues[2]);
APP_VOLUME_SLIDER voiceVolume(&leds[2], &analogSliderValues[3]);
APP_VOLUME_SLIDER gamesVolume(&leds[3], &analogSliderValues[4]);

// Callback Forward Definitions
void updateSliderValues();
void sendSliderValues();
void updateLEDs();
void showLEDs();

// Scheduler
Scheduler ts;

void setup()
{
    delay(1000); // initial delay of a few seconds is recommended

    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);                // init btn LED strip
    FastLED.addLeds<LED_TYPE, KNOB_DATA_PIN, COLOR_ORDER>(knob_leds, KNOB_NUM_LEDS).setCorrection(TypicalLEDStrip); // init knob LED strip

    FastLED.setBrightness(MAXBRIGHTNESS); // global brightness

    for (int i = 0; i < NUM_SLIDERS; i++)
    {
        pinMode(analogInputs[i], INPUT);
    }

    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        pinMode(buttonInputs[i], INPUT_PULLUP);
    }

    Serial.begin(9600);

    musicVolume.setHSV(87, 255, MINBRIGHTNESS, MAXBRIGHTNESS);   // green
    browserVolume.setHSV(50, 255, MINBRIGHTNESS, MAXBRIGHTNESS); // yellow
    voiceVolume.setHSV(144, 255, MINBRIGHTNESS, MAXBRIGHTNESS);  // blue
    gamesVolume.setHSV(250, 255, MINBRIGHTNESS, MAXBRIGHTNESS);  // red

    globalVolume.setHSV(0, 0, MAXBRIGHTNESS - 60); // white, decrease max brigthness because white is already quite bright

    // read initial volume slider values
    for (int i = 0; i < NUM_SLIDERS; i++)
    {
        analogSliderValues[i] = analogRead(analogInputs[i]);
    }

    musicVolume.init();
    browserVolume.init();
    voiceVolume.init();
    gamesVolume.init();
    globalVolume.init();
}

// main program
void loop()
{
    ts.execute();
}

// Task definitions
Task task_read_slider(10 * TASK_MILLISECOND, TASK_FOREVER, &updateSliderValues, &ts, true);
Task task_show_leds(15 * TASK_MILLISECOND, TASK_FOREVER, &showLEDs, &ts, true);
Task task_update_leds(15 * TASK_MILLISECOND, TASK_FOREVER, &updateLEDs, &ts, true);
Task task_send_slider(10 * TASK_MILLISECOND, TASK_FOREVER, &sendSliderValues, &ts, true);

// read slider values and apply EMS filter
void updateSliderValues()
{
    for (int i = 0; i < NUM_SLIDERS; i++)
    {
        analogSliderValues[i] = analogRead(analogInputs[i]);
    }

    for (int i = 0; i < NUM_BUTTONS; i++)
    {

        lastButtonValues[i] = buttonValues[i];
        buttonValues[i] = digitalRead(buttonInputs[i]);
    }

    musicVolume.applyFilterIteration();
    // musicVolume.updateBtn();
    browserVolume.applyFilterIteration();
    voiceVolume.applyFilterIteration();
    gamesVolume.applyFilterIteration();

    globalVolume.applyFilterIteration();
}

// update led state of all knobs
void updateLEDs()
{
    musicVolume.updateLED();
    browserVolume.updateLED();
    voiceVolume.updateLED();
    gamesVolume.updateLED();

    globalVolume.updateLED();
}

// display leds
void showLEDs()
{
    FastLED.show();
}

// send slider and button values to serial console in deej format
void sendSliderValues()
{
    String builtString = String("");

    for (int i = 0; i < NUM_SLIDERS; i++)
    {
        builtString += "s";
        builtString += String((int)analogSliderValues[i]);

        if (i < NUM_SLIDERS - 1)
        {
            builtString += String("|");
        }
    }

    if (NUM_BUTTONS > 0)
    {
        builtString += String("|");
    }

    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        builtString += "b";
        builtString += String((int)buttonValues[i]);

        if (i < NUM_BUTTONS - 1)
        {
            builtString += String("|");
        }
    }

    Serial.println(builtString);
}

// print slider and button values for debug purposes to the serial console
void printSliderValues()
{
    for (int i = 0; i < NUM_SLIDERS; i++)
    {
        String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
        Serial.write(printedString.c_str());

        if (i < NUM_SLIDERS - 1)
        {
            Serial.write(" | ");
        }
        else
        {
            // Serial.write("\n");
        }
    }

    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        String printedString = String("Btn #") + String(i + 1) + String(": ") + String(buttonValues[i]) + String(" mV");
        Serial.write(printedString.c_str());

        if (i < NUM_BUTTONS - 1)
        {
            Serial.write(" | ");
        }
        else
        {
            Serial.write("\n");
        }
    }
}
