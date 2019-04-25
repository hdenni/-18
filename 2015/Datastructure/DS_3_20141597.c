#include <stdio.h>
#include <stdlib.h>

#define MAX_COL 100

typedef struct {
	int row;
	int col;
	int value;
}mat;
void fast_transpose(mat a[], mat b[]);
void mmult(mat a[], mat b[], mat d[]);
void storesum(mat d[], int* totald, int row, int column, int sum);

int COMPARE(int s, int t)
{
	if (s == t) return 0;
	else if (s < t) return -1;
	else return 1;
}

int main()
{
	FILE *fp_a = fopen("a.txt", "r");
	FILE *fp_b = fopen("b.txt", "r");

	mat *a, *b, *d;
	int nA, nB;
	int t1, t2;

	int i;

	fscanf(fp_a, "%d %d %d", &t1, &t2, &nA);
	a = (mat*)malloc((nA + 2)*sizeof(mat));
	a[0].row = t1; a[0].col = t2; a[0].value = nA;
	for (i = 1; i <= nA; i++)
		fscanf(fp_a, "%d %d %d", &a[i].row, &a[i].col, &a[i].value);
	fclose(fp_a);

	fscanf(fp_b, "%d %d %d", &t1, &t2, &nB);
	b = (mat*)malloc((nB + 2)*sizeof(mat));
	b[0].row = t1; b[0].col = t2; b[0].value = nB;
	for (i = 1; i <= nB; i++)
		fscanf(fp_b, "%d %d %d", &b[i].row, &b[i].col, &b[i].value);
	fclose(fp_b);

	d = (mat*)malloc(((a[0].row * b[0].col) + 2)*sizeof(mat));

	mmult(a, b, d);
	for (i = 0; i < d[0].value+1; i++)
		printf("%d %d %d\n", d[i].row, d[i].col, d[i].value);

	return 0;

}
void fast_transpose(mat a[], mat b[]) // 기존배열 & 새로운 배열
{
	// row_terms : 기존 행렬에서 각 column이 차 있는 개수
	int row_terms[MAX_COL], starting_pos[MAX_COL];
	int num_cols = a[0].col, new_value = a[0].value;
	int i, j;

	b[0].row = a[0].col; b[0].col = a[0].row;
	b[0].value = a[0].value;

	if (new_value > 0) /* nonzero matrix */
	{
		// 초기화
		for (i = 0; i < num_cols; i++)	row_terms[i] = 0;
		for (i = 0; i <= new_value; i++)	row_terms[a[i].col]++;

		starting_pos[0] = 1;
		for (i = 1; i < num_cols; i++)
			starting_pos[i] = starting_pos[i - 1] + row_terms[i - 1];

		for (i = 1; i <= new_value; i++)
		{
			j = starting_pos[a[i].col]++;
			b[j].row = a[i].col;	b[j].col = a[i].row;
			b[j].value = a[i].value;
		}

	}
}
void mmult(mat a[], mat b[], mat d[])
{
	int i, j;
	
	// 각각 matrix A의 row, column개수와 배열개수(-1)
	int rows_a = a[0].row, cols_a = a[0].col, totala = a[0].value;
	int cols_b = b[0].col, totalb = b[0].value;

	int totald = 0, row_begin = 1;
	int row = a[1].row, column, sum = 0;

	mat *new_b;
	new_b = (mat*)malloc((b[0].row * b[0].col)*sizeof(mat));
	
	// 계산불가
	if (cols_a != b[0].row) {
		fprintf(stderr, "Incompatible matrices\n");
		exit(1);
	}

	fast_transpose(b, new_b);

	a[totala + 1].row = rows_a;
	new_b[totalb + 1].row = cols_b;	new_b[totalb + 1].col = -1;

	for (i = 1; i <= totala; )
	{
		column = new_b[0].row;
		for (j = 1; j <= totalb + 1; )
		{
			if (a[i].row != row){
				storesum(d, &totald, row, column, &sum);
				i = row_begin;
				for (; new_b[j].row == column; j++);
				column = new_b[j].row;
			}
			else if (new_b[j].row != column) {
				storesum(d, &totald, row, column, &sum);
				i = row_begin;
				column = new_b[j].row;
			}
			else{
				switch (COMPARE(a[i].col, new_b[j].col)) {
				case -1: i++; break;
				case 0: sum += (a[i++].value * new_b[j++].value);
					break;
				case 1: j++; break;
				}
			}
		}
		for (; a[i].row == row; i++);
		row_begin = i; row = a[i].row;
	}

	d[0].row = rows_a; d[0].col = cols_b;
	d[0].value = totald;
}

void storesum(mat d[], int* totald, int row, int column, int *sum)
{
	if (*sum == 0) return;
	d[++(*totald)].row = row;
	d[*totald].col = column;
	d[*totald].value = *sum;
	*sum = 0;
}