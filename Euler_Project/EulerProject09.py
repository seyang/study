#!/usr/bin/env python2

for num1 in range(1,1000):
	for num2 in range(num1,1000):
		for num3 in range(num2,1000):
			if (num1+num2+num3) == 1000:
				if pow(num1,2)+pow(num2,2) == pow(num3,2):
					print num1*num2*num3
