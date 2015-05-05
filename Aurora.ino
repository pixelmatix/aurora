/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * Designed in collaboration with Pixelmatix using the SmartMatrix Library: http://pixelmatix.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "Hardware.h"

#include "Aurora.h"

#include <SmartMatrix_32x32.h>
#include <FastLED.h>
#include <IRremote.h>
#include <SPI.h>
#include <SD.h>

#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>

#include <aJSON.h>

char versionText[] = "v1.5";

bool sdAvailable = false;
SmartMatrix matrix;
IRrecv irReceiver(IR_RECV_PIN);

boolean hasDS1307RTC = false;

rotationDegrees rotation = rotation0;

uint8_t brightness = 255;
uint8_t backgroundBrightness = 255;

const uint8_t brightnessCount = 5;
uint8_t brightnessMap[brightnessCount] = { 16, 32, 64, 128, 255 };
uint8_t backgroundBrightnessMap[brightnessCount] = { 16, 32, 64, 128, 255 };

#include "MessagePlayer.h"
MessagePlayer messagePlayer;

#include "AudioLogic.h"

#include "Effects.h"
Effects effects;

#include "IrCodes.h"

#include "GifPlayer.h"
GifPlayer gifPlayer;

//#include "BitmapPlayer.h"
//BitmapPlayer bitmapPlayer;

#include "Logo.h"

#include "Drawable.h"
#include "Playlist.h"
#include "Runnable.h"

#include "Geometry.h"

tmElements_t time;
bool isTimeAvailable = false;

#include "ClockDigitalShort.h"
ClockDigitalShort clockDigitalShort;

#include "ClockText.h"
ClockText clockText;

#include "ClockDisplay.h"
ClockDisplay clockDisplay;

#include "Patterns.h"
Patterns patterns;

#include "AudioPatterns.h"
AudioPatterns audioPatterns;

#include "Animations.h"
Animations animations;

#include "Bitmaps.h"

rgb24 menuColor = CRGB(CRGB::Blue);
int autoPlayDurationSeconds = 10;

#include "StreamingMode.h"
StreamingMode streamingMode;

#include "MenuItem.h"
#include "Menu.h"
Menu menu;

#include "Settings.h"
Settings settings;

#include "SettingsSetTime.h"
#include "SettingsMoveClock.h"

MenuItem menuItemAudioPatterns = MenuItem((char*)"Audio Patterns", &audioPatterns);
MenuItem menuItemPatterns = MenuItem((char*)"Patterns", &patterns);
MenuItem menuItemAnimations = MenuItem((char*)"Animations", &animations);
MenuItem menuItemSettings = MenuItem((char*)"Settings", &settings);

// Main Menu
MenuItem* mainMenuItems [] = {
  &menuItemAudioPatterns,
  &menuItemPatterns,
  &menuItemAnimations,
  &menuItemSettings,
};

int mainMenuItemCount;

time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}

void setup()
{
  // Setup serial interface
  Serial.begin(115200);

  delay(250);
  // Serial.println(F("starting..."));

  // Initialize the IR receiver
  irReceiver.enableIRIn();

  // Initialize 32x32 LED Matrix
  matrix.begin();
  matrix.setRotation(rotation);
  matrix.setBrightness(brightness);
  matrix.setColorCorrection(cc24);
  matrix.setFont(gohufont11b);
  matrix.setScrollStartOffsetFromLeft(8);
  matrix.setScrollOffsetFromTop(25);
  matrix.setScrollSpeed(80);
  matrix.setScrollMode(wrapForward);
  matrix.fillScreen(rgb24 { 0, 0, 0 });

  matrix.swapBuffers();

  pinMode(SD_CARD_CS, OUTPUT);
  sdAvailable = SD.begin(SD_CARD_CS);
  if (sdAvailable) {
    animations.setup((char *)"/gifs/");
    messagePlayer.setup((char *)"/messages/");
  }

  // setup the effects generator
  effects.Setup();

  InitAudio();

  mainMenuItemCount = sizeof(mainMenuItems) / sizeof(MenuItem*);

  // initialize realtime clock
  // switch pins to use 16/17 for I2C instead of 18/19
  pinMode(18, INPUT);
  pinMode(19, INPUT);
  CORE_PIN16_CONFIG = (PORT_PCR_MUX(2) | PORT_PCR_PE | PORT_PCR_PS);
  CORE_PIN17_CONFIG = (PORT_PCR_MUX(2) | PORT_PCR_PE | PORT_PCR_PS);

  if (sdAvailable) {
    loadRemotesSetting();
    loadRotationSetting();

    loadDemoModeSetting();

    if (demoMode == 0) {
      loadSettings();
    }
    else {
      saveSettings();
      menuItemSettings.visible = false;
    }

    applyDemoMode();
  }

  if (!HAS_IR) {
    menu.playMode = Menu::PlaybackState::Autoplay;
    menu.visible = false;
  }

  menuItemAudioPatterns.audioScaleEnabled = true;
  menuItemAudioPatterns.playModeEnabled = true;
  menuItemAudioPatterns.paletteEnabled = true;

  menuItemPatterns.playModeEnabled = true;
  menuItemPatterns.paletteEnabled = true;

  menuItemAnimations.visible = sdAvailable && animations.imageCount > 0;
  menuItemAnimations.playModeEnabled = true;

  clockDisplay.readTime();
  // Serial.print(F("isTimeAvailable: "));
  // Serial.println(isTimeAvailable);
}

