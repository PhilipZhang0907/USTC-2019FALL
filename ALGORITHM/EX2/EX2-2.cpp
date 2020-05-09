#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define N 13000000 

int NUM[N];
int APPK[N] = {0};
int APPO[N] = {0};
int First_K = 0;
int contain = 0;
int start = 0;
int end = 0;

int fac(int num, int power)
{
	int val = num;
	if(power == 0)
		return 1;
	int i = 1;
	while(i < power)
	{
		val *= num;
		i++;
	}
	return val;
}

int Name_to_Num(char* name)
{
	int num = 0;
	int i = 0;
	while(i < strlen(name))
	{
		num += fac(26,4-i)*(*(name + i) - 'a');
		i++;
	}
	return num + 1;
}

int DO_CAL(int *NUM)
{
	int answer = 0;
	while(APPO[NUM[start]] != 1)
	{
		answer += end - First_K;
		if(APPK[NUM[start]] == 1)
		{
			while(NUM[First_K] != NUM[start])
			{
				First_K++;
				APPK[NUM[First_K]]++;
			}
			APPK[NUM[start]]--;
			APPO[NUM[start]]--;
			start++;
		}
		else
		{
			APPK[NUM[start]]--;
			APPO[NUM[start]]--;
			start++;
		}
	}
	answer += end - First_K;
	return answer;
}

int FIND_WINDOW(int *NUM,int k)
{
	int final = end;
	while(NUM[final] != -1)
	{
		if(APPO[NUM[final]] == 0)
		{
			if(contain < k)
			{
				APPK[NUM[final]]++;
				APPO[NUM[final]]++;
				contain++;
				if(contain == k)
				{
					First_K = final;
					final++;
					continue;
				}
				final++;
			}
			else	break;
		}
		else
		{
			if(contain < k)
			{
				APPK[NUM[final]]++;
				APPO[NUM[final]]++;
				final++;
			}
			else
			{
				APPO[NUM[final]]++;
				final++;
			}
		}
	}
	return final;
}

int SLIDE_WINDOW(int* NUM, int n, int k)
{
	start = end = 0;
	int answer = 0;
	while(NUM[end] != -1)
	{
		end = FIND_WINDOW(NUM, k);
		answer += DO_CAL(NUM);
		if(NUM[end] == -1)
			return answer;
		else
		{
			APPK[NUM[start]]--;
			APPO[NUM[start]]--;
			start++;
			APPO[NUM[end]]++;
			while(First_K < end)
			{
				First_K++;
				APPK[NUM[First_K]]++;
			}
			end++;
		}
	}
	while(APPO[NUM[start]] != 1)
	{
		answer += 1;
		start++;
	}
	answer += 1;
	return answer;
}

int main()
{
	int n,k;
	scanf("%d %d",&n,&k);
	int i = 0;
	char name[6];
	while(i < n)
	{
		scanf("%s",name);
		int num = Name_to_Num(name);
		NUM[i] = num;
		if(num < 0)
			printf("wrong answer.");
		i++;
	}
	NUM[i] = -1;
	int answer = SLIDE_WINDOW(NUM, n, k);
	printf("%d",answer);
	return 0;
}
