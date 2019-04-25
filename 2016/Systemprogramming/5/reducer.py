#!/usr/bin/python
# -*- coding: utf-8 -*-

from sys import stdin, stdout

# make dictionary
library = {}

while True:
	# read line one by one
	line = stdin.readline().strip()

	# if line is empty, it means EOF
	if line == "":
		break
	
	# cut the string by whitespace
	words = line.split()

	# word means "word1	word2"
	word = words[0] + '\t' + words[1]

	# cnt means number of the set of two words
	cnt = words[2]

	# if the set of words is in dictionary(library), 
	# cnt = cnt + 1
	# else, make dictionary about the set
	if word in library:
		library[word] += int(cnt)
	else:
		library[word] = int(cnt)

# print sorted dictionary by format
for i in sorted(library):
	stdout.write(i + '\t' + str(library[i]) + '\n')