void loop()
{
  menu.run(mainMenuItems, mainMenuItemCount);
}

void listPatterns() {
  patterns.listPatterns();
}

bool setPattern(String name) {
  if (patterns.setPattern(name)) {
    menu.currentIndex = 1;
    menu.visible = false;
    return true;
  }
  return false;
}

bool setPattern(int index) {
  if (patterns.setPattern(index)) {
    menu.currentIndex = 1;
    menu.visible = false;
    return true;
  }
  return false;
}

void listAnimations() {
  animations.listFiles();
}

bool setAnimation(String name) {
  if (animations.setAnimation(name)) {
    menu.currentIndex = 2;
    menu.visible = false;
    return true;
  }
  return false;
}

bool setAnimation(int index) {
  if (animations.setAnimation(index)) {
    menu.currentIndex = 2;
    menu.visible = false;
    return true;
  }

  return false;
}

void powerOff()
{
  // clear the display
  matrix.clearForeground();
  matrix.scrollText("", 1);
  matrix.fillScreen(CRGB(CRGB::Black));
  matrix.swapBuffers();
  matrix.clearForeground();
  matrix.displayForegroundDrawing(false);

  while (true) {
    InputCommand command = readCommand();
    if (command == InputCommand::Power ||
        command == InputCommand::CycleBrightness ||
        command == InputCommand::BrightnessUp ||
        command == InputCommand::BrightnessDown)
      return;

    // go idle for a while, converve power
    delay(500);
  }
}

// Demo mode is loaded separately from other settings, because in demo mode
// we revert to all defaults on startup, all other settings are discarded,
// and the settings menu is hidden.  The 5 button on the Adafruit remote
// can be used to show the settings menu and exit demo mode.
void loadDemoModeSetting() {
  demoMode = loadByteSetting("demomode.txt", 0);
}

// Loads which remote(s) should be enabled. This setting is loaded separately
// from other settings, as this applies moreso in demo mode than any other time.
void loadRemotesSetting() {
  // remotes setting is a bitmask:
  // 1 sparkfun
  // 2 adafruit
  // 3 sparkfun & adafruit
  // 4 smartmatrix
  // 5 sparkfun & smartmatrix
  // 6 adafruit & smartmatrix
  // 7 sparkfun, adafruit & smartmatrix

  byte remotes = loadByteSetting("remotes.txt", 7);

  Serial.print(F("remotes setting is "));
  Serial.println(remotes);

  sparkfunRemoteEnabled = (remotes & 1) == 1;
  adafruitRemoteEnabled = (remotes & 2) == 2;
  smartMatrixRemoteEnabled = (remotes & 4) == 4;

  Serial.print(F("sparkfun remote is "));
  Serial.println(sparkfunRemoteEnabled ? F("enabled") : F("disabled"));

  Serial.print(F("adafruit remote is "));
  Serial.println(adafruitRemoteEnabled ? F("enabled") : F("disabled"));

  Serial.print(F("smartmatrix remote is "));
  Serial.println(smartMatrixRemoteEnabled ? F("enabled") : F("disabled"));

  // if no remotes are enabled, fall back and enable them all
  if (!sparkfunRemoteEnabled && !adafruitRemoteEnabled && !smartMatrixRemoteEnabled) {
    Serial.println(F("enabling all remotes"));
    sparkfunRemoteEnabled = true;
    adafruitRemoteEnabled = true;
    smartMatrixRemoteEnabled = true;
  }
}

