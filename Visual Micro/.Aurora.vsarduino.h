/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Teensy 3.1, Platform=avr, Package=teensy
*/

#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
#define ARDUINO 106
#define ARDUINO_MAIN
#define __AVR__
#define __avr__
#define F_CPU 96000000
#define USB_SERIAL
#define LAYOUT_US_ENGLISH
#define __cplusplus
#define GCC_VERSION 40702
#define ARDUINO_ARCH_AVR
#define ARDUINO_AVR_TEENSY31
#define __MK20DX256__
#define TEENSYDUINO 120
#define __extension__
#define  __attribute__(x)
typedef void *__builtin_va_list;

#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __asm__ 
#define __volatile__

#define NEW_H
//#undef F
//extern char F(const char*);


extern int ffs (int __val) __ATTR_CONST__;
extern int ffsl (long __val) __ATTR_CONST__;
extern int ffsll (long long __val) __ATTR_CONST__;
extern void *memccpy(void *, const void *, int, size_t);
extern void *memchr(const void *, int, size_t) __ATTR_PURE__;
extern int memcmp(const void *, const void *, size_t) __ATTR_PURE__;
extern void *memcpy(void *, const void *, size_t);
extern void *memmem(const void *, size_t, const void *, size_t) __ATTR_PURE__;
extern void *memmove(void *, const void *, size_t);
extern void *memrchr(const void *, int, size_t) __ATTR_PURE__;
extern void *memset(void *, int, size_t);
extern char *strcat(char *, const char *);
extern char *strchr(const char *, int) __ATTR_PURE__;
extern char *strchrnul(const char *, int) __ATTR_PURE__;
extern int strcmp(const char *, const char *) __ATTR_PURE__;
extern char *strcpy(char *, const char *);
extern int strcasecmp(const char *, const char *) __ATTR_PURE__;
extern char *strcasestr(const char *, const char *) __ATTR_PURE__;
extern size_t strcspn(const char *__s, const char *__reject) __ATTR_PURE__;
extern char *strdup(const char *s1);
extern size_t strlcat(char *, const char *, size_t);
extern size_t strlcpy(char *, const char *, size_t);
extern size_t strlen(const char *) __ATTR_PURE__;
extern char *strlwr(char *);
extern char *strncat(char *, const char *, size_t);
extern int strncmp(const char *, const char *, size_t) __ATTR_PURE__;
extern char *strncpy(char *, const char *, size_t);
extern int strncasecmp(const char *, const char *, size_t) __ATTR_PURE__;
extern size_t strnlen(const char *, size_t) __ATTR_PURE__;
extern char *strpbrk(const char *__s, const char *__accept) __ATTR_PURE__;
extern char *strrchr(const char *, int) __ATTR_PURE__;
extern char *strrev(char *);
extern char *strsep(char **, const char *);
extern size_t strspn(const char *__s, const char *__accept) __ATTR_PURE__;
extern char *strstr(const char *, const char *) __ATTR_PURE__;
extern char *strtok(char *, const char *);
extern char *strtok_r(char *, const char *, char **);
extern char *strupr(char *);

#include <arduino.h>
#undef F
#define F(string_literal) ((const PROGMEM char *)(string_literal))
#undef cli
#define cli()
#define pgm_read_byte(address_short)
#define pgm_read_word(address_short)
#define pgm_read_word2(address_short)
#define digitalPinToPort(P)
#define digitalPinToBitMask(P) 
#define digitalPinToTimer(P)
#define analogInPinToBit(P)
#define portOutputRegister(P)
#define portInputRegister(P)
#define portModeRegister(P)
#include <Aurora.ino>
#include <Animations.h>
#include <Attractor.h>
#include <Aurora.h>
#include <BitmapPlayer.h>
#include <Bitmaps.h>
#include <Boid.h>
#include <ClockDigitalShort.h>
#include <ClockDisplay.h>
#include <ClockText.h>
#include <Drawable.h>
#include <Effects.h>
#include <GifPlayer.h>
#include <Hardware.h>
#include <IrCodes.h>
#include <Logo.h>
#include <Menu.h>
#include <MenuItem.h>
#include <MessagePlayer.h>
#include <PatternAnalogClock.h>
#include <PatternAttract.h>
#include <PatternBitmap.h>
#include <PatternBounce.h>
#include <PatternEffects.h>
#include <PatternElectricMandala.h>
#include <PatternFire.h>
#include <PatternFlock.h>
#include <PatternFlowField.h>
#include <PatternInfinity.h>
#include <PatternInvaders.h>
#include <PatternLife.h>
#include <PatternMaze.h>
#include <PatternMunch.h>
#include <PatternNoiseSmearing.h>
#include <PatternPendulumWave.h>
#include <PatternPlasma.h>
#include <PatternPulse.h>
#include <PatternRainbowSmoke.h>
#include <PatternSimplexNoise.h>
#include <PatternSnake.h>
#include <PatternSpark.h>
#include <PatternSpin.h>
#include <PatternSpiral.h>
#include <PatternSwirl.h>
#include <PatternWave.h>
#include <Patterns.h>
#include <Playlist.h>
#include <Runnable.h>
#include <Settings.h>
#include <SettingsAutoplayDuration.h>
#include <SettingsBackgroundBrightness.h>
#include <SettingsBrightness.h>
#include <SettingsClock24Hour.h>
#include <SettingsClockColor.h>
#include <SettingsMenuColor.h>
#include <SettingsMoveClock.h>
#include <SettingsSetTime.h>
#include <StreamingMode.h>
#include <Vector.h>
#endif
