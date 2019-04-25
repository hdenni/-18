# -*- coding: utf-8 -*-
from pq import PriorityQueue

def Dijkstra(G,src):
    #initialization
    dist = {}
#   prev = {}

    Q = PriorityQueue()
#   print(g["home"].keys())
    
    #출발노드 s는 자신까지의 거리가 0이고 자신의 predecessor
    dist[src] = 0
#   prev[s] = None

    #다른 노드들은 모두 거리를 infinity로 설정하고 predecessor는 일단 None으로 설정하고
    '''for n in g:
        dist[n[0]] = float('inf')
        prev[n[0]] = None
        dist[n[1]] = float('inf')
        prev[n[1]] = None
    '''
    #그러면서 PQ에다가 노드들을 넣어줍니다.
    for node in G.keys():
        if node != src :
            dist[node] = float('inf')
            #prev[n] = None
        '''
        if n in g[s].keys():
            dist[n] = float('inf')
            prev[n] = None
        '''
        Q.insert(dist[node],node) # n이 우선순위 dist가 value
    #PQ가 빌때까지 계속 루프를 돌면서,
    while Q.size() > 0:
        p, u = Q.pop() #현재까지 가장 짧은 거리를 갖고 있는 노드를 pop

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
        for v in G[u].keys():
            #alt = dist[u] + g[u][v].get('weight',1)
            alt = dist[u] + G[u][v]
            if alt < dist[v]:
                dist[v] = alt
                Q.insert(dist[v],v)
    
    return dist
    #마지막으로 s로부터 각 노드들까지의 거리와 predecessors를 리턴합니다.

landscape = {
    'home': {'hairShop':5, 'superMarket':10, 'EnglishAcademy':9},
    'hairShop' : {'home':5 ,'superMarket':3, 'bank':11},
    'superMarket' : {'hairShop':3, 'home':10,'EnglishAcademy':7, 'restourant':3},
    'EnglishAcademy': {'home':9, 'superMarket':7, 'school':12},
    'restourant' : {'superMarket':3, 'bank':4},
    'bank' : {'hairShop':11, 'restourant':4,'EnglishAcademy':7, 'school':2},
    'school' : {'bank':2, 'EnglishAcademy':12}
}
