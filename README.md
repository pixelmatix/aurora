Aurora
======
------

Features
--------

Aurora is a bright and aesthetically-pleasing display, driven by 1,024 RGB LEDS behind a diffusion screen.

* Procedural pattern viewer
* Animated GIF slideshow viewer
* Optional remote control, allowing for ease of use.
* Optional Clock display in the foreground, over animation and pattern slideshows.

Installation
------------

If Aurora software did not come pre-installed on your display, you can install Aurora manually.  First connect your display to your computer and supply power to the display:

1.  Insert the smaller end of a Micro USB cable into the Micro USB port on the back of the display.
2.  Insert the larger end of the Micro USB cable into your computer.
3.  Insert the power cable into the power port on the back of the display.
4.  Plug the power cable in to a wall power receptacle (the display does not draw power from the Micro USB cable).

Next download the required software:

1.  Download the [Aurora.hex] file.
2.  Choose and download the [Teensy Loader Application] for your computer's operating system.
3.  Follow the instructions for running and using the Teensy Loader Application on the 'Using The Teensy Loader' page for your computer's operating system.

Finally, install the Aurora software on your display:

1.  Click the 'Open HEX File' button on the toolbar or click the File menu, then click 'Open HEX File'.
2.  Find and open the Aurora.hex file you downloaded.
2.  Click the Program button on the toolbar or click the Operation menu, then click Program.

The Aurora software should now be running on your display. 

Micro SD Card
-------------

Aurora makes use of an optional MicroSD card to display animated GIF images and to store user preferences and settings.  Aurora will still work fine if no MicroSD card is inserted into the slot, but obviously no animated GIFs will be displayed, and changes to any settings will not be retained if the display loses power.

Aurora includes and makes use of several optional files and example animated GIF images which can be downloaded and copied into the root of your MicroSD card:

[Aurora MicroSD Card]

Animated GIF Image support
--------------------------

Animated GIFs to be displayed must meet the folowing requirements:

* 32x32 size (32 pixels wide and 32 pixels high)
* Indexed color mode
* Placed in a 'gifs' directory in the root of the MicroSD card.
 
Aurora Setting Storage
----------------------

Aurora stores settings in a directory named Aurora in the root of the MicroSD card. Aurora will create the directory automatically if it does not exist.

Remote Control
--------------

Aurora utilizes the [Sparkfun Infrared Remote Control]

![Sparkfun Infrared Remote Image]

Button functions:

* Power ![Power Symbol]
    * Press to turn on the display.
    * Press to toggle through brightness presets:
        * Off, 1%, 25%, 50%, 75%, 100%
    * Hold to turn off the display without changing the brightness.
    * Unless unplugged, the display is never completely powered off. It enters a low-power state, where it uses
       just enough power to poll the infrared sensor.
* Up ^
    * Move to the previous menu item.
* Down v
    * Move to the next menu item.
* Left <
    * Switch to the previous content (Pattern or Animation).
* Right >
    * Switch to the next content (Pattern or Animation).
* Select O
    * Selects the currently displayed content, hiding the main menu.
    * Returns to the main menu from displayed content.
* A
    * Press the A button to toggle through playback modes:
        * Pause
            * Stay on the current content until the left or right button is pressed.
        * Autoplay
            * Automatically switches to the next content after a short time.
        * Random
            * Automatically switches to a random content after a short time.
* B
    * Switch to the next color palette in Pattern mode.
    * Most, but not all Patterns support multiple color palettes such as Rainbow, Fire, Ice, Forest, etc.
* C
    * Shows and hides the clock display when content is selected and the main menu is hidden.

Aurora Menu
-----------

* Patterns
    * Dynamic, procedurally generated color displays.
* Animations
    * Animated GIF image files read from the MicroSD card.
