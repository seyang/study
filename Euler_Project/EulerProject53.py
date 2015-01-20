#!/usr/bin/env python2
import Euler

def nCr(n, r):
	return Euler.factorial(n) / (Euler.factorial(r) * Euler.factorial(n-r))

cnt = 0
for i in range(1,101):
	for j in range(1,i+1):
		if nCr(i,j) > 1000000:
			cnt += 1

print cnt
