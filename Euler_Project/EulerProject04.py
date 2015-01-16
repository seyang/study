#!/usr/bin/env python2

maxnum = 0

for i in range(100,1000):
	for j in range(100,1000):
		if str(i*j) == str(i*j)[::-1]:
			if (i*j) > maxnum:
				maxnum = i*j
	
print maxnum


