#!/usr/bin/env python2
import math

# (Euclidean Algorithm)
def gcd(a, b):
  while (b != 0):
    temp = a % b
    a = b
    b = temp
  return abs(a)

def lcm(a, b):
  gcd_value = gcd(a, b)
  if (gcd_value == 0): return 0 
  return abs( (a * b) / gcd_value )

result = 1
for i in range(1,21):
	result = lcm(result,i)

print result
