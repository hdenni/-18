#!/usr/bin/python
# -*- coding: utf-8 -*-

from sys import stdin, stdout

while True:
	# read line one by one
	line = stdin.readline().strip()

	# if line is empty, it means EOF
	if line=='':
		break
	
	# cut string by whitespace
	words = line.split()

	# print words by format
	for i in range(0, len(words)-1):
		stdout.write(words[i] + '\t' + words[i+1] + '\t1\n')
