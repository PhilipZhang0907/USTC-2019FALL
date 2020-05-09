#include<stdio.h>
#include<stdlib.h>
#define N 100000
#define M 1000000

struct CONFLICT{
	int num1;
	int num2;
	int danger;
};

struct SET{
	int num;
	SET* nextconflict;
	int val;
};

struct NODE{
	int parent;
	int rank;
};

CONFLICT HAZARD[M];
SET conflict[N + 1];
NODE node[N + 1];

int hazard = 0;
int flag = 0;


void Initialize(int n)
{
	int i = 0;
	while(i < n)
	{
		node[i].parent = i;
		node[i].rank = 0;
		i++;
	}
	return;
}

int FIND_GROUP(int a)
{
	int tmp = a;
	while(node[tmp].parent != tmp)
		tmp = node[tmp].parent;
	if(a != tmp)
	{
		node[node[a].parent].rank--;
		node[a].parent = tmp;
	}
	return tmp;	
}

void JOIN(int a, int b)
{
	int group1 = FIND_GROUP(a);
	int group2 = FIND_GROUP(b);
	if(group1 == group2)
		return;
	else
	{
		if(node[group1].rank < node[group2].rank)
		{
			node[group2].rank++;
			node[group1].parent = group2;
		}
		else
		{
			node[group1].rank++;
			node[group2].parent = group1;
		}
	}
	return;
}

int FIND_HAZARD_SET(int a)
{
	int tmp = conflict[a].num;
	if(tmp != -1)
		tmp = FIND_GROUP(tmp);
	return tmp;
}

void SET_HAZARD(int a, int b, int value)
{
	if(conflict[a].num == -1)
	{
		conflict[a].num = b;
		conflict[a].val = value;
		return;
	}
	else
	{
		SET* tmp = conflict + a;
		while(tmp->nextconflict != NULL)
			tmp = tmp->nextconflict;
		tmp->nextconflict = (SET*)malloc(1*sizeof(SET));
		tmp->nextconflict->num = b;
		tmp->nextconflict->val = value;
		tmp->nextconflict->nextconflict = NULL;
		return;
	}
}

void DO(CONFLICT* array, int i)
{
	int num1 = array[i].num1;
	int num2 = array[i].num2;
	if(FIND_GROUP(num1) == FIND_GROUP(num2))
	{
		flag = 1;
		hazard = array[i].danger;
		return;
	}
	if(FIND_HAZARD_SET(num1) != -1)
		JOIN(FIND_HAZARD_SET(num1), FIND_GROUP(num2));
	if(FIND_HAZARD_SET(num2) != -1)
		JOIN(FIND_HAZARD_SET(num2), FIND_GROUP(num1));	
	SET_HAZARD(num1, num2, array[i].danger);
	SET_HAZARD(num2, num1, array[i].danger);
	return;
}

int cmp(CONFLICT a, CONFLICT b)
{
	if(a.danger >= b.danger)
	{
		return 1;
	}
	else return 0;
}

void Swap(CONFLICT* arr, int low, int high)
{
    CONFLICT temp;
    temp = arr[low];
    arr[low] = arr[high];
    arr[high] = temp;
}

int Partition(CONFLICT arr[], int low, int high)
{
    CONFLICT base = arr[low];
    while(low < high)
    {
        while(low < high && !cmp(arr[high], base))
        {
            high --;
        }
        Swap(arr, low, high);
        while(low < high && cmp(arr[low], base))
        {
            low ++;
        }
        Swap(arr, low, high);
    }
    return low;
}

void QuickSort(CONFLICT arr[], int low, int high)
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
	int n, m;
	scanf("%d %d",&n,&m);
	int i = 0;
	i = 0;
	while(i < n + 1)
	{
		conflict[i].num = -1;
		conflict[i].nextconflict = NULL;
		i++;
	}
	Initialize(n + 1);
	i = 0;
	while(i < m)
	{
		scanf("%d %d %d",&HAZARD[i].num1, &HAZARD[i].num2, &HAZARD[i].danger);
		i++;
	}
	QuickSort(HAZARD, 0, m-1);
	i = 0;
	while(i < m)
	{
		DO(HAZARD, i);
		if(flag)
			break;
		i++;
	}
	printf("%d",hazard);
	return 0;
}
