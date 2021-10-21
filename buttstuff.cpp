
/*******
 * Copyright 2021 Jeremy D. Impson <jdimpson at acm.org>
 * buttstuff is a button library for arduino  and circuitpython 
 * that makes it easy to detech when momentary switches are held 
 * for specific lengths of time, and to avoid unwanted repeating
 * repetition when a button is held down.
 * 
 * This software is available to be used under the terms described
 * in what is commonly knownas the MIT License, which can be found
 * in the file LICENSE.
 * 
 * To use the arduino code, open buttstuff.cpp and buttstuff.h
 * with Sketch->Add File in the Arduino IDE. Then in your main code:
 *      include buttstuff.h
 *      declare "extern struct buttstuff mbs;" as a global variable, 
 *      call buttsetup(pin, pull) in setup(), where pin is the digital 
 *          pin with your button, and pull is INPUT, INPUT_PULLUP, or 
 *          INPUT_PULLDOWN as appropriate
 *      call buttloop() in loop() before you want to check the button
 *      call buttheldfor(msecs, reset) to find out if the button was 
 *          pressed. msecs is the debounce time to wait before deciding 
 *          the button was pressed, and reset is a boolean that determines 
 *          whether the next check of the pin should be treated as a different
 *          press or the same press.
 * Not well tested with reset = False or with pulled down pins.
 *******/

#include <Arduino.h>
#include "buttstuff.h"

struct buttstuff bs = {
  0, 0, 0, 0, false, false, false, false
};

void buttsetup(int pin, int pull) {
  pinMode(pin, pull);
  bs.buttpin = pin;
  bs.wastouched = bs.tchanged = false;
  bs.holding = 0;
  bs.mono = 0;
  bs.pressed = bs.released = false;
}

void buttloop() {
  bs.mono = millis();
  bool touched = digitalRead(bs.buttpin) == 0;
  if (touched) {
    if (!bs.wastouched) {
      bs.tchanged = true;
    } else {
      bs.tchanged = false;
    }
  }else {
    if (bs.wastouched) {
      bs.tchanged = true;
    } else {
      bs.tchanged = false;
    }
  }
  bs.wastouched = touched;
  bs.pressed = (bs.tchanged && touched);
  bs.released = (bs.tchanged && !touched);
  if ((bs.holding > 0) && !touched) {
    bs.holding = 0;
  }
  if (bs.pressed) {
    if (bs.holding == 0) {
      bs.holding = bs.mono;
    }
  }
  if (bs.released) {
    bs.holding = 0;
  }
}

bool buttheldfor(unsigned long msecs, bool reset) {
  bool bo = (bs.holding > 0) && (bs.holding + msecs < bs.mono);
  if (bo && reset) {
    bs.holding = 0;
  }
  return bo;
}
