#include<stdio.h>
#include<stdlib.h>
#define N 200
#define W 10000
#define M 10000
typedef struct OBJECT{
	int weight;
	int value;
	int num;
}OBJ;

struct OBJECT item[N];
int DP[N+1][W+1];

void INITIAL(int n, int w)
{
	int i = 0;
	while(i <= n)
	{
		DP[i][0] = 0;
		i++;
	}
	i = 0;
	while(i <= w)
	{
		DP[0][i] = 0;
		i++;
	}
	return;
}

int main()
{
	int n, w;
	scanf("%d %d",&n,&w);
	int i = 0;
	while(i < n)
	{
		scanf("%d %d %d",&item[i].weight, &item[i].value, &item[i].num);
		i++;
	}
	INITIAL(n, w);
	int k;
	int m;
	int HowMany;
	for(k = 1; k <= n; k++)
	{
		for(m = 1; m <= w; m++)
		{
			int max = DP[k-1][m];
			for(HowMany = 0; HowMany <= item[k-1].num; HowMany++)
			{
				if(HowMany*item[k-1].weight > m)
				{
					break;
				}
				else
				{
					if(max < DP[k-1][m- HowMany*item[k-1].weight] + HowMany*item[k-1].value)
						max = DP[k-1][m- HowMany*item[k-1].weight] + HowMany*item[k-1].value;
				}
			}
			DP[k][m] = max;
		}
	}
	printf("%d",DP[n][w]);
	return 0;
}
