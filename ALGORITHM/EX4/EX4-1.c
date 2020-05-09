#include<stdio.h>
#include<stdlib.h>
#define N 100000
#define R 1000

int Coordinate[N];

void Initial(int n)
{
	int i = 0;
	while(i < n)
	{
		Coordinate[i] = -1;
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

int NextUncovered(int cover, int n)
{
	int i = 0;
	while(Coordinate[i] <= cover && i < n)
	{
		i++;
	}
	if(i == n)
		return -1;
	else
		return Coordinate[i];
}

int SetBase(int uncover, int r, int n)
{
	int NextBase = 0;
	while(Coordinate[NextBase] - uncover <= r && NextBase < n)
		NextBase++;
	NextBase -= 1;
	return Coordinate[NextBase] + r;
}

int main()
{
	int n,r;
	scanf("%d %d",&n, &r);
	Initial(n);
	int i = 0;
	while(i < n)
	{
		scanf("%d",Coordinate + i);
		i++;
	}
	QuickSort(Coordinate, 0, n-1);
	int LongestCoverRange = -1;
	int BaseNumber = 0;
	i = Coordinate[0];
	while(LongestCoverRange <= Coordinate[n-1] && i != -1)
	{
		LongestCoverRange = SetBase(i,r,n);
		BaseNumber++;
		i = NextUncovered(LongestCoverRange,n);
	}
	printf("%d", BaseNumber);
	return 0;
}
