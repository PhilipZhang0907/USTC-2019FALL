#include<stdio.h>
#define N 10000000
int array[N];

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

int main()
{
	int n;
	scanf("%d\n",&n);
	int i = 0;
	while(i < n)
	{
		scanf("%d",array + i);
		i++;
	}
	QuickSort(array, 0, n-1);
	int gap = 0;
	i = 1;
	while(i < n)
	{
		if(gap < array[i]-array[i-1])
			gap = array[i] - array[i-1];
		i++;
	} 
	printf("%d",gap);
	return 0;
}
