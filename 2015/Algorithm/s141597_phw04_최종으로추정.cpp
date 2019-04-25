#ifndef DEFINE_C
#define DEFINE_C 

#define DMAX 1000000000;

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

double dist(double, double, double, double);
double abs_double(double);
void swap(unsigned int*, unsigned int*);
double min(double, double);
void quickSort(int, int, double*, unsigned int*);
double bruteForce(unsigned int, unsigned int, unsigned int*, unsigned int*, double*, double*, unsigned int*);

// O(nlogn) - Quick Sort ���
void Sort_by_X_or_Y(double *A, unsigned int *P, unsigned int N)
{
	for (int i = 0; i < N; i++)
		P[i] = i;
	quickSort(0, N-1, A, P);
}

double Closest_Pair_DC(
	unsigned int L, unsigned int R,			// left and right indices
	unsigned int *pt1, unsigned int *pt2,   // distance�� �ּ��� �� ���� 
											// X, Y �迭������ index
	double *X, double *Y,					// input points position array
	unsigned int *P1, unsigned int *P2, unsigned int *P3, // temp index arrays
	unsigned int THR	// threshold value
	) 
{
	int N = R - L + 1;
	int M;
	int i, j;
	double d = DMAX;
	double CPl, CPr;
	double MX;
	int countP;

	// P1 : X�� ����
	// P2 : Y�� ����
	// P3 : d �Ÿ� ��ŭ�� ���ο� ����� ������ �����ϴµ� ���

	// Brute-Force Algorithm
	if (N < THR)
		return bruteForce(L, R, pt1, pt2, X, Y, P2);

	// ���� ���� üũ
	if (N % 2 == 0) M = N / 2;
	else M = N / 2 + 1;

	CPl = Closest_Pair_DC(L, L+M-1, pt1, pt2, X, Y, P1, P2, P3, THR);
	CPr = Closest_Pair_DC(L+M, R, pt1, pt2, X, Y, P1, P2, P3, THR);

	d = min(CPl, CPr);

	// P3�� �� �Լ� �������� ����ϹǷ� �ʱ�ȭ
	for(i=0; i<N; i++)
		P3[i] = -1;
	
	// MX : X�� ���Ľ� �߰���
	// MX�� �������� �߰����� ������ - P3�� ����
	MX = X[P1[M]];
	j = 0;
	for(i=0; i<N; i++)
	{
		if(abs_double(X[P1[i]] - MX) < d) 
		{
			P3[j] = P1[i];
			j++;
		}
	}
	
	// �߰��� ��ġ�� ����� ������ ���� �ּ� distance���� ��
	// countP�� P3 �迭�� ����� ������ ����. 
	countP = j;
	quickSort(0, countP-1, Y, P3);
	
	for(i=0; i<countP; i++)
	{
		for(j=i+1; j<countP && abs_double(Y[P3[j]] - Y[P3[i]]) < d; j++)
		{
			double dmin = dist(X[P3[j]], Y[P3[j]], X[P3[i]], Y[P3[i]]);
			if(dmin < d)
			{
				*pt1 = P3[i]; *pt2 = P3[j];
				d = dmin;
			}
		}
	}

	return d;

}
// Brute-Force Algorithm
double bruteForce(
	unsigned int L, unsigned int R, 
	unsigned int* pt1, unsigned int*pt2, 
	double* X, double* Y, 
	unsigned int* P
	)
{
	int N = R - L + 1;
	int d = DMAX;
	int i, j;

	for(i=L; i<=R; i++)
		P[i] = i;
	quickSort(L, R, Y, P);

	d = dist(X[P[L]], Y[P[L]], X[P[L+1]], Y[P[L+1]]);
	*pt1 = P[L]; *pt2 = P[L+1];

	for(i=L; i<R; i++)
	{
		for(j=i+1; j<R && abs_double(Y[P[j]] - Y[P[i]]) < d ; j++)
		{
			double dij = dist(X[P[i]], Y[P[i]], X[P[j]], Y[P[j]]);
			if(dij < d)
			{
				d = dij;
				*pt1 = P[i], *pt2 = P[j];
			}
		}
	}

	return d;

}
// �� �� ������ �Ÿ�
double dist(double x1, double y1, double x2, double y2)
{
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}
// double���� ���� ABS �Լ�
double abs_double(double x)
{
	if (x < 0) return -1 * x;
	else return x;
}
// swap �Լ�. quickSort�� ���� ���
void swap(unsigned int* a, unsigned int* b)
{
	unsigned int t = *a;
	*a = *b;
	*b = t;
}
// ���� ���� �� return
double min(double a, double b)
{
	return (a < b) ? a : b;
}
// QuickSort Algorithm
void quickSort(int left, int right, double *A, unsigned int *P)
{
	int pivot = left;
	int j = pivot;
	int i = left + 1;

	if (left < right)
	{
		for (; i <= right; i++) {
			if (A[(int)P[i]] < A[(int)P[pivot]]) {
				j++;
				swap(&P[j], &P[i]);

			}
		}
		swap(&P[left], &P[j]);
		pivot = j;

		quickSort(left, pivot - 1, A, P);
		quickSort(pivot + 1, right, A, P);
	}
}

#endif DEFINE_C