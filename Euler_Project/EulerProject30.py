#!/usr/bin/env python2
num = 2
result = 0

while 1:
	chk_num = 0
	for i in str(num):
		chk_num += pow(int(i),5)
	if chk_num == num:
		result += chk_num
		print result
	num += 1
		
