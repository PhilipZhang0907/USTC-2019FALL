#include<stdio.h>
#include<stdlib.h>
#define N 10001
#define K 1000

typedef struct MEM{
	struct ACC* FirstACC;
	int Incache;
}mem;

typedef struct ACC{
	int time;
	struct ACC* NextACC;
}acc;

struct MEM Memory[N];
int Cache[K];
int AccSeq[N];
int CacheUse = 0;
int MISS = 0;
int LatestUse = 0;
int CacheOUT = 0;

void Initial(int n)
{
	int i = 0;
	while(i < n)
	{
		Memory[i].FirstACC = NULL;
		Memory[i].Incache = 0;
		i++;
	}
	return;
}

void AddAcc(int num, int acctime)
{
	struct ACC* tmp = Memory[num].FirstACC;
	if(tmp == NULL)
	{
		Memory[num].FirstACC = (struct ACC*)malloc(sizeof(struct ACC));
		Memory[num].FirstACC->time = acctime;
		Memory[num].FirstACC->NextACC = NULL;
		return;
	}
	else
	{
		while(tmp->NextACC != NULL)
			tmp = tmp->NextACC;
		tmp->NextACC = (struct ACC*)malloc(sizeof(struct ACC));
		tmp->NextACC->time = acctime;
		tmp->NextACC->NextACC = NULL;
		return;
	}
	return;
}

void Refresh(int k)
{
	LatestUse = Cache[0];
	CacheOUT = 0;
	int i = 0;
	while(i < k)
	{
		if(Memory[Cache[i]].FirstACC == NULL)
		{
			LatestUse = Cache[i];
			CacheOUT = i;
			break;
		}
		else if(Memory[LatestUse].FirstACC->time < Memory[Cache[i]].FirstACC->time)
		{
			LatestUse = Cache[i];
			CacheOUT = i;
		}
		i++;
	}
	return;
}

int main()
{
	int n, k;
	scanf("%d %d",&n,&k);
	Initial(N);
	int i = 0;
	while(i < n)
	{
		scanf("%d",AccSeq + i);
		AddAcc(AccSeq[i], i);
		i++;
	}
	i = 0;
	while(i < n)
	{
		int Address = AccSeq[i];
		if(CacheUse < k)
		{
			if(Memory[Address].Incache == 1)
			{
				Memory[Address].FirstACC = Memory[Address].FirstACC->NextACC;
			}
			else
			{
				Memory[Address].Incache = 1;
				Memory[Address].FirstACC = Memory[Address].FirstACC->NextACC;
				Cache[LatestUse] = Address;
				LatestUse++;
				CacheUse++;
				MISS++;
			}
		}
		else
		{
			Refresh(k);
			if(Memory[Address].Incache == 1)
			{
				Memory[Address].FirstACC = Memory[Address].FirstACC->NextACC;
				Refresh(k);
			}
			else
			{
				Memory[Address].Incache = 1;
				Memory[LatestUse].Incache = 0;
				Memory[Address].FirstACC = Memory[Address].FirstACC->NextACC;
				Cache[CacheOUT] = Address;
				MISS++;
				Refresh(k);
			}
		}
		i++;
	}
	printf("%d",MISS);
	return 0;
}
