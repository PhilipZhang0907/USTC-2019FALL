#include<stdio.h>
#include<stdlib.h>

#define N 100000
#define M 1000000

typedef struct EDGE{
	int Head;
	int Tail;
	int Weight;
}Edge;

typedef struct VERTEX{
	int number;
	int parent;
}Vertex;

struct EDGE Edges[M+1];
struct VERTEX Vertices[N+1];

int Parent(int number)
{
	int i = number;
	while(Vertices[i].parent != Vertices[i].number)
	{
		i = Vertices[i].parent;
	}
	return i;
}

int TreeSpan(int n)
{
	int Cost = 0;
	int i = 0;
	int EdgesInTree = 0;
	while(EdgesInTree <= n-1 && i < M)
	{
		int HeadParent = Parent(Edges[i].Head);
		int TailParent = Parent(Edges[i].Tail);
		if(HeadParent != TailParent)
		{
			Cost += Edges[i].Weight;
			EdgesInTree++;
			Vertices[TailParent].parent = HeadParent;
		}
		i++;
	}
	return Cost;
}

void Swap(int low, int high)
{
    struct EDGE temp;
    temp.Head = Edges[low].Head;
    temp.Tail = Edges[low].Tail;
    temp.Weight = Edges[low].Weight;
    Edges[low].Head = Edges[high].Head;
    Edges[low].Tail = Edges[high].Tail;
    Edges[low].Weight = Edges[high].Weight;
    Edges[high].Head = temp.Head;
    Edges[high].Tail = temp.Tail;
    Edges[high].Weight = temp.Weight;
}

int Partition(int low, int high)
{
    int base = Edges[low].Weight;
    while(low < high)
    {
        while(low < high && Edges[high].Weight >= base)
        {
            high --;
        }
        Swap(low, high);
        while(low < high && Edges[low].Weight <= base)
        {
            low ++;
        }
        Swap(low, high);
    }
    return low;
}

void QuickSort(int low, int high)
{
    if(low < high)
    {
        int base = Partition(low, high);
        QuickSort(low, base - 1);
        QuickSort(base + 1, high);
    }
}

void Initial(int n)
{
	int i = 1;
	while(i <= n)
	{
		Vertices[i].number = i;
		Vertices[i].parent = i;
		i++;
	}
	return;
}

int main()
{
	int n, m;
	scanf("%d %d",&n,&m);
	Initial(n);
	int i = 0;
	while(i < m)
	{
		scanf("%d %d %d",&Edges[i].Head, &Edges[i].Tail, &Edges[i].Weight);
		i++;
	}
	QuickSort(0, m-1);
	int Result = TreeSpan(n);
	printf("%d", Result);
	return 0;
}
