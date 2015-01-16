#!/usr/bin/env python2

cnt = 2
num1 = 1
num2 = 1
num3 = 0

while 1:
	num3 = num1 + num2
	cnt = cnt + 1
	if len(str(num3)) >= 1000:
		print cnt
		break
	num1 = num2
	num2 = num3
