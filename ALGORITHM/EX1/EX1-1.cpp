#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define M 256313
typedef struct Data{
	int id;
	int attr;
	struct Data* Nlode;
}DATA;

DATA HASHTABLE[M];

void HASHMAP(DATA hash[], int tmpid, int tmpattr)
{
	int num = tmpattr%M;
	if(hash[num].id == -1)
	{
		hash[num].id = tmpid;
		hash[num].attr = tmpattr;
	}
	else{
		struct Data* ptr = hash[num].Nlode;
		while(ptr != NULL)
			ptr = ptr->Nlode;
		ptr = (struct Data*)malloc(1*sizeof(DATA));
		ptr->attr = tmpattr;
		ptr->id = tmpid;
		ptr->Nlode = hash[num].Nlode;
		hash[num].Nlode = ptr;
	}
	return;
}

int main()
{
	int i = 0;
	while(i < M)
	{
		HASHTABLE[i].attr = -1;
		HASHTABLE[i].id = -1;
		HASHTABLE[i].Nlode = NULL;
		i++;
	}
	int tmpid;
	int tmpattr;
	char cmd[7];
	while(cmd[0]!='E')
	{
		scanf("%s", cmd);
		switch(cmd[0])
		{
			case 'I':{
				scanf("%d %d",&tmpid, &tmpattr);
				HASHMAP(HASHTABLE, tmpid, tmpattr);
				break;
			}
			case 'F':{
				scanf("%d", &tmpattr);
				struct Data* ptr = HASHTABLE + tmpattr%M;
				while(ptr->attr!=tmpattr)
				{
					ptr = ptr->Nlode;
					if(ptr == NULL)
						break;
				}
				if(ptr == NULL)
					printf("-1\n");
				else printf("%d\n", ptr->id);
				break;
			}
			default:break;
		}
	}
	return 0;
}
