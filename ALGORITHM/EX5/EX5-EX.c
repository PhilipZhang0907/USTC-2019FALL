#include<stdio.h>
#include<stdlib.h>
#define N 5000
#define M 100000

typedef struct EDGE
{
	int head;
	int tail;
	struct EDGE* nextedge;
}edge;

typedef struct VERTEX{
	int weight;
	struct EDGE* firstedge;
}vertex;

struct EDGE* Vertices[N+1];
struct VERTEX ShrinkG[N+1];
int InDeg[N+1];
int VAL[N+1];
int DFN[N+1];// DFS time
int LOW[N+1];// Ancestor's time
int VIS[N+1];// In stack or not
int STACK[N+1];
int Top = 0;// stack top
int in = 0;
int BELONG[N+1];
int ANS = 0;

void Initial(int n)
{
	int i = 1;
	while(i <= n)
	{
		Vertices[i] = NULL;
		DFN[i] = 0;
		LOW[i] = 0;
		VIS[i] = 0;
		BELONG[i] = -1;
		i++;
	}
	return;
}

void Push(int u)
{
	STACK[Top] = u;
	Top++;
	return;
}

void Pop()
{
	Top--;
	return;
}

int StackEmpty()
{
	if(Top == 0)
		return 1;
	else 
		return 0;
}

int StackTop()
{
	return STACK[Top-1];
}

int MIN(int a, int b)
{
	if(a < b)
		return a;
	else
		return b;
}

void Tarjan(int u)
{
	in++;
	DFN[u] = in;
	LOW[u] = in;
	Push(u);
	VIS[u] = 1;
	struct EDGE* tmp = Vertices[u];
	while(tmp != NULL)
	{
		int num = tmp->head;
		if(!DFN[num])
		{
			Tarjan(num);
			LOW[u] = MIN(LOW[u], LOW[num]);
		}
		else if(VIS[num])
		{
			LOW[u] = MIN(LOW[u], DFN[num]);
		}
		tmp = tmp->nextedge;
	}
	if(LOW[u] == DFN[u])
	{
		while(!StackEmpty() && StackTop() != u)
		{
			VIS[StackTop()] = 0;
			BELONG[StackTop()] = ANS;
			Pop();
		}
		VIS[u] = 0;
		BELONG[u] = ANS;
		Pop();
		ANS++;
	}
}

void AddEdge(int from, int to)
{
	if(Vertices[from] == NULL)
	{
		Vertices[from] = (struct EDGE*)malloc(sizeof(edge));
		Vertices[from]->tail = from;
		Vertices[from]->head = to;
		Vertices[from]->nextedge = NULL;
		return;
	}
	else
	{
		struct EDGE* tmp = Vertices[from];
		while(tmp->nextedge != NULL && tmp->head != to)
		{
			tmp = tmp->nextedge;
		}
		if(tmp->head == to)
		{
			return;
		}
		else
		{
			tmp->nextedge = (struct EDGE*)malloc(sizeof(edge));
			tmp->nextedge->tail = from;
			tmp->nextedge->head = to;
			tmp->nextedge->nextedge = NULL;
			return;
		}
	}
	return;
}

void ShrinkGraph(int n)
{
	int i = 0;
	while(i < ANS)
	{
		ShrinkG[i].firstedge = NULL;
		ShrinkG[i].weight = 0;
		InDeg[i] = 0;
		VAL[i] = 0;
		i++;
	}
	i = 1;
	while(i <= n)//scan every vertex in the original graph
	{
		int vertex = BELONG[i];
		ShrinkG[vertex].weight++;
		struct EDGE* tmp = Vertices[i];
		while(tmp != NULL)
		{
			int ToInG = tmp->head;
			int ToInS = BELONG[ToInG];
			if(ToInS != vertex)
			{
				struct EDGE* ptr = ShrinkG[vertex].firstedge;
				if(ptr == NULL)
				{
					InDeg[ToInS]++;
					ShrinkG[vertex].firstedge = (struct EDGE*)malloc(sizeof(edge));
					ShrinkG[vertex].firstedge->head = ToInS;
					ShrinkG[vertex].firstedge->tail = vertex;
					ShrinkG[vertex].firstedge->nextedge = NULL; 
				}
				else
				{
					while(ptr->nextedge != NULL && ptr->head != ToInS)
					{
						ptr = ptr->nextedge;
					}
					if(ptr->nextedge == NULL)
					{
						InDeg[ToInS]++;
						ptr->nextedge = (struct EDGE*)malloc(sizeof(edge));
						ptr->nextedge->head = ToInS;
						ptr->nextedge->tail = vertex;
						ptr->nextedge->nextedge = NULL;
					}
				}
			}// Add edge from vertex -> ToInS
			tmp = tmp->nextedge;
		}
		i++;
	}
}

void FindLongestPath(int num)
{
	if(ShrinkG[num].firstedge == NULL)
	{
		VAL[num] = ShrinkG[num].weight;
		return;
	}
	else
	{
		int val = 0;
		struct EDGE* tmp = ShrinkG[num].firstedge;
		while(tmp != NULL)
		{
			if(VAL[tmp->head] == 0)
				FindLongestPath(tmp->head);
			if(VAL[tmp->head] > val)
				val = VAL[tmp->head];
			tmp = tmp->nextedge;
		}
		VAL[num] = ShrinkG[num].weight + val;
		return;
	}
}

int main()
{
	int n, m;
	scanf("%d %d",&n,&m);
	Initial(n);
	int i = 0;
	while(i < m)
	{
		int from, to;
		scanf("%d %d",&from, &to);
		AddEdge(from, to);
		i++;
	}
	i = 1;
	while(i <= n)
	{
		if(BELONG[i] == -1)
		{
			Tarjan(i);
		}
		i++;
	}
	ShrinkGraph(n);
	int RESULT = 0;
	i = 0;
	while(i < ANS)
	{
		if(InDeg[i] == 0)
		{
			FindLongestPath(i);
			if(RESULT < VAL[i])
				RESULT = VAL[i];
		}
		i++;
	}
	printf("%d", RESULT);
	return 0;
}
