#include "Graph_adj_array.h"
#include "Graph_adj_array_extern.h"
#include "Graph_adj_array_function.h"

void Read_Graph_adj_array ( ) {
	// read graph information
	for (int i = 0; i < Vnum; i++) {
		V[i].name = i;
		V[i].f_hd = -1;
		V[i].r_hd = -1;
	}

	for (int i = 0; i < Enum; i++) {
		E[i].name = i;
		E[i].fp = -1;
		E[i].vr = -1;
		E[i].vr = -1;
		E[i].rp = -1;
	}
	for(int i=0; i<Enum; i++){
		scanf_s("%d %d %d", &(E[i].vf), &(E[i].vr), &(E[i].cost));

		// vf(왼쪽 입력값)에 대하여
		// 첫 할당시엔 입력받은 값을 그대로 할당
		if(V[E[i].vf].f_hd == -1)
			V[E[i].vf].f_hd = E[i].name;

		// 이미 값이 할당되어 있다면 Edge 배열로 이미 저장된 Edge name을 넘김
		else
		{
			E[i].fp = V[E[i].vf].f_hd;
			V[E[i].vf].f_hd = E[i].name;
		}

		// vr(오른쪽 입력값)에 대하여
		if(V[E[i].vr].r_hd == -1)
			V[E[i].vr].r_hd = E[i].name;
		else
		{
			E[i].rp = V[E[i].vr].r_hd;
			V[E[i].vr].r_hd = E[i].name;
		}
	}
}

void DFS_Tree_adj_array ( int src ) {  // src = source node index

	// 방문한 vertex의 flag를 true로 set
	V[src].flag = true;

	// f_hd(왼쪽 입력값)에 대하여 탐색
	for(int w = V[src].f_hd; w!=-1; w = E[w].fp)
	{
		// 지나갈 edge의 flag 확인
		if(E[w].flag == false)
		{
			// 방문할 vertex의 flag 확인
			if(V[E[w].vf].flag == false || V[E[w].vr].flag == false)
			{
				E[w].flag = true;
				Tree_cost += E[w].cost;
				// 방문하지 않은 vertex쪽으로 DFS
				if(V[E[w].vf].flag == false) DFS_Tree_adj_array(E[w].vf);
				else DFS_Tree_adj_array(E[w].vr);
			}
		}
	}
	// r_hd(오른쪽 입력값)에 대하여 탐색 (방법은 f_hd의 방법과 동일)
	for(int w = V[src].r_hd; w!=-1; w = E[w].rp)
	{
		if(E[w].flag == false)
		{
			if(V[E[w].vf].flag == false || V[E[w].vr].flag == false)
			{
				E[w].flag = true;
				Tree_cost += E[w].cost;
				if(V[E[w].vf].flag == false) DFS_Tree_adj_array(E[w].vf);
				else DFS_Tree_adj_array(E[w].vr);
			}
		}
	}
}
