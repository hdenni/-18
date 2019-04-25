execfile('20141597.conf')

from boto.dynamodb2.table import Table
from boto.s3.connection import S3Connection

s3 = S3Connection()
bucket = s3.get_bucket('20141597tokyo')

dynamoTable = Table('project6')

temp = bucket.list()
for i in temp : 
	if 'project5output' in i.key and 'part-' in i.key : 
		content  = i.get_contents_as_string()
		for line in content.split('\n') : 
			try : words, counts = line.split('\t')
			except : pass
			else : 
				try : dynamoTable.put_item(data={'words' : words, 'counts' : counts})
				except : pass
