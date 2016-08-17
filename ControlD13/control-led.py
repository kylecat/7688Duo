#!/usr/bin/env python

# control led 13

import serial
import time

myserial = None
ledStatus = 0

def commandInput():
	global ledStatus
	ledStatus = input("D13 LED is (0/1)")

def led():
	global ledStatus
	if ledStatus:
		ledStatus = 1
		myserial.write("on")
		print("D13 on")
	else:
		ledStatus = 0
		myserial.write("off")
		print("D13 off")

def setup():
	global myserial
	# open serial COM port to /dev/ttyS0, which maps to UART0(D0/D1)
	# the baudrate is set to 57600 and should be the same as the one
	# specified in the Arduino sketch uploaded to ATmega32U4.
	myserial = serial.Serial("/dev/ttyS0", 57600)

def main():
	for i in range(1,11):
		print ("")
		print ( "%s.Led State:%s" ) % ( i, ledStatus)
		commandInput()
		led()
		time.sleep(0.5)

if __name__ == "__main__":
	setup()
	while True:
		main()