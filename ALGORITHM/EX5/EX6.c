#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LENPATTERN 10000
#define LENSTRING 1000000

void KMPPrefix(char pattern[], int prefix[], int len)
{
	prefix[0] = 0;
	int k = 0;
	int i = 2;
	while(i <= len)
	{
		while(k>0 && pattern[k]!=pattern[i-1])
		{
			k = prefix[k-1];
		}
		if(pattern[k]==pattern[i-1])
		{
			k++;
		}
		prefix[i-1] = k;
		i++;
	}
	return;
}

int main()
{
	int T;
	scanf("%d", &T);
	int i = 0;
	while(i < T)
	{
		int Occurance = 0;
		char PATTERN[LENPATTERN];
		int PREFIX[LENPATTERN];
		char STRING[LENSTRING];
		scanf("%s %s",PATTERN, STRING);
		int len = strlen(PATTERN);
		int slen = strlen(STRING);
		KMPPrefix(PATTERN, PREFIX, len);
		int q = 0;
		int j = 1;
		while(j <= slen)
		{
			while(q > 0 && PATTERN[q] != STRING[j-1])
				q = PREFIX[q-1];
			if(PATTERN[q] == STRING[j-1])
			{
				q++;
				if(q == len)
				{
					Occurance++;
					q = PREFIX[q-1];
				}
			}
			j++;
		}
		printf("%d\n",Occurance);
		i++;
	}
	return 0;
}
