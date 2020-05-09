#include<stdio.h>
#include<stdlib.h>

#define N 500
#define M 20000

int Capacity[N+1][N+1];
int Flow[N+1][N+1];
int Remnant[N+1][N+1];
int Father[N+1];
int source;
int target;
int BFSQueue[N+1];
int QueueSize = 0;
int Queuehead = 0;

void Initial(int n)
{
	int i = 1;
	while(i <= n)
	{
		int j = 1;
		while(j <= n)
		{
			Capacity[i][j] = 0;
			Flow[i][j] = 0;
			Remnant[i][j] = 0;
			j++;
		}
		i++;
	}
	return;
}

void OutQueue(int n)
{
	int VertexNum = BFSQueue[Queuehead];
	QueueSize--;
	Queuehead++;
	int QueueTail = Queuehead + QueueSize;
	int i = 1;
	while(i <= n)
	{
		if(Remnant[VertexNum][i] != 0 && Father[i] == -1)
		{
			Father[i] = VertexNum;
			BFSQueue[QueueTail] = i;
			QueueSize++;
			QueueTail = Queuehead + QueueSize;
		}
		i++;
	}
	return;
}

void BFS(int n)
{
	int i = 1;
	while(i <= n)
	{
		Father[i] = -1;
		i++;
	}
	Father[source] = source;
	BFSQueue[0] = source;
	Queuehead = 0;
	QueueSize++;
	while(QueueSize != 0)
	{
		OutQueue(n);
	}
	return;
}

void SetRemnantNetwork(int n)
{
	int i = 1;
	while(i <= n)
	{
		int j = 1;
		while(j <= n)
		{
			Remnant[i][j] = 0;
			j++;
		}
		i++;
	}
	i = 1;
	while(i <= n)
	{
		int j = 1;
		while(j <= n)
		{
			Remnant[i][j] += Capacity[i][j] - Flow[i][j];
			Remnant[j][i] += Flow[i][j];
			j++;
		}
		i++;
	}
	return;
}

int FindAugmentPath(int n)
{
	BFS(n);
	int i = 1;
	int father = Father[target];
	if(father != -1)
	{
		int AddFlow = Remnant[father][target];
		int tmp = father;
		while(tmp != source)
		{
			if(Remnant[Father[tmp]][tmp] < AddFlow)
				AddFlow = Remnant[Father[tmp]][tmp];
			tmp = Father[tmp];
		}
		return AddFlow;
	}
	else
		return 0;
}

void RefreshFlow(int flow, int n)
{
	int tmp = target;
	int father = Father[tmp];
	while(tmp != source)
	{
		if(Flow[father][tmp] != 0 && Flow[tmp][father] == 0)
		{
			Flow[father][tmp] += flow; 
		}
		else if(Flow[father][tmp] == 0 && Flow[tmp][father] != 0)
		{
			Flow[tmp][father] -= flow;
		}
		else
		{
			if(Capacity[father][tmp] - Flow[father][tmp] >= flow)
			{
				Flow[father][tmp] += flow; 
			}
			else
			{
				Flow[father][tmp] = Capacity[father][tmp];
				Flow[tmp][father] -= flow - (Capacity[father][tmp] - Flow[father][tmp]);
			}
		}
		tmp = father;
		father = Father[tmp];
	}
	return;
}

int main()
{
	int n, m;
	scanf("%d %d %d %d",&n, &m, &source, &target);
	Initial(n);
	int i = 0;
	while(i < m)
	{
		int v1, v2;
		int capacity;
		scanf("%d %d %d",&v1, &v2, &capacity);
		Capacity[v1][v2] += capacity;
		i++;
	}
	SetRemnantNetwork(n);
	int FLOW = 0;
	int Addition = FindAugmentPath(n);
	while(Addition != 0)
	{
		FLOW+=Addition;
		RefreshFlow(Addition, n);
		SetRemnantNetwork(n);
		Addition = FindAugmentPath(n);
	}
	printf("%d", FLOW);
	return 0;
}
