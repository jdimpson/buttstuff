# buttstuff
buttstuff is a button library for arduino and circuitpython that makes it easy to detect when momentary switches are held for specific lengths of time, and to avoid unwanted repetition when a button is held down.

## arduino example

To use the arduino code, open buttstuff.cpp and buttstuff.h with Sketch->Add File in the Arduino IDE. 

Then use it in your main code like this:

#include "buttstuff.h"
extern struct mybuttstuff mbs;
void setup() {
	int butpin = 18;
	int pull = INPUT_PULLUP; // or INPUT_PULLDOWN or INPUT, as appropriate
	buttsetup(butpin, pull);
	// ...
}

void loop() {
	buttloop(); // must be called once per loop
	// ...
	int msecs = 50;
	bool reset = true;
	if (buttheldfor(msecs, reset)) {
		// do whatever you want when the button is pushed for at least msec milliseconds
	}
}

Not well tested with reset = False or with pulled down pins.


## circuitpython example
This is just the __main__ section of the python file:


if __name__ == "__main__":
	import board
	from time import sleep
	from touchio import TouchIn
	from digitalio import DigitalInOut, Direction

	led = DigitalInOut(board.D13)
	led.direction = Direction.OUTPUT
	mode = "run"
	holdsecs = 5
	touch2 = TouchIn(board.A2)
	bs = buttstuff(touch2)
	i=0
	while True:
		bs.loop()
		led.value = bs.button.value
		#print(mode)
		if bs.heldfor(holdsecs,reset=True):
			if   mode == "run":
				print("CHANGED TO programming")
				mode = "program"
			elif mode == "program":
				print("CHANGED TO running")
				mode = "run"

		i = (i+1) % 256  # run from 0 to 255
		sleep(0.1)
