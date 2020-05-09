#include<stdio.h>
#include<stdlib.h>
#define N 100000
#define R 10000002
#define P 1000

typedef struct NODE{
	int ArriveTime;
	int TimeLeft;
	struct NODE* Lchild;
	struct NODE* Rchild;
	struct NODE* Parent;
}node;

typedef struct PROCESS{
	int ArriveTime;
	int ProcessTime;
	struct PROCESS* Next;
}process;

typedef struct TIME{
	int time;
	struct PROCESS* Arrival;
}time;

struct TIME Time[R];
struct NODE* Root = NULL;
int RESULT = 0;
int FLAG = 0;
int LastArrive = 0;

void Initial()
{
	int i = 0;
	while(i < R)
	{
		Time[i].time = i;
		Time[i].Arrival = NULL;
		i++;
	}
	return;
}

void Insert(int arrive, int process)
{
	if(Root == NULL)
	{
		Root = (struct NODE*)malloc(sizeof(struct NODE));
		Root->ArriveTime = arrive;
		Root->TimeLeft = process;
		Root->Lchild = NULL;
		Root->Rchild = NULL;
		Root->Parent = NULL;
		return;
	}
	else
	{
		struct NODE* tmp = Root;
		while(process < tmp->TimeLeft && tmp->Lchild != NULL || process >= tmp->TimeLeft && tmp->Rchild != NULL)
		{
			if(process < tmp->TimeLeft)
				tmp = tmp->Lchild;
			else if(process >= tmp->TimeLeft)
				tmp = tmp->Rchild;
		}
		if(process < tmp->TimeLeft)
		{
			tmp->Lchild = (struct NODE*)malloc(sizeof(struct NODE));
			tmp->Lchild->Lchild = NULL;
			tmp->Lchild->Rchild = NULL;
			tmp->Lchild->Parent = tmp;
			tmp->Lchild->ArriveTime = arrive;
			tmp->Lchild->TimeLeft = process;
			return;
		}
		else
		{
			tmp->Rchild = (struct NODE*)malloc(sizeof(struct NODE));
			tmp->Rchild->Lchild = NULL;
			tmp->Rchild->Rchild = NULL;
			tmp->Rchild->Parent = tmp;
			tmp->Rchild->ArriveTime = arrive;
			tmp->Rchild->TimeLeft = process;
			return;
		}
	}
	return;
}

void Delete(struct NODE* ptr)
{
	if(ptr->Parent == NULL)
	{
		Root = ptr->Rchild;
		if(ptr->Rchild != NULL)
			ptr->Rchild->Parent = NULL;
		return;
	}
	else
	{
		ptr->Parent->Lchild = ptr->Rchild;
		if(ptr->Rchild != NULL)
			ptr->Rchild->Parent = ptr->Parent;
		return;
	}
}

void Execute(int time)
{
	struct NODE* tmp = Root;
	if(tmp == NULL)
	{
		return;
	}
	while(tmp->Lchild != NULL)
	{
		tmp = tmp->Lchild;
	} 
	tmp->TimeLeft --;
	if(tmp->TimeLeft == 0)
	{
		RESULT += (time - tmp->ArriveTime + 1);
		Delete(tmp);
		if(Root == NULL && time >= LastArrive)
			FLAG = 1;
	}
	return;
}

void AddArrival(int time, int process)
{
	if(Time[time].Arrival == NULL)
	{
		Time[time].Arrival = (struct PROCESS*)malloc(sizeof(struct PROCESS));
		Time[time].Arrival->ProcessTime = process;
		Time[time].Arrival->Next = NULL;
	}
	else
	{
		struct PROCESS* tmp = Time[time].Arrival;
		while(tmp->Next != NULL)
			tmp = tmp->Next;
		tmp->Next = (struct PROCESS*)malloc(sizeof(struct PROCESS));
		tmp->Next->ProcessTime = process;
		tmp->Next->Next = NULL;
	}
	return;
}

void AddNode(int time)
{
	struct PROCESS* tmp = Time[time].Arrival;
	while(tmp != NULL)
	{
		Insert(time, tmp->ProcessTime);
		tmp = tmp->Next;
	}
	return;
}

int main()
{
	Initial();
	int n;
	scanf("%d",&n);
	int i = 0;
	while(i < n)
	{
		int arrive;
		int process;
		scanf("%d %d",&arrive, &process);
		if(LastArrive < arrive)
			LastArrive = arrive;
		AddArrival(arrive, process);
		i++;
	}
	i = 0;
	while(1)
	{
		AddNode(i);
		Execute(i);
		if(FLAG == 1)
			break;
		i++;
	}
	printf("%d",RESULT);
	return 0;
}
