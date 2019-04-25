#include <stdio.h>

int pan[10][10];

int solve()
{
	int i, j;

	// ����ִ� ������ ĭ Ž��
	for (i = 1; i <= 9; i++)
	{
		for (j = 1; j <= 9; j++)
			if (pan[i][j] == 0) break;
		if (j != 10) break;
	}

	// ����ִ� ������ ĭ�� ����. ������ �Ϸ�
	if (i == 10 && j == 10)
		return true;

	// ����ִ� ĭ�� 1���� 9���� ���ڸ� �־��
	for (int num = 1; num <= 9; num++)
	{
		int tmp, a, b;
		bool flag = false;

		// ���� �ٿ� ���� ���ڰ� ���ԵǾ� ���� ��� ����
		for (tmp = 1; tmp <= 9; tmp++)
			if (pan[i][tmp] == num) break;
		if (tmp != 10) continue;

		// ���� �ٿ� ���� ���ڰ� ���ԵǾ� ���� ��� ����
		for (tmp = 1; tmp <= 9; tmp++)
			if (pan[tmp][j] == num) break;
		if (tmp != 10) continue;

		// 3*3 ĭ �ȿ� ���� ���ڰ� ���ԵǾ� ���� ��� ����
		a = ((i - 1) / 3) * 3 + 1;
		b = ((j - 1) / 3) * 3 + 1;
		for (int m = a; m<a + 3; m++)
			for (int n = b; n<b + 3; n++)
				if (pan[m][n] == num) flag = true;
		if (flag) continue;

		// �ش� ���ڸ� ���� ä�� üũ
		pan[i][j] = num;
		if (solve()) return true;

		// ���� ���� ���� �ʴٸ� �ٽ� �������� �ΰ�, �ٸ� ���ڸ� ���� Ž��.
		pan[i][j] = 0;
	}
	return false;
}
int main()
{
	// �Է�
	for (int i = 1; i <= 9; i++)
		for (int j = 1; j <= 9; j++)
			scanf("%d", &pan[i][j]);

	// ����Ǯ��
	solve();

	// ���
	for (int i = 1; i <= 9; i++)
	{
		for (int j = 1; j <= 9; j++)
			printf("%d ", pan[i][j]);
		printf("\n");
	}

	return 0;
}