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
 *******/
 
#ifndef MYBUTTSTUFF_H
#define MYBUTTSTUFF_H

struct buttstuff {
  int buttpin;
  unsigned long lastdebounce;
  unsigned long mono;
  unsigned long holding;
  bool wastouched;
  bool tchanged; 
  bool pressed;
  bool released; 
};

void buttsetup(int pin, int pull);
void buttloop();
bool buttheldfor(unsigned long msecs, bool reset);
#endif /* MYBUTTSUFF_H */
