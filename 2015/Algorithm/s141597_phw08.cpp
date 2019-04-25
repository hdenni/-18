#include <stdio.h>

int pan[10][10];

int solve()
{
	int i, j;

	// 비어있는 스도쿠 칸 탐색
	for (i = 1; i <= 9; i++)
	{
		for (j = 1; j <= 9; j++)
			if (pan[i][j] == 0) break;
		if (j != 10) break;
	}

	// 비어있는 스도쿠 칸이 없음. 스도쿠 완료
	if (i == 10 && j == 10)
		return true;

	// 비어있는 칸에 1부터 9까지 숫자를 넣어본다
	for (int num = 1; num <= 9; num++)
	{
		int tmp, a, b;
		bool flag = false;

		// 가로 줄에 같은 숫자가 포함되어 있을 경우 실패
		for (tmp = 1; tmp <= 9; tmp++)
			if (pan[i][tmp] == num) break;
		if (tmp != 10) continue;

		// 세로 줄에 같은 숫자가 포함되어 있을 경우 실패
		for (tmp = 1; tmp <= 9; tmp++)
			if (pan[tmp][j] == num) break;
		if (tmp != 10) continue;

		// 3*3 칸 안에 같은 숫자가 포함되어 있을 경우 실패
		a = ((i - 1) / 3) * 3 + 1;
		b = ((j - 1) / 3) * 3 + 1;
		for (int m = a; m<a + 3; m++)
			for (int n = b; n<b + 3; n++)
				if (pan[m][n] == num) flag = true;
		if (flag) continue;

		// 해당 숫자를 넣은 채로 체크
		pan[i][j] = num;
		if (solve()) return true;

		// 만약 값이 맞지 않다면 다시 공백으로 두고, 다른 숫자를 먼저 탐색.
		pan[i][j] = 0;
	}
	return false;
}
int main()
{
	// 입력
	for (int i = 1; i <= 9; i++)
		for (int j = 1; j <= 9; j++)
			scanf("%d", &pan[i][j]);

	// 문제풀이
	solve();

	// 출력
	for (int i = 1; i <= 9; i++)
	{
		for (int j = 1; j <= 9; j++)
			printf("%d ", pan[i][j]);
		printf("\n");
	}

	return 0;
}