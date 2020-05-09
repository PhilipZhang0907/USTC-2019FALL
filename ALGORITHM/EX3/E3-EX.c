#include<stdio.h>
#include<stdlib.h>
#define N 2000
#define M 2000

int land[N][M];
int square[N][M];

int main()
{
	int n,m;
	scanf("%d %d",&n,&m);
	int i;
	int j;
	int answer = 0;
	for(i = 0;i < n;i++)
	{
		for(j = 0;j < m;j++)
			scanf("%d",&land[i][j]);
	}
	i = 0;
	j = 0;
	while(j < m)
	{
		if(land[0][j] == 1)
			square[0][j] = 0;
		else
		{
			square[0][j] = 1;
		}
		j++;
	}
	i = 0;
	j = 0;
	while(i < n)
	{
		if(land[i][0] == 1)
			square[i][0] = 0;
		else
		{
			square[i][0] = 1;
		} 
		i++;
	}
	i = 1;
	j = 1;
	while(i < n)
	{
		j = 1;
		while(j < m)
		{
			if(land[i][j] == 1)
			{
				square[i][j] = 0;
				j++;
				continue;
			}
			else
			{
				int min;
				if(square[i-1][j] > square[i-1][j-1])
					min = square[i-1][j-1];
				else min = square[i-1][j];
				if(min > square[i][j-1])
					min = square[i][j-1];
				square[i][j] = min + 1;
			}
			j++;
		}
		i++;
	}
	for(i = 0;i < n;i++)
	{
		for(j = 0;j < m;j++)
		{
			answer = answer + square[i][j];
		}	
	}
	printf("%d",answer);
	return 0;
}
