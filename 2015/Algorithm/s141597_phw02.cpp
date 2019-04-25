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

		// vf(���� �Է°�)�� ���Ͽ�
		// ù �Ҵ�ÿ� �Է¹��� ���� �״�� �Ҵ�
		if(V[E[i].vf].f_hd == -1)
			V[E[i].vf].f_hd = E[i].name;

		// �̹� ���� �Ҵ�Ǿ� �ִٸ� Edge �迭�� �̹� ����� Edge name�� �ѱ�
		else
		{
			E[i].fp = V[E[i].vf].f_hd;
			V[E[i].vf].f_hd = E[i].name;
		}

		// vr(������ �Է°�)�� ���Ͽ�
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

	// �湮�� vertex�� flag�� true�� set
	V[src].flag = true;

	// f_hd(���� �Է°�)�� ���Ͽ� Ž��
	for(int w = V[src].f_hd; w!=-1; w = E[w].fp)
	{
		// ������ edge�� flag Ȯ��
		if(E[w].flag == false)
		{
			// �湮�� vertex�� flag Ȯ��
			if(V[E[w].vf].flag == false || V[E[w].vr].flag == false)
			{
				E[w].flag = true;
				Tree_cost += E[w].cost;
				// �湮���� ���� vertex������ DFS
				if(V[E[w].vf].flag == false) DFS_Tree_adj_array(E[w].vf);
				else DFS_Tree_adj_array(E[w].vr);
			}
		}
	}
	// r_hd(������ �Է°�)�� ���Ͽ� Ž�� (����� f_hd�� ����� ����)
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
