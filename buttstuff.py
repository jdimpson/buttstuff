#######
# Copyright 2021 Jeremy D. Impson <jdimpson at acm dot org>
# buttstuff is a button library for arduino  and circuitpython
# that makes it easy to detect when momentary switches are held 
# for specific lengths of time, and to avoid unwanted repetition 
# when a button is held down.
# 
# This software is available to be used under the terms described
# in what is commonly knownas the MIT License, which can be found
# in the file LICENSE.
#######

from time import monotonic

class buttstuff(object):
	def __init__(self, button):
		self.button = button
		self.wastouched = self.tchanged = False
		self.holding = 0
		self.mono = 0
		self.pressed = self.released = False

	def loop(self, debug=False):
		self.mono = monotonic()

		touched = bool(self.button.value)
		if touched:
			if self.wastouched is False:
				tchanged = True
			else: 
				tchanged = False
		else:
 			if self.wastouched is True:
				tchanged = True
			else: 
				tchanged = False
		self.wastouched = touched

		self.pressed = tchanged and touched
		self.released = tchanged and not touched

		if self.holding > 0 and not touched:
			self.holding = 0

		if self.pressed:
			if debug: print("pressed")
			if self.holding == 0:
				self.holding = self.mono

		if self.holding and debug:
			print("holding")

		if self.released:
			if debug: print("released")
			self.holding = 0
		return

	def heldfor(self, s, reset=False):
		bo = self.holding > 0 and self.holding + s < self.mono
		if bo and reset:
			self.holding = 0
		return bo

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