void loadRotationSetting() {
  byte rotationIndex = loadByteSetting("rotation.txt", 0);

  switch (rotationIndex) {
    case 0:
    default:
      rotation = rotation0;
      break;

    case 1:
      rotation = rotation90;
      break;

    case 2:
      rotation = rotation180;
      break;

    case 3:
      rotation = rotation270;
      break;
  }

  matrix.setRotation(rotation);
}

void loadSettings() {
  brightness = loadByteSetting("brghtnss.txt", 255);
  boundBrightness();
  matrix.setBrightness(brightness);

  backgroundBrightness = loadByteSetting("bckbrght.txt", 63);
  boundBackgroundBrightness();
  matrix.setBackgroundBrightness(backgroundBrightness);

  audioScale = loadByteSetting("audioscl.txt", 0);
  boundAudioScale();

  menuColor.red = loadByteSetting("menuR.txt", 0);
  menuColor.green = loadByteSetting("menuG.txt", 0);
  menuColor.blue = loadByteSetting("menuB.txt", 255);

  autoPlayDurationSeconds = loadIntSetting("autoplyd.txt", 3, 10);

  settings.load();

  clockDisplay.loadSettings();
}

void saveSettings() {
  saveAudioScaleSetting();
  saveBrightnessSetting();
  saveBackgroundBrightnessSetting();
  saveMenuColor();
  saveAutoPlayDurationSeconds();
  clockDisplay.saveSettings();
}

int getBrightnessLevel() {
  int level = 0;
  for (int i = 0; i < brightnessCount; i++) {
    if (brightnessMap[i] >= brightness) {
      level = i;
      break;
    }
  }
  return level;
}

int getBackgroundBrightnessLevel() {
  int level = 0;
  for (int i = 0; i < brightnessCount; i++) {
    if (backgroundBrightnessMap[i] >= backgroundBrightness) {
      level = i;
      break;
    }
  }
  return level;
}

void adjustBrightness(int delta, boolean wrap) {
  int level = getBrightnessLevel();

  level += delta;
  if (level < 0)
    level = wrap ? brightnessCount - 1 : 0;
  if (level >= brightnessCount)
    level = wrap ? 0 : brightnessCount - 1;

  brightness = brightnessMap[level];
  boundBrightness();
  matrix.setBrightness(brightness);
}

uint8_t cycleBrightness() {
  adjustBrightness(1, true);
  saveBrightnessSetting();

  if (brightness == brightnessMap[0])
    return 0;

  return brightness;
}

void adjustBackgroundBrightness(int d) {
  int level = 0;
  for (int i = 0; i < brightnessCount; i++) {
    if (backgroundBrightnessMap[i] >= backgroundBrightness) {
      level = i;
      break;
    }
  }

  level += d;
  if (level < 0)
    level = brightnessCount - 1;
  if (level >= brightnessCount)
    level = 0;

  backgroundBrightness = backgroundBrightnessMap[level];
  boundBackgroundBrightness();
  matrix.setBackgroundBrightness(backgroundBrightness);
}

void boundBrightness() {
  if (brightness < brightnessMap[0])
    brightness = brightnessMap[0];
  else if (brightness > brightnessMap[brightnessCount - 1])
    brightness = brightnessMap[brightnessCount - 1];
}

void boundBackgroundBrightness() {
  if (backgroundBrightness < backgroundBrightnessMap[0])
    backgroundBrightness = backgroundBrightnessMap[0];
  else if (backgroundBrightness > backgroundBrightnessMap[brightnessCount - 1])
    backgroundBrightness = backgroundBrightnessMap[brightnessCount - 1];
}

void adjustDemoMode(int delta) {
  if (delta < 1) {
    if (demoMode <= 0)
      demoMode = 6;
    else
      demoMode--;
  }

  if (delta > 0) {
    if (demoMode >= 6)
      demoMode = 0;
    else
      demoMode++;
  }

  menuItemSettings.visible = demoMode == 0;
  applyDemoMode();
}

