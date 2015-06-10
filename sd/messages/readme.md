Messages
========
To display scrolling text messages in Aurora, you can place text or JSON files in a messages directory on your SD card.

Text format
-----------

Text files should have an extension of .txt, such as Message.txt.

text=message to be displayed. REQUIRED (up to 50 characters in length)  
red=red value (0-255) default is 0 (if no color component is specified, the default color is white, or 255, 255, 255) 
green=green value (0-255) default is 0 
blue=blue value (0-255) default is 0 
top=vertical location (top of the text) (0-31) default is 1 
left=horizontal location (left of the text) (0-31) default is 1 
font=font name (font3x5, font5x7, font6x10, font8x13, gohufont11, gohufont11b) default is gohufont11b
speed=scroll speed in pixels per second (0-255) default is 24 
mode=scroll mode (wrapForward, bounceForward, bounceReverse, stopped, off, wrapForwardFromLeft) default is wrapForward

Here's an example text file:  
    
    text=Hello World!  
    red=255  
    green=0  
    blue=0  
    top=15  
    left=10
    font=font8x13
    speed=80
    mode=wrapForwardFromLeft

JSON format
-----------

JSON files should have an extension of .js, such as Message.js.

JSON formatted message files use the same settings as the text format above, with slightly different delimiters:

    {
      "text":"Hello World!",
      "red":255,
      "green":0,
      "blue":0,
      "top":15,
      "left":10,
      "font":"font8x13",
      "speed":80,
      "mode":"wrapForwardFromLeft"
    }
