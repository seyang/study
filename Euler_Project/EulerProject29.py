#!/usr/bin/env python2
import math

t = []

for i in range(2,101):
	for j in range(2,101):
		t.append(pow(i, j))

print len(set(t))

