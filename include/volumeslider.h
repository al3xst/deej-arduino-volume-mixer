#include "ledstrip.h"

/// @brief Base class to manage a volume slider (potentiometer)
class VOLUME_SLIDER
{
public:
    /// @brief Constructor
    /// @param analogSliderValue pointer to the global analogSliderValue
    VOLUME_SLIDER(int *analogSliderValue) : analogSliderValue(analogSliderValue){};

    /// @brief Initilize the EMA filter with the first reading from the volume slider
    void init();

    /// @brief Calculate the new slider value using the EMA filter
    /// This should be called directly after reading a new value from the slider
    void applyFilterIteration();

    /// @brief EMA alpha value
    float EMA_a{0.6};
    /// @brief EMA S value (filtered sensor value)
    int EMA_S{0};

    int *analogSliderValue{nullptr};
};

/// @brief Helper class to manage a volume slider with its own LED strip
class GLOBAL_VOLUME_SLIDER : public VOLUME_SLIDER
{
public:
    /// @brief Constructor
    /// @param leds reference to the global LED strip
    /// @param analogSliderValue pointer to the global analogSliderValue
    GLOBAL_VOLUME_SLIDER(LED_STRIP &leds, int *analogSliderValue) : leds(leds), VOLUME_SLIDER(analogSliderValue){};

    /// @brief Calculates and set the proper color and brightness values for the LED strip
    /// Supports dimming LEDs to create a more smooth light effect
    void updateLED();

    /// @brief Sets the general color (hue and sat) for the LED strip and the max  brightness
    /// @param hue
    /// @param sat
    /// @param maxbrightness
    void setHSV(int hue, int sat, int maxbrightness);

    int hue{0};
    int sat{0};
    int maxbrightness{0};

    LED_STRIP &leds;
};

/// @brief Helper class to manage a volume slider with its own single LED
class APP_VOLUME_SLIDER : public VOLUME_SLIDER
{
public:
    /// @brief
    /// @param led
    /// @param analogSliderValue
    /// @param buttonValues
    /// @param lastButtonsValues
    APP_VOLUME_SLIDER(CRGB *led, int *analogSliderValue, int *buttonValues = nullptr, int *lastButtonsValues = nullptr)
        : led(led), VOLUME_SLIDER(analogSliderValue), buttonValues(buttonValues), lastButtonsValues(lastButtonsValues){};

    /// @brief
    void updateLED();

    /// @brief
    void updateBtn();

    /// @brief Sets the general color (hue and sat) for the LED and the min and max brightness
    /// @param hue
    /// @param sat
    /// @param minbrightness
    /// @param maxbrightness
    void setHSV(int hue, int sat, int minbrightness, int maxbrightness);

    CRGB *led;

    int *buttonValues{nullptr};
    int *lastButtonsValues{nullptr};

    bool mainButtonToggled{false};

    int hue{0};
    int sat{0};
    int minbrightness{0};
    int maxbrightness{0};

    int oldhue{0};
    int oldsat{0};

    unsigned long btn_time = 0;         // the last time the output pin was toggled
    unsigned long btn_debounce = 200UL; // the debounce time, increase if the output flickers
};