void applyDemoMode() {
  if (demoMode != 0) {
    menu.visible = false;
    menu.clockVisible = true;

    switch (demoMode) {
      case 1: // autoplay audio patterns
        menu.currentIndex = 0;
        menu.playMode = Menu::PlaybackState::Autoplay;
        break;

      case 2:
        menu.currentIndex = 0;
        menu.playMode = Menu::PlaybackState::Random;
        break;

      case 3:
        menu.currentIndex = 1;
        menu.playMode = Menu::PlaybackState::Autoplay;
        break;

      case 4:
        menu.currentIndex = 1;
        menu.playMode = Menu::PlaybackState::Random;
        break;

      case 5:
        menu.currentIndex = 2;
        menu.playMode = Menu::PlaybackState::Autoplay;
        break;

      case 6:
        menu.currentIndex = 2;
        menu.playMode = Menu::PlaybackState::Random;
        break;
    }
  }
}

void saveBrightnessSetting() {
  saveByteSetting("brghtnss.txt", brightness);
}

void saveBackgroundBrightnessSetting() {
  saveByteSetting("bckbrght.txt", backgroundBrightness);
}

void saveMenuColor() {
  saveMenuR();
  saveMenuG();
  saveMenuB();
}

void saveMenuR() {
  saveByteSetting("menuR.txt", menuColor.red);
}

void saveMenuG() {
  saveByteSetting("menuG.txt", menuColor.green);
}

void saveMenuB() {
  saveByteSetting("menuB.txt", menuColor.blue);
}

void saveAutoPlayDurationSeconds() {
  saveIntSetting("autoplyd.txt", autoPlayDurationSeconds);
}

void saveDemoMode() {
  saveByteSetting("demomode.txt", demoMode);
}

int loadIntSetting(const char* name, uint8_t maxLength, int defaultValue) {
  if (!sdAvailable)
    return defaultValue;

  int intValue = defaultValue;

  char* path = (char *) "/aurora/";

  if (!SD.exists(path)) {
    SD.mkdir(path);
  }

  char filepath[255];
  strcpy(filepath, path);
  strcat(filepath, name);

  //    Serial.print("loading ");
  //    Serial.println(filepath);

  File file = SD.open(filepath, FILE_READ);
  if (file) {
    String value;
    char c = file.read();
    int length = 1;
    while (c >= 0 && length <= maxLength) {
      value.append(c);
      c = file.read();
      length++;
    }
    file.close();
    intValue = value.toInt();
  }

  return intValue;
}

int loadByteSetting(const char* name, byte defaultValue) {
  if (!sdAvailable)
    return defaultValue;

  uint8_t maxLength = 3;

  byte byteValue = defaultValue;

  char* path = (char *) "/aurora/";

  if (!SD.exists(path)) {
    SD.mkdir(path);
  }

  char filepath[255];
  strcpy(filepath, path);
  strcat(filepath, name);

  File file = SD.open(filepath, FILE_READ);
  if (file) {
    String value;
    char c = file.read();
    int length = 1;
    while (c >= 0 && length <= maxLength) {
      value.append(c);
      c = file.read();
      length++;
    }
    file.close();
    byteValue = (byte) value.toInt();
  }

  return byteValue;
}

void saveIntSetting(const char* name, int value) {
  if (!sdAvailable)
    return;

  char* path = (char *) "/aurora/";

  if (!SD.exists(path)) {
    SD.mkdir(path);
  }

  char filepath[255];
  strcpy(filepath, path);
  strcat(filepath, name);

  Serial.print("saving ");
  Serial.println(filepath);

  File file = SD.open(filepath, O_CREAT | O_TRUNC | O_WRITE);
  if (file) {
    file.print(value, 10);
    file.close();
  }
}

void saveByteSetting(const char* name, byte value) {
  if (!sdAvailable)
    return;

  char* path = (char *) "/aurora/";

  if (!SD.exists(path)) {
    SD.mkdir(path);
  }

  char filepath[255];
  strcpy(filepath, path);
  strcat(filepath, name);

  Serial.print("saving ");
  Serial.println(filepath);

  File file = SD.open(filepath, O_CREAT | O_TRUNC | O_WRITE);
  if (file) {
    file.print(value, 10);
    file.close();
  }
}

