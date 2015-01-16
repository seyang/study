#!/usr/bin/env python2

def factors(n):    
	return set(reduce(list.__add__, 
		([i, n//i] for i in range(1, int(n**0.5) + 1) if n % i == 0)))

num = 1
t_num = 0 # triangle number
while 1:
	t_num += num
	num += 1
	if len(factors(t_num)) > 500:
		print t_num
		break


