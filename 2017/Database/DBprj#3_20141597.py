#-*- coding: utf-8 -*-
import datetime
import time
import sys
import MeCab
from pymongo import MongoClient

def printMenu():
    print "1. WordCount"
    print "2. TF-IDF"
    print "3. Similarity"
    print "4. MorpAnalysis"
    print "5. CopyData"

def MorpAnalysis(docs, col_tfidf):
    print "MorpAnalysis"

    # call Morphological analyzer
    t = MeCab.Tagger('-d/usr/local/lib/mecab/dic/mecab-ko-dic')

    # read stopword
    stop_word = {}
    f = open("wordList.txt", 'r')
    while True:
        line = f.readline()
        if not line: break
        stop_word[line.strip('\n')] = line.strip('\n')
    f.close()

    id_input = raw_input('Insert Object ID: ')

    # Analysis Morpheme and Remove stopword
    for doc in docs:
        content = doc['content']
        nodes = t.parseToNode(content.encode('utf-8'))

        MorpList = []
        while nodes:
            if nodes.feature[0] == 'N' and nodes.feature[1] == 'N':
                w = nodes.surface
                if not w in stop_word:
                    MorpList.append(w)
            nodes = nodes.next

        # save at MongoDB
        contentDic = {}
        for key in doc.keys():
            contentDic[key] = doc[key]
        contentDic['morp'] = MorpList
        col_tfidf.update({'_id':contentDic['_id']}, contentDic, True)
        
        # print Morplist at input ID
        if str(contentDic['_id']) == str(id_input):
            for morp in contentDic['morp']:
                print morp

def WordCount(docs, col_tfidf):
    print "WordCount"

    if 'morp' not in docs[0].keys():
        print "You have to do '4. MorpAnalysis'"
    else:
        id_input = raw_input('Insert Object ID: ')
        
        # Count Word
        for doc in docs:
            WordCount = {}

            for morp in doc['morp']:
                if WordCount.get(morp) == None:
                    WordCount[morp] = 1
                else:
                    WordCount[morp] += 1
            
            # save at MongoDB
            contentDic = {}
            for key in doc.keys():
                contentDic[key] = doc[key]
            contentDic['WordCount'] = WordCount
            col_tfidf.update({'_id':contentDic['_id']}, contentDic, True)
            
            # print WordCountlist at input ID
            if str(contentDic['_id']) == str(id_input):
                for word, count in contentDic['WordCount'].items():
                    print word, ':', count

        
def TfIdf(docs, col_tfidf):
    print "TF-IDF"

    if 'morp' not in docs[0].keys():
        print "You have to do '4. MorpAnalysis'"
    elif 'WordCount' not in docs[0].keys():
        print "You have to do '1. WordCount'"
    else:
        id_input = raw_input('Insert Object ID: ')

        # Calculate idf
        idf = {}
        for doc in docs:
            for word in doc['WordCount'].keys():
                if idf.get(word) == None:
                    idf[word] = 1
                else:
                    idf[word] += 1

        import math
        docs = col_tfidf.find()

        # Calculate tf, and multiplicate with idf
        # Assign Tf-Idf in TfIdf dictionary
        for doc in docs:
            TfIdf = {}
            N = sum(doc['WordCount'].values())
            
            for word, count in doc['WordCount'].items():
                TfIdf[word] = (float(count) / N) * math.log(float(docs.count()) / idf[word])
                
            # save at MongoDB
            contentDic = {}
            for key in doc.keys():
                contentDic[key] = doc[key]
            contentDic['TfIdf'] = TfIdf
            col_tfidf.update({'_id': contentDic['_id']}, contentDic, True)

            # print tfidf top 10
            if str(contentDic['_id']) == str(id_input):
                import operator
                sorted_tfidf = sorted(contentDic['TfIdf'].items(), key=operator.itemgetter(1), reverse = True)

                for i in range(10):
                    print sorted_tfidf[i][0], ':', sorted_tfidf[i][1]


def Similarity(docs, col_tfidf):
    print "Similarity"

    if 'morp' not in docs[0].keys():
        print "You have to do '4. MorpAnalysis'"
    elif 'WordCount' not in docs[0].keys():
        print "You have to do '1. WordCount'"
    elif 'TfIdf' not in docs[0].keys():
        print "You have to do '2. TfIdf'"
    else:
        id_input1 = raw_input('Insert Object ID(1): ')
        id_input2 = raw_input('Insert Object ID(2): ')
        
        # check 2 IDs
        f = 0
        for doc in docs:
            if str(doc['_id']) == str(id_input1):
                doc1 = doc
                f += 1
            if str(doc['_id']) == str(id_input2):
                doc2 = doc
                f += 1

        if f != 2:
            print "ID matching Error!"
            return

        # make A-tfidf and B-tfidf
        tfidf1 = {}
        tfidf2 = {}
        for word, tfidf in doc1['TfIdf'].items():
            tfidf1[word] = tfidf
        for word, tfidf in doc2['TfIdf'].items():
            if tfidf1.get(word) == None:
                tfidf1[word] = 0.0
            tfidf2[word] = tfidf
        for word, tfidf in doc1['TfIdf'].items():
            if tfidf2.get(word) == None:
                tfidf2[word] = 0.0

        # Calculate numerator
        sim = 0
        for word1, value1 in tfidf1.items():
            for word2, value2 in tfidf2.items():
                if word1 == word2:
                    sim += value1 * value2

        # Calculate denominator
        A = 0
        for value in tfidf1.values():
            A += value * value
        B = 0
        for value in tfidf2.values():
            B += value * value

        import math
        A = math.sqrt(A)
        B = math.sqrt(B)

        # calculate Simularity and print it
        sim = float(sim) / (A*B)

        print sim


    

def copyData(docs, col_tfidf):
    col_tfidf.drop()
    for doc in docs:
        contentDic = {}
        for key in doc.keys():
            if key != "_id":
                contentDic[key] = doc[key]
        col_tfidf.insert(contentDic)

conn = MongoClient('localhost')
db = conn['db20141597']
db.authenticate('db20141597', 'db20141597')
col = db['news']
col_tfidf = db['news_tfidf']

printMenu()
selector = input()

docs = col_tfidf.find()

if selector == 1:
    WordCount(docs, col_tfidf)
elif selector == 2:
    TfIdf(docs, col_tfidf)
elif selector == 3:
    Similarity(docs, col_tfidf)
elif selector == 4:
    MorpAnalysis(docs, col_tfidf)
elif selector == 5:
    docs = col.find()
    copyData(docs, col_tfidf)

