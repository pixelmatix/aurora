Aurora
======

Aurora is a bright and aesthetically-pleasing display, driven by 1,024 RGB LEDS behind a diffusion screen. It's built using the [SmartMatrix SD Shield]
and a 32x32 RGB LED matrix.

Features
--------

* Procedural pattern viewer
* Animated GIF slideshow viewer
* Remote control, allowing for ease of use.
* Optional Clock display in the foreground, over animation and pattern slideshows (**requires real-time clock chip**).

Patterns  
[![Patterns](http://img.youtube.com/vi/yucr3tPudL4/0.jpg)](http://www.youtube.com/watch?v=yucr3tPudL4)  

Animations  
[![Animations](http://img.youtube.com/vi/veYClVCJSAs/0.jpg)](http://www.youtube.com/watch?v=veYClVCJSAs)  

Settings  
[![Settings](http://img.youtube.com/vi/sOlAI7qmnBM/0.jpg)](http://www.youtube.com/watch?v=sOlAI7qmnBM)  

Clock
-----

Clock display and settings require an optional real-time clock (RTC) module.  If one is not present, clock display and settings will not be available. 
Aurora uses the [DS1307RTC Library], which supports DS1307, DS1337 and DS3231 real time clock chips that use the I2C interface. 
Here are some compatible boards that are available:
* [ChronoDot] Ultra-precise Real Time Clock
* [DS1307] Real Time Clock

Micro SD Card
-------------

Aurora makes use of an optional MicroSD card to display animated GIF images and to store user preferences and settings.  Aurora will still work fine if
no MicroSD card is inserted into the slot, but obviously no animated GIFs will be displayed, and changes to any settings will not be retained if the display
loses power.

Animated GIF Image support
--------------------------

Animated GIFs to be displayed must meet the folowing requirements:

* 32x32 size (32 pixels wide and 32 pixels high)
* Indexed color mode
* Placed in a 'gifs' directory in the root of the MicroSD card.
 
Aurora Setting Storage
----------------------

Aurora stores settings in a directory named Aurora in the root of the MicroSD card. Aurora will create the directory automatically if it does not exist.

Installation
------------

Aurora includes and makes use of several optional files and example animated GIF images which can be downloaded, extracted, and copied into the
root of your MicroSD card:

1. Download the [Aurora MicroSD Card] zip file.
2. Extract all contents of the zip file.
3. Insert a MicroSD card into your computer, using a card reader.
4. Copy the extracted contents of the zip file to your MicroSD card.
    * The card should now contain at least two directories: aurora and gifs.
5. Prepare the MicroSD card for safe removal from your computer by first ejecting, unmounting, etc.
    * Removing the MicroSD card from your computer without first preparing it can cause the corruption and/or loss of data.
6. Remove the MicroSD card from your computer.
7. Insert the MicroSD card into the MicroSD card slot on the SmartMatrix board. It's located underneath the Micro USB port.
   Make sure the label on the MicroSD card is facing up, and it's inserted with the narrower end with the contacts first.

Connect the display to power:

1.  Insert the power cable into the power port on the back of the display.
2.  Plug the power cable in to a wall power outlet (the display does not draw power from the Micro USB cable).

The display will likey not light up or do anything, as the Aurora software did not come pre-installed on your display.
Next we'll install the Aurora software.  First connect your display to your computer:

1.  Insert the smaller end of a Micro USB cable into the Micro USB port on the back of the display.
2.  Insert the larger end of the Micro USB cable into one of your computer's USB ports.

Next download the required software:

1.  Download the [Aurora.hex] file.
2.  Download the [Teensy Loader Application] for your computer's operating system.
3.  Follow the instructions for running and using the Teensy Loader Application on the 'Using The Teensy Loader' page for your computer's operating system.

Finally, install the Aurora software on your display:

1.  Click the 'Open HEX File' button on the toolbar or click the File menu, then click 'Open HEX File'.
2.  Find and open the Aurora.hex file you downloaded.
2.  Click the Program button on the toolbar or click the Operation menu, then click Program.

The Aurora software should now be running on your display. 

Remote Control
--------------

Aurora can use:

[![SparkFun Infrared Remote Control](https://cdn.sparkfun.com/r/92-92/assets/parts/7/9/8/6/11759-01.jpg)](https://www.sparkfun.com/products/11759)  

[SparkFun Infrared Remote Control]  

[![Adafruit Infrared Remote Control](http://www.adafruit.com/images/230x173/389-00.jpg)](http://www.adafruit.com/products/389)  

[Adafruit Infrared Remote Control]

SparkFun remote button functions:

* ![Power Symbol] Power
    * Press to turn on the display.
    * Press to toggle through brightness presets:
        * Off, 1%, 25%, 50%, 75%, 100%
    * Hold to turn off the display without changing the brightness.
    * Unless unplugged, the display is never completely powered off. It enters a low-power state, where it uses
       just enough power to poll the infrared sensor.
* **^** Up
    * Move to the previous menu item.
* **v** Down
    * Move to the next menu item.
* **<** Left
    * Switch to the previous content (Pattern or Animation).
* **>** Right
    * Switch to the next content (Pattern or Animation).
* **O** Select
    * Selects the currently displayed content, hiding the main menu.
    * Returns to the main menu from displayed content.
* **A**
    * Press the A button to toggle through playback modes:
        * Pause
            * Stay on the current content until the left or right button is pressed.
        * Autoplay
            * Automatically switches to the next content after a short time.
        * Random
            * Automatically switches to a random content after a short time.
* **B**
    * Switch to the next color palette in Pattern mode.
    * Most, but not all Patterns support multiple color palettes such as Rainbow, Fire, Ice, Forest, etc.
* **C**
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

Aurora requires a 5 volt power supply, capable of providing 4 amps of peak power.  When in idle, low power mode, Aurora uses only 1.2 watts.
When turned on, power use depends on the brightness and pattern displayed.  When on 100% brightness with every LED on full white, it can draw as much as 20 watts, which is still only half of what a standard (40W) incandescent light bulb uses.

All power use was measured using a [Kill A Watt].

Compiling Aurora
----------------

For those familiar with Arduino development, Aurora can easily be compiled from source.

Required software:

* [Teensyduino Software]
* [Arduino Software] *(currently requires version 1.0.6)*

Source code and libraries:

* [Aurora Source Code]
* [SmartMatrix Library]
* [FastLED v2.1 Library] **(make sure you use the 2.1 branch!)**
* [IR Remote Library]
* [SD Library]
* [Time Library]
* [DS1307RTC Library]

After downloading and installing all of the software, download all of the required libraries and extract/place them in your 
Arduino libraries directory on your computer.  For more information, see [Installing Additional Arduino Libraries]

Next, connect your Aurora display. See installation instructions above for more information.

Finally, open Aurora.ino in the Arduino IDE and click the Upload button.

Troubleshooting
---------------

If you have any problems using Aurora (crashes, glitches, etc), please search the [Aurora Issue Tracker] before creating a new issue.    
    
If you have any problems compiling Aurora, please make sure all the above libraries are properly installed.
Please refer to [Installing Additional Arduino Libraries], and [SmartMatrix External Libraries] for more troubleshooting information.

Acknowledgements
----------------

Many thanks to all of the fantastic hardware and open source software libraries used in Aurora!

* [SmartMatrix SD Shield] by Louis Beaudoin
* [SmartMatrix Library] by Louis Beaudoin
* [FastLED v2.1 Library] by Daniel Garcia and Mark Kriegsman
* [IR Remote Library] by Ken Shirriff
* [SD Library] by Bill Greiman
* [Teensy v3.1] by Paul Stoffregen
* [Teensyduino Software] by Paul Stoffregen
* [Time Library] by Paul Stoffregen
* [DS1307RTC Library]
* [Teensyduino Software]
* [Arduino Software]
 
A huge thank you to all of the projects and open source software that are used in and inspired Aurora!
* [Light Appliance] by Craig Lindley
* [Funky Clouds] by [Stefan Petrick]
* [Rainbow Smoke] by [Jozsef Fejes]
* [LED Effects] by Robert Atkins
* [The Nature of Code] by [Daniel Shiffman]

Thanks to these fine game companies for allowing us to use and distribute their pixel art to showcase Aurora!  
![Tiny Barbarian](https://github.com/pixelmatix/aurora/blob/master/sd/gifs/TinyFlex.gif) [Tiny Barbarian] by [StarQuail Games]  
![Chasm](https://github.com/pixelmatix/aurora/blob/master/sd/gifs/ChasmDaltynCrouch.gif) [Chasm] by [Discord Games]

[Aurora Source Code]:https://github.com/pixelmatix/aurora/archive/master.zip
[Aurora Issue Tracker]:https://github.com/pixelmatix/aurora/issues
[SparkFun Infrared Remote Control]:https://www.sparkfun.com/products/11759
[SparkFun Infrared Remote Image]:https://cdn.sparkfun.com/r/92-92/assets/parts/7/9/8/6/11759-01.jpg
[Adafruit Infrared Remote Control]:http://www.adafruit.com/products/389
[Adafruit Infrared Remote Image]:http://www.adafruit.com/images/230x173/389-00.jpg
[ChronoDot]:http://www.adafruit.com/products/255
[DS1307]:https://www.adafruit.com/product/264
[Aurora.hex]:https://github.com/pixelmatix/aurora/raw/master/bin/Aurora.hex
[Aurora MicroSD Card]:https://github.com/pixelmatix/aurora/blob/master/sd/sd.zip?raw=true
[Teensyduino Software]:https://www.pjrc.com/teensy/td_download.html
[Installing Additional Arduino Libraries]:http://arduino.cc/en/Guide/Libraries
[Teensy Loader Application]:https://www.pjrc.com/teensy/loader.html
[Arduino Software]:http://www.arduino.cc/en/Main/Software
[SmartMatrix Library]:https://github.com/pixelmatix/SmartMatrix
[SmartMatrix SD Shield]:http://docs.pixelmatix.com/SmartMatrix/shield-sd.html
[SmartMatrix External Libraries]:http://docs.pixelmatix.com/SmartMatrix/#external-libraries
[FastLED v2.1 Library]:https://github.com/FastLED/FastLED/tree/FastLED2.1
[IR Remote Library]:https://github.com/shirriff/Arduino-IRremote
[SD Library]:https://github.com/adafruit/SD
[Time Library]:https://github.com/PaulStoffregen/Time
[DS1307RTC Library]:https://www.pjrc.com/teensy/td_libs_DS1307RTC.html
[Teensy v3.1]:https://www.pjrc.com/teensy/index.html
[Power Symbol]:http://upload.wikimedia.org/wikipedia/commons/thumb/b/b2/IEC5009_Standby_Symbol.svg/16px-IEC5009_Standby_Symbol.svg.png
[Kill A Watt]:http://www.p3international.com/products/p4400.html

[Light Appliance]:https://github.com/CraigLindley/LightAppliance
[Funky Clouds]:https://gist.github.com/anonymous/2eb826070e108b09b8fd
[Stefan Petrick]:http://www.stefan-petrick.de/wordpress_beta
[Rainbow Smoke]:http://rainbowsmoke.hu
[Rainbow Smoke Codegolf]:http://codegolf.stackexchange.com/a/22326
[Jozsef Fejes]:http://joco.name/
[LED Effects]:https://bitbucket.org/ratkins/ledeffects
[The Nature of Code]:http://natureofcode.com/
[Daniel Shiffman]:http://shiffman.net/

[Tiny Barbarian]:http://www.tinybarbarian.com/
[StarQuail Games]:http://www.starquail.com/
[Chasm]:http://chasmgame.com/
[Discord Games]:http://discordgames.com/
