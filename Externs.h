/*
 * Externs.h
 *
 *  Created on: May 21, 2015
 *      Author: Konstantin Gredeskoul
 *        Code: https://github.com/kigster
 *
 */

#ifndef EXTERNS_H_
#define EXTERNS_H_

extern char versionText[];
extern bool sdAvailable;
extern SmartMatrix matrix;
extern IRrecv irReceiver;
extern boolean hasDS1307RTC;
extern rotationDegrees rotation;
extern uint8_t brightness;
extern uint8_t backgroundBrightness;
extern const uint8_t brightnessCount;
extern uint8_t brightnessMap[];
extern uint8_t backgroundBrightnessMap[];
extern char* brghtnssFilename;
extern char* bckbrghtFilename;
extern char* audiosclFilename;
extern char* menuRFilename;
extern char* menuGFilename;
extern char* menuBFilename;
extern char* autoplydFilename;
extern bool isTimeAvailable;
extern int autoPlayDurationSeconds;

extern bool setPattern(String name);
extern bool setAnimation(String name);
extern void listAnimations();
extern void listPatterns();
extern int loadByteSetting(const char* name, byte defaultValue);
extern void saveByteSetting(const char* name, byte value);
extern bool setTemperature(int temperature);
extern bool setWeatherType(int type);
extern void reloadAnimations();
extern void listAudioPatterns();
extern bool setAudioPattern(String name);
extern void saveAutoPlayDurationSeconds();
extern tmElements_t loadDateTimeSetting(const char* name);
extern void restartAndJumpToApp(void);
extern time_t getTeensy3Time();
extern void adjustBrightness(int delta, boolean wrap);
extern void toggleSettingsMenuVisibility();
extern void adjustDemoMode(int delta);
extern void saveDemoMode();
extern void saveMenuColor();

class GifPlayer;
class Weather;
class MessagePlayer;
class ClockDigitalShort;
class ClockText;
class ClockCountdown;
class ClockPong;
class ClockDisplay;
class Patterns;
class AudioPatterns;
class Animations;
class StreamingMode;
class Menu;
class MenuItem;
class Games;
class Settings;
class Effects;

extern MessagePlayer messagePlayer;
extern Effects effects;
extern GifPlayer gifPlayer;
extern tmElements_t time;
extern ClockDigitalShort clockDigitalShort;
extern ClockText clockText;
extern ClockCountdown clockCountdown;
extern ClockPong clockPong;
extern ClockDisplay clockDisplay;
extern Patterns patterns;
extern AudioPatterns audioPatterns;
extern Animations animations;
extern Weather weather;
extern rgb24 menuColor;
extern StreamingMode streamingMode;
extern Menu menu;
extern Games games;
extern Settings settings;
extern MenuItem menuItemAudioPatterns;
extern MenuItem menuItemPatterns;
extern MenuItem menuItemAnimations;
extern MenuItem menuItemGames;
extern MenuItem menuItemWeather;
extern MenuItem menuItemSettings;
extern MenuItem* mainMenuItems [];

#endif
