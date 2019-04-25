# -*- coding: utf-8 -*-
from dijk import Dijkstra
from pq import PriorityQueue
import networkx as nx
import nxmetis as nm
    
def make_graph(D):
    G = nx.Graph()
    for src, dst_dic in D.items():
        for dst, w in dst_dic.items():
            G.add_edge(src, dst, weight=w)

    return G
 
# Pre-computation function
def transit_pre(G):

    #e = int(input("Enter number of graph partitions: ))
    e=2
    n, parted_G = nm.partition(G,e,edge_weight='weight')

    ###############################################################
    # set transit node (dictionary) : border
    ###############################################################
    tNode=dict()
    for node in list(G.nodes()):
        tNode[node] = False
    
    for node in list(G.nodes()):
        if tNode[node] == True :
            continue
        else:
            tList = landscape[node]
            # plist : partition graph with node
            for plist in parted_G:
                if node in plist : 
                    break
            for j in tList:
                if j not in plist:
                    tNode[node]=True
                    tNode[j]=True
    
    ###############################################################
    # set closest transit node
    ###############################################################
    '''
    cst_node_dict = dict()
    for node in tNode.keys():
        result = Dijkstra(landscape, node)
        for key, value in sorted(result.items(), key=lambda t:t[1]):
            if tNode[key] == True and key != node: 
                # node의 closest transit node
                temp = dict()
                temp[key] = value
                cst_node_dict[node] = temp
                break
    '''
    #cst_node_dict = dict()
    for node in list(G.nodes()):
        if tNode[node] == True:
            temp = dict()
            temp[node] = 0
            closest_transit_node[node] = temp
        else:
            result = Dijkstra(landscape, node)
            for key, value in sorted(result.items(), key=lambda t:t[1]):
                if tNode[key] == True:
                    temp = dict()
                    temp[key] = value
                    closest_transit_node[node] = temp
                    break

    ###############################################################
    # 같은 partition 내 노드끼리의 distance 저장(dictionary) : part_node_dict
    ###############################################################
    #part_node_dict = dict()
    '''
    for plist in parted_G:
        for node1 in plist:
            temp = dict()
            for node2 in plist:
                if node1 == node2 : continue
                else:
                    check_edge1 = (node1, node2)
                    check_edge2 = (node2, node1)
                    if (check_edge1 in list(G.edges()) or (check_edge2 in list(G.edges()))
                            temp[node2] = 
                            part_node_dict[node1]
    '''
    for plist in parted_G:
        for node1 in plist:
            result = Dijkstra(landscape, node1)
            same_partition_node[node1] = dict()
            for node2 in plist:
                if node1 == node2 : continue
                else :
                    same_partition_node[node1][node2] = result[node2]
    
    ###############################################################
    # transit node distance Table : tNode_disTable
    ###############################################################
   
    # count : transit node 개수. 필요 없어서 일단 뺐음
    # transit node distance table(dictionary) 설정
    #count=0
    #tNode_disTable = dict()
    for key, value in tNode.items():
        if value==True: 
            #count += 1
            distance_table[key] = dict()

    for node_src, node_dst_dict in distance_Table.items():
        result = Dijkstra(landscape, node_src)
        for key, value in result.items():
            if key != node_src and tNode[key] == True:
                node_dst_dict[key] = value
    
#    return cst_node_dict, part_node_dict, tNode_disTable, parted_G

# 실제 계산함수
def transit(G, src, dst, close, pTable, dTable, part_list):
# src->closest of src -> Table -> closest of dst -> dst
    distance = 0

    # 같은 partition안에 있을 때 미리 계산된 partition table을 이용
    for plist in part_list:
        if src in plist and dst in plist:
            distance = pTable[src][dst]
            return distance

    # src의 closest node와 dst의 closest node를 추출
    src_closest = close[src].keys()[0]
    dst_closest = close[dst].keys()[0]
    
    # d = d(src, closest_src) + d(closest_dst, dst)
    distance = close[src].values()[0] + close[dst].values()[0]

    # d = d + d(closest_src, closest_dst)
    distance = distance + dTable[src_closest][dst_closest]

    return distance



landscape = {
    'home': {'hairShop':5, 'superMarket':10, 'EnglishAcademy':9},
    'hairShop' : {'home':5 ,'superMarket':3, 'bank':11},
    'superMarket' : {'hairShop':3, 'home':10,'EnglishAcademy':7, 'restourant':3},
    'EnglishAcademy': {'home':9, 'superMarket':7, 'school':12},
    'restaurant' : {'superMarket':3, 'bank':4},
    'bank' : {'hairShop':11, 'restaurant':4,'EnglishAcademy':7, 'school':2},
    'school' : {'bank':2, 'EnglishAcademy':12}
}

# type : graph
G = make_graph(landscape)

# type : dictionary
closest_transit_node = dict()
same_partition_node = dict()
distance_table = dict()
partition_list = dict()
transit_pre(G)

source =raw_input("Enter the source:")
destination = raw_input("Enter the destination:")

#type : float
distance = transit(G, source, destination, closest_transit_node, same_partition_node, distance_table, partition_list)
print(distance)
