#include<stdio.h>
#define N 1000000
#define MOD 10000019
int array[N];
int AfterSort[N];
int CountArray(int array[], int low, int high, int AfterSort[])
{
	if(low == high)
	{
		AfterSort[0] = array[high];
		return 0;
	}
	int len_low = (high-low+1)/2;
	int len_high = (high-low+1)-len_low;
	int result_low[len_low];
	int result_high[len_high];
	int count = (CountArray(array, low, low+len_low-1, result_low) + CountArray(array, low+len_low,high,result_high)) % MOD;
	int ptr_low = 0;
	int ptr_high = 0;
	int ptr = 0;
	int flag = -1;
	while(ptr_low < len_low && ptr_high < len_high)
	{
		if(result_low[ptr_low] > result_high[ptr_high])
		{
			count = (count + len_low - ptr_low) % MOD;
			AfterSort[ptr] = result_high[ptr_high];
			ptr_high++;
			ptr++;
		}
		else{
			AfterSort[ptr] = result_low[ptr_low];
			ptr_low++;
			ptr++;
		}
		if(ptr_low == len_low)
			flag = 0;
		if(ptr_high == len_high)
			flag = 1;
	}
	if(flag == -1)
		printf("Runtime Error");
	else if(flag == 0)
	{
		while(ptr_high < len_high)
		{
			AfterSort[ptr] = result_high[ptr_high];
			ptr_high++;
			ptr++;
		}
	}
	else if(flag == 1)
	{
		while(ptr_low < len_low)
		{
			AfterSort[ptr] = result_low[ptr_low];
			ptr_low++;
			ptr++;
		}
	}
	return count;
}

int main()
{
	int n;
	scanf("%d",&n);
	int i = 0;
	while(i < n)
	{
		scanf("%d", array + i);
		i++;
	}
	int result = CountArray(array, 0, n-1, AfterSort);
	printf("%d",result);
	return 0;
}
