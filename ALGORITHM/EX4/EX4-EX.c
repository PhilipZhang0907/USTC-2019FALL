#include<stdio.h>
#include<stdlib.h>
#define N 200001
#define M 200001
#define IFIN 30000000

int Coordi[N];
int GAP[N];

void Initial(int n)
{
	int i = 0;
	while(i < n)
	{
		Coordi[i] = -1;
		i++;
	}
	return;
}

void Swap(int arr[], int low, int high)
{
    int temp;
    temp = arr[low];
    arr[low] = arr[high];
    arr[high] = temp;
}

int Partition(int arr[], int low, int high)
{
    int base = arr[low];
    while(low < high)
    {
        while(low < high && arr[high] >= base)
        {
            high --;
        }
        Swap(arr, low, high);
        while(low < high && arr[low] <= base)
        {
            low ++;
        }
        Swap(arr, low, high);
    }
    return low;
}

void QuickSort(int arr[], int low, int high)
{
    if(low < high)
    {
        int base = Partition(arr, low, high);
        QuickSort(arr, low, base - 1);
        QuickSort(arr, base + 1, high);
    }
}

int Judge(int Expect, int m, int n)
{
	int count = 0;
	int gap;
	int j = 1;
	while(j < n)
	{
		gap = GAP[j];
		j++;
		if(gap < Expect)
		{
			while(gap < Expect && j < n)
			{
				gap += GAP[j];
				j++;
				count++;
			}
			if(gap < Expect)
			{
				if(count < m)
					return 1;
				else
					return 0;
			}
		}
	}
	if(count > m)
		return 0;
	else
		return 1;
}

int main()
{
	int n,m;
	scanf("%d %d",&n,&m);
	Initial(n);
	int i = 0;
	while(i < n)
	{
		scanf("%d",Coordi + i);
		i++;
	}
	QuickSort(Coordi, 0, n-1);
	i = 1;
	GAP[0] = 0;
	while(i < n)
	{
		GAP[i] = Coordi[i] - Coordi[i-1];
		i++;
	}
	int min = 1;
	int max = Coordi[n-1];
	int result = (min + max)/2;
	while(min < max - 1)
	{
		if(Judge(result, m, n) == 0)
		{
			max = result;
			result = (min + max)/2;
		}
		else
		{
			min = result;
			result = (min + max)/2;
		}
	}
	result = (min + max)/2;
	printf("%d",result);
	return 0;
}
