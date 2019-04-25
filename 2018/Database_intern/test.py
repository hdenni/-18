# -*- coding: utf-8 -*-
from pq import PriorityQueue
import numpy
import random
from scipy.stats import laplace

def Laplace(g,d):
    for i in g.keys():
        for j in g[i].keys():
            if(g[i][j][2] - g[i][j][1] != 0):
                noise = numpy.random.laplace(0,(1)/(g[i][j][2]-g[i][j][1]), 1000)
            else:
                noise = numpy.random.laplace(0,(1),1000)
            noiseadded[i][j][0] = (g[i][j][0]) + noise[0]
    return noiseadded

def Exponential(ex,g,d):
    qua = {}
    prob = {}
    for i in g.keys():
        for j in g[i].keys():
            if (g[i][j][2] - g[i][j][1]) == 0:
                ex[i][j][0] = g[i][j][1]
                continue
            qua[0] = numpy.exp( d *(-abs((g[i][j][0] - g[i][j][3])))/ (2*(g[i][j][2]-g[i][j][1])))
            qua[1] = numpy.exp( d *(-abs((g[i][j][0] - g[i][j][4])))/ (2*(g[i][j][2]-g[i][j][1])))
            qua[2] = numpy.exp( d *(-abs((g[i][j][0] - g[i][j][5])))/ (2*(g[i][j][2]-g[i][j][1])))
            prob[0] = qua[0] /(qua[0] + qua[1] + qua[2])
            prob[1] = qua[1] /(qua[0] + qua[1] + qua[2])
            prob[2] = qua[2] /(qua[0] + qua[1] + qua[2])
            
            rand = random.random()

           # prob[0] = prob[0] * 100
           # prob[1] = prob[1] * 100
           # prob[2] = prob[2] * 100

            if rand < prob[0]:
                ex[i][j][0] = g[i][j][3]
            elif rand < prob[0]+prob[1]:
                ex[i][j][0] = g[i][j][4]
            else:
                ex[i][j][0] = g[i][j][5]

def Dijkstra(g,gn,s,d):
    #initialization
    dist = {}
    prev = {}

    Q = PriorityQueue()
#    print(g["home"].keys())
    #출발노드 s는 자신까지의 거리가 0이고 자신의 predecessor
    dist[s] = 0
    prev[s] = None

    #다른 노드들은 모두 거리를 infinity로 설정하고 predecessor는 일단 None으로 설정하고
    '''for n in g:
        dist[n[0]] = float('inf')
        prev[n[0]] = None
        dist[n[1]] = float('inf')
        prev[n[1]] = None
    '''
    #그러면서 PQ에다가 노드들을 넣어줍니다.
    for n in g.keys():
        if n != s :
            dist[n] = float('inf')
            prev[n] = None
        
        if n in g[s].keys():
            dist[n] = float('inf')
            prev[n] = None
        Q.insert(dist[n],n) # n이 우선순위 dist가 value
    #PQ가 빌때까지 계속 루프를 돌면서,
    while Q.size() > 0:
        p,u = Q.pop() #현재까지 가장 짧은 거리를 갖고 있는 노드를 pop

        #꺼낸 노드의 각 이웃들까지의 거리를 현재 자신까지의 minimum cost와 더한 후
        #이웃들이 가지고 있는 거리보다 작으면 이것으로 업데이트 시키고 다시 PQ에 넣거나 update합니다
        #pd insert 기능에 포함되어 있다고 한다.
        '''for v in g[u].keys():
#           alt = dist[u] + g[u][v].get('weight',1)
            alt = dist[u] + g[u][v]
            if alt < dist[v]:
                dist[v] = alt
                prev[v] = u
                Q.insert(dist[v],v)       #for v in g.neighbors(u):
        '''
        for v in g[u].keys():
#       alt = dist[u] + g[u][v].get('weight',1)
            alt = dist[u] + int(g[u][v][0])  # distance 계산에 쓰임.
            alt2 = dist[u] + int(gn[u][v][0])  # 경로 선택에 쓰임.
            if alt2 < dist[v]:
                dist[v] = alt
                prev[v] = u
                Q.insert(dist[v],v)
    return dist, prev
    #마지막으로 s부터 각 노드들까지의 거리와 predecessors를 리턴합니다.


if __name__ == '__main__':
    og = dict()
    noiseadded = dict()
    ex = dict()

    # graph 
    landscape = raw_input("Enter a name of file : ")
    f= open(landscape, 'r')
    while True: 
        line = f.readline()
        if not line: break
        splitline = line.split(",")
        og[splitline[0]] = dict()
        og[splitline[1]] = dict()
        noiseadded[splitline[0]] = dict()
        noiseadded[splitline[1]] = dict()
        ex[splitline[0]] = dict()
        ex[splitline[1]] = dict()


    f=open(landscape, 'r')
    
    while True: 
        line = f.readline()
        if not line: break
        splitline = line.split(",")
        mini = float(splitline[3])
        maxi = float(splitline[4])

        og[splitline[0]].update({splitline[1]:[float(splitline[2]),mini,maxi,float((mini+maxi)/6),float((mini+maxi)/2),float(((mini+maxi)/6)*5)]})
        og[splitline[1]].update({splitline[0]:[float(splitline[2]),mini,maxi,float((mini+maxi)/6),float((mini+maxi)/2),float(((mini+maxi)/6)*5)]})
        noiseadded[splitline[0]].update({splitline[1]:[float(splitline[2]),mini,maxi,float((mini+maxi)/6),float((mini+maxi)/2),float(((mini+maxi)/6)*5)]})
        noiseadded[splitline[1]].update({splitline[0]:[float(splitline[2]),mini,maxi,float((mini+maxi)/6),float((mini+maxi)/2),float(((mini+maxi)/6)*5)]})
        ex[splitline[0]].update({splitline[1]:[float(splitline[2]),mini,maxi,float((mini+maxi)/6),float((mini+maxi)/2),float(((mini+maxi)/6)*5)]})
        ex[splitline[1]].update({splitline[0]:[float(splitline[2]),mini,maxi,float((mini+maxi)/6),float((mini+maxi)/2),float(((mini+maxi)/6)*5)]})

    print("og : ")
    print(og)

    source = raw_input("Enter a start point : ")
    destination = raw_input("Enter a destination : ")
    Ipsilon = raw_input("Enter a ipsilon : ")
    division = raw_input("Enter a number of division : ")
    noiseadded = Laplace(og,Ipsilon)
    print("Laplace noise")
    print(noiseadded)

    Exponential(ex, og, float(Ipsilon))
    print("Exponential noise ")
    print(ex)
    flag = 1

    f.close()



