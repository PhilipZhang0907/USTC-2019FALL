#include<stdio.h>
#include<stdlib.h>
#define N 100000
#define M 1000000

typedef struct Edge{
	int head;
	int tail;
	int weight;
	struct Edge* nextedge;
}EDGE;

typedef struct PathLen{
	int vertex;
	int weight;
}LEN;

int ShortestDis[N];
int PosInQueue[N];
// Priority Queue
struct PathLen Path[N];
int QueueSize = 0;

struct Edge* Vertices[N];
int MinVertex;
int MinLen;

int HeapParent(int num)
{
	return (num+1)/2 - 1;
}//checked

int HeapLchild(int num)
{
	return 2*num + 1;
}//checked

int HeapRchild(int num)
{
	return 2*(num+1);
}//checked

void UpHeapify(int pos)
{
	if(pos == 0)
	{
		return;
	}
	int parent = HeapParent(pos);
	if(Path[parent].weight > Path[pos].weight)
	{
		PosInQueue[Path[pos].vertex-1] = parent;
		PosInQueue[Path[parent].vertex-1] = pos;
		struct PathLen tmp;
		tmp = Path[parent];
		Path[parent] = Path[pos];
		Path[pos] = tmp;
		UpHeapify(parent);
	}
	return;
}

void DownHeapify(int pos)
{
	int lchild = HeapLchild(pos);
	int rchild = HeapRchild(pos);
	int SMALLEST = pos;
	if(lchild < QueueSize && Path[lchild].weight < Path[pos].weight)
	{
		SMALLEST = lchild;
	}
	if(rchild < QueueSize && Path[rchild].weight < Path[SMALLEST].weight)
	{
		SMALLEST = rchild;
	}
	if(SMALLEST != pos)
	{
		PosInQueue[Path[pos].vertex-1] = SMALLEST;
		PosInQueue[Path[SMALLEST].vertex-1] = pos;
		struct PathLen tmp = Path[pos];
		Path[pos] = Path[SMALLEST];
		Path[SMALLEST] = tmp;
		DownHeapify(SMALLEST);
	}
	return;
}

void InQueue(int vertex, int weight)
{
	Path[QueueSize].vertex = vertex;
	Path[QueueSize].weight = weight;
	PosInQueue[vertex - 1] = QueueSize;
	UpHeapify(QueueSize);
	QueueSize++;
	return;
}//checked

void ExtractMin()
{
	if(QueueSize == 0)
	{
		return;
	}
	MinVertex = Path[0].vertex;
	MinLen = Path[0].weight;
	ShortestDis[MinVertex-1] = MinLen;
	Path[0] = Path[QueueSize-1];
	QueueSize--;
	if(QueueSize > 0)
		DownHeapify(0);
	return;
}//checked

void LooseEdge(int num, int weight)
{
	int i = PosInQueue[num - 1];
	if(Path[i].vertex == num)
	{
		if(Path[i].weight > MinLen + weight)
		{
			Path[i].weight = MinLen + weight;
			UpHeapify(i);
			return;
		}
		return;
	}
	InQueue(num, MinLen + weight);
	return;
}

void Loose()
{
	struct Edge* tmp = Vertices[MinVertex-1];
	while(tmp != NULL)
	{
		if(ShortestDis[tmp->head - 1] == -1)
		{
			LooseEdge(tmp->head, tmp->weight);
		}
		tmp = tmp->nextedge;
	}
	return;
}

void Initial(int n, int vertex)
{
	int i = 0;
	while(i < n)
	{
		ShortestDis[i] = -1;
		PosInQueue[i] = -1;
		Vertices[i] = NULL;
		i++;
	}
	ShortestDis[vertex - 1] = 0;
	return;
}

void AddEdge(int from, int to, int cost)
{
	if(Vertices[from-1] == NULL)
	{
		Vertices[from-1] = (struct Edge*)malloc(sizeof(EDGE));
		Vertices[from-1]->tail = from;
		Vertices[from-1]->head = to;
		Vertices[from-1]->weight = cost;
		Vertices[from-1]->nextedge = NULL;
		return;
	}
	else
	{
		struct Edge* tmp = Vertices[from-1];
		while(tmp->nextedge != NULL && tmp->head != to)
		{
			tmp = tmp->nextedge;
		}
		if(tmp->head == to)
		{
			if(tmp->weight > cost)
			{
				tmp->weight = cost;
			}
			return;
		}
		else
		{
			tmp->nextedge = (struct Edge*)malloc(sizeof(EDGE));
			tmp->nextedge->tail = from;
			tmp->nextedge->head = to;
			tmp->nextedge->weight = cost;
			tmp->nextedge->nextedge = NULL;
			return;
		}
	}
	return;
}

int main()
{
	int n, m;
	int start, end;
	scanf("%d %d %d %d",&n, &m, &start, &end);
	Initial(n,start);
	int i = 0;
	while(i < m)
	{
		int From, To, Cost;
		scanf("%d %d %d",&From, &To, &Cost);
		AddEdge(From, To, Cost);
		i++;
	}
	struct Edge* tmp = Vertices[start-1];
	while(tmp != NULL)
	{
		InQueue(tmp->head, tmp->weight);
		tmp = tmp->nextedge;
	}
	while(QueueSize != 0)
	{
		ExtractMin();
		Loose();
	}
	printf("%d\n",ShortestDis[end-1]);
	return 0;
}