void toggleSettingsMenuVisibility() {
  menuItemSettings.visible = !menuItemSettings.visible;
}

// translates from x, y into an index into the LED array
uint16_t XY(uint8_t x, uint8_t y) {
  uint8_t hwx, hwy;

  // map pixel into hardware buffer before writing
  if (rotation == rotation0) {
    hwx = x;
    hwy = y;
  } else if (rotation == rotation180) {
    hwx = (MATRIX_WIDTH - 1) - x;
    hwy = (MATRIX_HEIGHT - 1) - y;
  } else if (rotation == rotation90) {
    hwx = (MATRIX_WIDTH - 1) - y;
    hwy = x;
  } else { /* if (screenConfig.rotation == rotation270)*/
    hwx = y;
    hwy = (MATRIX_HEIGHT - 1) - x;
  }

  if (hwy >= MATRIX_HEIGHT) {
    hwy = MATRIX_HEIGHT - 1;
  }
  if (hwy < 0) {
    hwy = 0;
  }
  if (hwx >= MATRIX_WIDTH) {
    hwx = MATRIX_WIDTH - 1;
  }
  if (hwx < 0) {
    hwx = 0;
  }

  return (hwy * MATRIX_WIDTH) + hwx;
}

/////////////////////////////////////////////////////////////
// the assembly code must be run inside a C, not C++ function
#ifdef __cplusplus
extern "C" {
#endif
void jumpToApplicationAt0x38080() {
  /* Load stack pointer and program counter from start of new program */
  asm("movw r0, #0x8080");
  asm("movt r0, #0x0003");
  asm("ldr sp, [r0]");
  asm("ldr pc, [r0, #4]");
}

void jumpToApplicationAt0x8080() {
  /* Load stack pointer and program counter from start of new program */
  asm("movw r0, #0x8080");
  asm("ldr sp, [r0]");
  asm("ldr pc, [r0, #4]");
}

/*
 * These are the minimum peripherals that needed to be disabled to allow the
 * uTasker USB-MSD application to work.  You may need to reset more peripherals
 * depending on the application you are running, and what other peripherals
 * your sketch uses if you add more to this example than just blinking an LED
*/
void resetPeripherals() {
  /* set (some of) USB back to normal */
  NVIC_DISABLE_IRQ(IRQ_USBOTG);
  NVIC_CLEAR_PENDING(IRQ_USBOTG);
  SIM_SCGC4 &= ~(SIM_SCGC4_USBOTG);

  /* disable all GPIO interrupts */
  NVIC_DISABLE_IRQ(IRQ_PORTA);
  NVIC_DISABLE_IRQ(IRQ_PORTB);
  NVIC_DISABLE_IRQ(IRQ_PORTC);
  NVIC_DISABLE_IRQ(IRQ_PORTD);
  NVIC_DISABLE_IRQ(IRQ_PORTE);

  /* set (some of) ADC1 back to normal */
  // wait until calibration is complete
  while (ADC1_SC3 & ADC_SC3_CAL);

  // clear flag if calibration failed
  if (ADC1_SC3 & 1 << 6)
    ADC1_SC3 |= 1 << 6;

  // clear conversion complete flag (which could trigger ISR otherwise)
  if (ADC1_SC1A & 1 << 7)
    ADC1_SC1A |= 1 << 7;

  /* set some clocks back to default/reset settings */
  MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(4);
  SIM_CLKDIV1 = 0;
  SIM_CLKDIV2 = 0;
}

void startup_late_hook(void) {
  // look for the condition that indicates we want to jump to the application with offset
  if (eeprom_read_byte(0) == 0xAE) {

    // clear the condition
    eeprom_write_byte(0, 0);

    // set peripherals (mostly) back to normal then jump
    __disable_irq();
    resetPeripherals();
    jumpToApplicationAt0x38080();
  }
}
#ifdef __cplusplus
}
#endif

#define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL 0x5FA0004
#define CPU_RESTART() (*CPU_RESTART_ADDR = CPU_RESTART_VAL);

extern void restartAndJumpToApp(void) {
  eeprom_write_byte(0, 0xAE);

  CPU_RESTART();
}
