..\..\srecord-1.63\srec_cat.exe Aurora.cpp.hex -Intel uTaskerUsbMsd-SmartMatrix.hex -Intel -Output Aurora.hex -Intel

..\..\srecord-1.63\srec_cat.exe ( ( ( Aurora.cpp.hex -Intel uTaskerUsbMsd-SmartMatrix.hex -Intel ) -crop 0x8080 0x40000 -offset -0x8080 ) -fill 0xFF 0x0000 0x37F7E -crc16-b-e 0x37f7E -xmodem -offset 0x8080 ) uTaskerBootloader-SmartMatrix.hex -Intel -Output AuroraWithBootAndUsbMsd.hex -Intel

..\..\srecord-1.63\srec_cat.exe ( ( Aurora.cpp.hex -Intel uTaskerUsbMsd-SmartMatrix.hex -Intel ) -crop 0x8080 0x40000 -offset -0x8080 ) -fill 0xFF 0x0000 0x37F7E -crc16-b-e 0x37f7E -xmodem -Output software.bin -Binary

"C:\Program Files (x86)\Arduino\hardware\tools\teensy_post_compile.exe" -file="Aurora" -path="C:\Users\Jason\Documents\Arduino\Aurora\bin" -tools="C:\Program Files (x86)\Arduino\hardware\tools"