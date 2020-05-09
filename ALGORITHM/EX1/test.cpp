#include<stdio.h>
#include<stdlib.h>
#define N 1000000

int array[N];
int max = -1;
int max_pos = -1;
int main()
{
	int n;
	int k;
	scanf("%d %d",&n,&k);
	int i=1;
	while(i<=n)
	{
		scanf("%d",array + i - 1);
		if(array[i-1]>max)
		{
			max = array[i-1];
			max_pos = i;
		}
		if(max_pos <= i-k)
		{
			max_pos = i-k+1;
			max = array[i-k];
			int j = i-k+1;
			for(j;j<=i;j++)
			{
				if(array[j-1]>=max)
				{
					max_pos = j;
					max = array[j-1];
				}
			}
		}
		if(i>=k && i<=n)
			printf("%d ",max);
		i++;
	}
	return 0;
}
