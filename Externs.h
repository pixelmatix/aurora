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

extern MessagePlayer messagePlayer;
extern IRrecv irReceiver;
extern bool setPattern(String name);
extern bool setAnimation(String name);
extern void listAnimations();
extern void listPatterns();
extern uint8_t brightness;
extern uint8_t backgroundBrightness;
extern Effects effects;

#endif /* EXTERNS_H_ */
