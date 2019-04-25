#include <stdio.h>
#include <string.h>

char L[1000001];		// 입력 문자열
char stack[1000001] = { 0 };	// pop, push할 스택
char output[1000001] = { 0 };	// 결과물

int main()
{
	int len = 0;	//입력받은 문자열 길이
	int count;		//현재 문자열 커서 위치
	int stack_cnt;	//stack 배열 카운터
	int input;		//테스트케이스 수
	int i, n;

	scanf("%d", &input);
	
	for (n = 0; n < input; n++)
	{
		scanf("%s", L);
		len = strlen(L);
		count = 0;
		stack_cnt = 0;
		for (i = 0; i < len; i++)
		{
			if (L[i] == '<')
			{
				if (count > 0)
				{
					stack[stack_cnt++] = output[count-1];
					output[count-1] = '\0';
					count--;
				}
			}
			else if (L[i] == '>')
			{
				if (stack_cnt > 0)
				{
					output[count++] = stack[stack_cnt-1];
					stack[stack_cnt-1] = '\0';
					stack_cnt--;
				}
			}
			else if (L[i] == '-')
			{
				if (count > 0)
					output[--count] = '\0';
			}
			else
				output[count++] = L[i];
		}
		for (i = 0; i < count; i++)
			printf("%c", output[i]);
		for (i = stack_cnt-1; i >= 0; i--)
			printf("%c", stack[i]);
		printf("\n");

		for (i = 0; i < count; i++)
			output[i] = '\0';
		for (i = 0; i < stack_cnt; i++)
			stack[i] = '\0';
	}
	
	return 0;
}