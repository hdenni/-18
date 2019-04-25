execfile('20141597.conf')

from boto.dynamodb2.table import Table
from sys import stdin, stdout

# dynamoTable is the table in DynamoDB of AWS
dynamoTable = Table('project6')

while True:
	# input word 
	str = stdin.readline().strip()

	# if word is None, exit the program
	if str == '':
		break

	# find word in table
	word = dynamoTable.get_item(words=str)
	
	# if word does not exist, print zero(number of the word)
	if word['counts'] == None :
		print 0
	# else, print number of the word
	else:
		print(word['counts'])
