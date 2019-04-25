#include <stdio.h>
#include <stdlib.h>

typedef struct {
	float coef;
	int exp;
}poly;

int padd(poly*a, poly*b, int nA, int nB, poly* x);
int COMPARE(int s, int t);

int main()
{
	FILE *fp_a = fopen("a.txt", "r");
	FILE *fp_b = fopen("b.txt", "r");

	poly *a, *b;
	poly *x;

	int nA = 0, nB = 0;
	int nX = 0;
	int i, j;

	/* n 개수 체크 */
	while (fscanf(fp_a, "%d %d", &i, &j) != EOF)
		nA++;

	while (fscanf(fp_b, "%d %d", &i, &j) != EOF)
		nB++;

	fseek(fp_a, 0, SEEK_SET);
	fseek(fp_b, 0, SEEK_SET);
	
	a = (poly*)malloc(nA  * sizeof(poly));
	b = (poly*)malloc(nB * sizeof(poly));
	x = (poly*)malloc((nA + nB)*sizeof(poly));

	/* 입력 */
	i = 0;
	while (fscanf(fp_a, "%f %d", &a[i].coef, &a[i].exp)!=EOF)
		i++;

	i = 0;
	while (fscanf(fp_b, "%f %d", &b[i].coef, &b[i].exp) != EOF)
		i++;

	fclose(fp_a);		fclose(fp_b);
	
	nX = padd(a, b, nA, nB, x);

	for (i = 0; i < nX; i++)
		printf("(%.0f, %d) ", x[i].coef, x[i].exp);
	printf("\n");

	return 0;
}

int padd(poly*a, poly*b, int nA, int nB, poly* x)
{ 
	int i = 0, j = 0;
	int avail=0;

	while (i <= nA - 1 && j <= nB - 1)
	{
		switch (COMPARE(a[i].exp, b[j].exp)) {
		case -1:
			x[avail].exp = b[j].exp;
			x[avail++].coef = b[j++].coef;
			break;
		case 0:
			if (a[i].coef + b[j].coef)
			{
				x[avail].exp = a[i].exp;
				x[avail++].coef = a[i].coef + b[j].coef;
			}
			i++; j++; 
			break;
		case 1:
			x[avail].exp = a[i].exp;
			x[avail++].coef = a[i++].coef;
			break;
		}
	}
	/* 나머지 붙이기 */
	for (; i <= nA - 1; i++)
	{
		x[avail].exp = a[i].exp;
		x[avail++].coef = a[i].coef;
	}
	for (; j <= nB - 1; j++)
	{
		x[avail].exp = b[j].exp;
		x[avail++].coef = b[j].coef;
	}

	return avail;
}

int COMPARE(int s, int t)
{
	if (s == t) return 0;
	else if (s < t) return -1;
	else return 1;
}