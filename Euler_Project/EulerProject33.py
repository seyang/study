#!/usr/bin/env python2
"""
The fraction 49/98 is a curious fraction, as an inexperienced mathematician in attempting to simplify it may incorrectly believe that 49/98 = 4/8, which is correct, is obtained by cancelling the 9s.

We shall consider fractions like, 30/50 = 3/5, to be trivial examples.

There are exactly four non-trivial examples of this type of fraction, less than one in value, and containing two digits in the numerator and denominator.

If the product of these four fractions is given in its lowest common terms, find the value of the denominator.
"""

def drange(start, stop, step):
	r = start
	while r < stop:
		yield r
		r += step

for i in drange(10.0,100.0,1.0):
	for j in drange(10.0,i,1.0):
		if float(str(j)[0]) == float(str(i)[0]):
			if float(str(i)[1]) == 0:
				continue
			if float(str(j)[1])/float(str(i)[1]) == j/i:
				print "%f/%f, %f/%f"%(float(str(j)[1])/float(str(i)[1]), j/i, j, i)
		if float(str(j)[0]) == float(str(i)[1]):
			if float(str(i)[0]) == 0:
				continue
			if float(str(j)[1])/float(str(i)[0]) == j/i:
				print "%f/%f, %f/%f"%(float(str(j)[1])/float(str(i)[0]), j/i, j, i)
		if float(str(j)[1]) == float(str(i)[0]):
			if float(str(i)[1]) == 0:
				continue
			if float(str(j)[0])/float(str(i)[1]) == j/i:
				print "%f/%f, %f/%f"%(float(str(j)[0])/float(str(i)[1]), j/i, j, i)
		if float(str(j)[1]) == float(str(i)[1]):
			if int(str(j)[1]) == 0 & int (str(i)[1]):
				continue
			if float(str(i)[0]) == 0:
				continue
			if float(str(j)[0])/float(str(i)[0]) == j/i:
				print "%f/%f, %f/%f"%(float(str(j)[0])/float(str(i)[0]), j/i, j, i)
