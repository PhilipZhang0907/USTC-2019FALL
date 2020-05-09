#include<stdio.h>
#include<stdlib.h>
#define N 10000
typedef struct CORD{
	int X;
	int Y;
}cord;

struct CORD mark[N];
int DP[N];

int main()
{
	int n;
	scanf("%d",&n);
	int i = 0;
	while(i < n)
	{
		scanf("%d %d",&mark[i].X, &mark[i].Y);
		i++;
	}
	int answer = 0;
	i = 0;
	while(i < n)
	{
		DP[i] = 1;
		int j = 0;
		while(j < i)
		{
			if(mark[j].X <= mark[i].X && mark[j].Y <= mark[i].Y || mark[i].X <= mark[j].X && mark[i].Y <= mark[j].Y)
				DP[i] = DP[i] > (DP[j] + 1) ? DP[i] : DP[j] + 1;
			j++;
		}
		answer = answer > DP[i] ? answer : DP[i];
		i++;
	}
	printf("%d",answer);
	return 0;
}
