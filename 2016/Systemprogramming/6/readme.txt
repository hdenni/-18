# DynamoDB_Import_S3.py
20141597tokyo bucket 안에 있는 project5output 폴더 안의 part00000~part00006 파일을 입력받아 DynamoDB 안의 project6 Table에 저장한다. Table 안에는 words, 단어의 조합과 파일들 안에서 단어가 출력된 갯수인 counts가 저장되어 있다.
# DYnamoDB_Query_CLI.py
AWS의 DynamoDB 안에 있는 project6 table의 내용을 DynamoTable에 저장한다. 입력은 공백을 입력받을 때까지 계속 입력받으며, 입력받은 단어가 Table안에 존재하지 않을 시 0을 출력한다. 즉, Table안에 단어가 0개 존재한다는 뜻이다. Table안에 단어가 존재 할 경우, 그에 따른 개수를 출력한다. 
