#include<stdio.h>
#include<stdlib.h>
#define N 200

int RESULT[N][N];
int LENGTH[N][N];

void Initial(int n)
{
	int i = 0;
	int j = 0;
	while(i < n)
	{
		j = 0;
		while(j < n)
		{
			RESULT[i][j] = 0;
			j++;
		}
		i++;
	}
	return;
}

int MERGE(int start, int end)
{
	if(start + 1 == end)
	{
		RESULT[start][end] = LENGTH[start][end];
		return RESULT[start][end];
	}
	int i = start;
	int answer = -1;
	while(i < end)
	{
		if(RESULT[start][i] == 0 && start != i)
			RESULT[start][i] = MERGE(start, i);
		if(RESULT[i+1][end] == 0 && i+1 != end)
			RESULT[i+1][end] = MERGE(i+1, end);
		if(answer > RESULT[start][i] + RESULT[i+1][end] + LENGTH[start][i] + LENGTH[i+1][end] || answer == -1)
			answer = RESULT[start][i] + RESULT[i+1][end] + LENGTH[start][i] + LENGTH[i+1][end];
		i++;
	}
	RESULT[start][end] = answer;
	return answer;
}

int main()
{
	int n;
	Initial(n);
	scanf("%d",&n);
	int i = 0;
	while(i < n)
	{
		scanf("%d",&LENGTH[i][i]);
		int j = i - 1;
		while(j >= 0)
		{
			LENGTH[j][i] = LENGTH[j][i - 1] + LENGTH[i][i];
			j--;
		}
		i++;
	}
	int answer = MERGE(0, n-1);
	printf("%d",answer);
	return 0;
}
