# global
a = 0
url = open('URL.txt', 'w')
urltmp = 0

text = []
root = "http://cspro.sogang.ac.kr/~gr120160213"

def f(x):
	global urltmp
	global a
	global url
	global text
	global main

	r = requests.get(x)

	# If page is not found, do not write URL
	if r.status_code != 200:
		return

	# If the page is already visited, do not write URL (compare content)
	if text == []:
		text.append(r.content)
	else:
		for i in text:
			if i == r.content:
				return
		text.append(r.content)


	# Write URL.txt
	if urltmp == 0:
		url.write(x)
		urltmp = 1
	else:
		url.write('\n')
		url.write(x)
	
	# Set Output File format
	b = format(a, '04d')
	a = a+1
	c = 'Output_'+b+'.txt'
	
	# Read contents of page and check tag
	soup = BeautifulSoup(r.content, "html.parser")
	
	# Open & Read Output file
	fp = open(c, 'w')	
	text_file = soup.getText()
	fp.write(str(text_file))
	fp.close()

	# Read URL from code of the page
	for i in soup.find_all('a'):
		link = str(i.get('href'))
		if link[0:1] == '#' or link[0:1] == '?':
			continue
		if link == '':
			continue
		if len(link) < 4:
			f(root+'/'+link)
		else:
			if link[0:4] == 'http':
				f(link)
			else:
				f(root+'/'+link)
	
	return 0

# main
import requests
from bs4 import BeautifulSoup

f(root)

url.close()
