# Info

This projects provides the code for the following deej volume mixer: https://www.printables.com/model/867600-deej-volume-mixer-arduinoledrgbmusic-control

To support the buttons, the following deej fork is required: https://github.com/Miodec/deej

The following libraries are used:  
[TaskScheduler](https://github.com/arkhipenko/TaskScheduler) - Cooperative multitasking for Arduino  
[FastLED](https://github.com/FastLED/FastLED) - FastLED library for colored LED animation on Arduino

## GitHub Info

The code on GitHub is only a snapshot. The development takes place on a self hosted git instance, thus you will not find a proper git history here.

# Installation

This code has been developed using vscode with the [PlatformIO](https://platformio.org/install/ide?install=vscode) plugin and tested with an Arduino Nano-V3 328PB Module.

With the PlatformIO plugin the installation should be straight forward.

# Configuration

In the `src/main.cpp` you can modify min and max brightness of the LEDs by changing the values of `MAXBRIGHTNESS` and `MINBRIGHTNESS`.

The colors of the app sliders are set in the `setup()` function. The values are provided in the HSV format, but keep in mind, that [FastLED maps the HSV color values to the 0-255 number range](https://github.com/FastLED/FastLED/wiki/FastLED-HSV-Colors). So a hue of 300 should be provided as 212.