* Settings
    * Brightness
        * Adjust the overall display brightness using the up and down buttons.
        * 1%, 25%, 50%, 75%, 100%
        * Press select button to return to the settings menu.
    * BG Brightness (background brightness)
        * Adjust the brightness of content shown in the background, under the menu and clock text, using the up and down buttons.
        * 1%, 25%, 50%, 75%, 100%
        * Press select button to return to the settings menu.
    * Move Clock
        * Adjust the vertical location of the clock using the up and down buttons.
        * Press select button to return to the settings menu.
    * Clock Color
        * Change the color of the clock text using the up, down, left, and right buttons.
        * Press select button to return to the settings menu.
    * 12/24 Hour Clock
        * Press the up and down buttons to change from 12 to 24 hour clock display.
        * Press select button to return to the settings menu.
    * Set Time
        * Press up and down buttons to adjust the hour.
        * Press right button to change to minute adjustment.
        * Press up and down buttons to adjust the minute.
        * Press select button to return to the settings menu.
    * Exit Settings
        * Press select button to return to the main menu.

Power Utilization
-----------------

Aurora requires a 5 volt power supply, capable of providing 4 amps of peak power.  When in idle, low power mode, Aurora uses only 1.2 watts.  When turned on, power use depends on the brightness and pattern displayed.  When on 100% brightness with every LED on full white, it can draw as much as 20 watts, which is still only half of what a standard (40W) incandescent light bulb uses.

All power use was measured using a [Kill A Watt].

Compiling Aurora
----------------

For those familiar with Arduino development, Aurora can easily be compiled from source.

Required software:

* [Teensyduino Software]
* [Arduino Software] *(currently requires version 1.0.6)*

Aurora source code:

* [Arduino Software]

Required libraries:

* [SmartMatrix Library]
* [FastLED v2.1 Library]
* [IR Remote Library]
* [SdFat Library]
* [Time Library]
* [DS1307RTC Library]

After downloading and installing all of the software, download all of the required libraries and extract/place them in your Arduino libraries directory on your computer.  For more information, see [Installing Additional Arduino Libraries]

Next, connect your Aurora display. See installation instructions above for more information.

Finally, open Aurora.ino in the Arduino IDE and click the Upload button.

Acknowledgements
----------------

Many thanks to all of the fantastic hardware and open source software libraries used in Aurora!

* [SmartMatrix] by Louis Beaudoin
* [SmartMatrix Library] by Louis Beaudoin
* [FastLED v2.1 Library] by Daniel Garcia and Mark Kriegsman
* [Light Appliance] by Craig Lindley
* [IR Remote Library] by Ken Shirriff
* [SdFat Library] by Bill Greiman
* [Teensy v3.1] by Paul Stoffregen
* [Teensyduino Software] by Paul Stoffregen
* [Time Library] by Paul Stoffregen
* [DS1307RTC Library]

[Aurora.hex]:https://github.com/pup05/aurora/blob/master/bin/Aurora.hex
[Aurora MicroSD Card]:https://github.com/pup05/aurora/tree/master/sd 
[Teensy Loader Application]:https://www.pjrc.com/teensy/loader.html
[Teensyduino Software]:https://www.pjrc.com/teensy/td_download.html
[Arduino Software]:http://www.arduino.cc/en/Main/Software
[Installing Additional Arduino Libraries]:http://arduino.cc/en/Guide/Libraries
[SmartMatrix]:http://docs.pixelmatix.com/SmartMatrix/
[SmartMatrix Library]:https://github.com/pixelmatix/SmartMatrix
[SmartMatrix Library Documentation]:http://docs.pixelmatix.com/SmartMatrix/library.html
[FastLED v2.1 Library]:https://github.com/FastLED/FastLED/tree/FastLED2.1
[IR Remote Library]:https://github.com/shirriff/Arduino-IRremote
[SdFat Library]:https://github.com/greiman/SdFat/tree/master/SdFat
[Time Library]:https://github.com/PaulStoffregen/Time
[DS1307RTC Library]:https://www.pjrc.com/teensy/td_libs_DS1307RTC.html
[Teensy v3.1]:https://www.pjrc.com/teensy/index.html
[Light Appliance]:https://github.com/CraigLindley/LightAppliance
[Sparkfun Infrared Remote Control]:https://www.sparkfun.com/products/11759
[Sparkfun Infrared Remote Image]:https://cdn.sparkfun.com/r/92-92/assets/parts/7/9/8/6/11759-01.jpg
[Power Symbol]:http://upload.wikimedia.org/wikipedia/commons/thumb/b/b2/IEC5009_Standby_Symbol.svg/16px-IEC5009_Standby_Symbol.svg.png
[Kill A Watt]:http://www.p3international.com/products/p4400.html
