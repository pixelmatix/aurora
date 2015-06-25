mv Aurora.cpp.hex Aurora.offset.hex

srec_cat.exe Aurora.hex -Intel uTaskerUsbMsd-SmartMatrix.hex -Intel -Output AuroraWithUsbMsd.hex -Intel

srec_cat.exe ( ( ( Aurora.offset.hex -Intel uTaskerUsbMsd-SmartMatrix.hex -Intel ) -crop 0x8080 0x40000 -offset -0x8080 ) -fill 0xFF 0x0000 0x37F7E -crc16-b-e 0x37f7E -xmodem -offset 0x8080 ) uTaskerBootloader-SmartMatrix.hex -Intel -Output AuroraWithBootAndUsbMsd.hex -Intel

srec_cat.exe ( ( Aurora.offset.hex -Intel uTaskerUsbMsd-SmartMatrix.hex -Intel ) -crop 0x8080 0x40000 -offset -0x8080 ) -fill 0xFF 0x0000 0x37F7E -crc16-b-e 0x37f7E -xmodem -Output software.bin -Binary