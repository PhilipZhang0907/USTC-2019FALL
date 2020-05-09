#include<stdio.h>
#include<stdlib.h>
#define N 2000000

typedef struct BST{
	int min;
	int max;
	struct BST* lchild;
	struct BST* rchild;
	struct BST* parent;
	int height;
}Bst;

struct BST* root;
struct BST* nul;

int max(int a, int b)
{
	if(a > b)
		return a;
	else return b;
}

struct BST* LL(struct BST* ptr)
{
	struct BST* ltmp = ptr->lchild;
	ptr->lchild = ptr->lchild->rchild;
	ptr->lchild->parent = ptr;
	ltmp->rchild = ptr;
	if(ptr->parent != NULL)
	{
		ltmp->parent = ptr->parent;
		if(ptr == ptr->parent->lchild)
			ptr->parent->lchild = ltmp;
		else ptr->parent->rchild = ltmp;
		ptr->parent = ltmp;
	}
	else
	{
		ltmp->parent = NULL;
		root = ltmp;
		ptr->parent = ltmp;
	}
	//printf("LL %d\n",ptr->key);
	return ltmp;
}

struct BST* RR(struct BST* ptr)
{
	struct BST* rtmp = ptr->rchild;
	ptr->rchild = ptr->rchild->lchild;
	ptr->rchild->parent = ptr;
	rtmp->lchild = ptr;
	if(ptr->parent != NULL)
	{
		rtmp->parent = ptr->parent;
		if(ptr == ptr->parent->lchild)
			ptr->parent->lchild = rtmp;
		else ptr->parent->rchild = rtmp;
		ptr->parent = rtmp;
	}
	else
	{
		rtmp->parent = NULL;
		root = rtmp;
		ptr->parent = rtmp;
	}
	//printf("RR %d\n",ptr->key);
	return rtmp;
}

struct BST* LR(struct BST* ptr)
{
	ptr->lchild = RR(ptr->lchild);
	return LL(ptr);
}

struct BST* RL(struct BST* ptr)
{
	ptr->rchild = LL(ptr->rchild);
	return RR(ptr);
}

void LBalance(struct BST* ptr)
{
	struct BST* tmp = ptr;
	if(tmp->lchild->lchild->height > tmp->lchild->rchild->height)
	{
		tmp = LL(tmp);
		tmp->rchild->height = max(tmp->rchild->lchild->height, tmp->rchild->rchild->height) + 1;
		return;
	}
	else
	{
		tmp = LR(tmp);
		tmp->rchild->height = max(tmp->rchild->lchild->height, tmp->rchild->rchild->height) + 1;
		tmp->lchild->height = max(tmp->lchild->lchild->height, tmp->lchild->rchild->height) + 1;
		return;
	}
}

void RBalance(struct BST* ptr)
{
	struct BST* tmp = ptr;
	if(tmp->rchild->rchild->height > tmp->rchild->lchild->height)
	{
		tmp = RR(tmp);
		tmp->lchild->height = max(tmp->lchild->lchild->height, tmp->lchild->rchild->height) + 1;
		return;
	}
	else
	{
		tmp = RL(tmp);
		tmp->rchild->height = max(tmp->rchild->lchild->height, tmp->rchild->rchild->height) + 1;
		tmp->lchild->height = max(tmp->lchild->lchild->height, tmp->lchild->rchild->height) + 1;
		return;
	}
}

int Insert_BST(int pmin, int pmax)
{
	if(root == NULL)
	{
		root = (struct BST*)malloc(1*sizeof(Bst));
		nul = (struct BST*)malloc(1*sizeof(Bst));
		root->min = pmin;
		root->max = pmax;
		root->lchild = root->rchild = nul;
		root->parent = NULL;
		root->height = 1;		
		nul->min = -1;
		nul->max = -1;
		nul->lchild = nul->rchild = nul->parent = NULL;
		nul->height = 0;
		//printf("init root %d\n",key_in);
		return 0; 
	}
	struct BST* tmp = root;
	while((pmin > tmp->max && tmp->rchild != nul) || (pmax < tmp->min && tmp->lchild != nul))
	{
		if(pmax < tmp->min)
			tmp = tmp->lchild;
		else tmp = tmp->rchild;
	}
	if(pmin <= tmp->min && pmax >= tmp->min || pmin <= tmp->max && pmax >= tmp->max || pmin >= tmp->min && pmax <= tmp->max)
		return -1;
	if(pmax < tmp->min)// is tmp's lchild
	{
		tmp->lchild = (struct BST*)malloc(1*sizeof(Bst));
		tmp->lchild->min = pmin;
		tmp->lchild->max = pmax;
		tmp->lchild->parent = tmp;
		tmp->lchild->lchild = tmp->lchild->rchild = nul;
		tmp->lchild->height = 1;		
	}
	if(pmin > tmp->max)// is tmp's rchild
	{
		tmp->rchild = (struct BST*)malloc(1*sizeof(Bst));
		tmp->rchild->min = pmin;
		tmp->rchild->max = pmax;
		tmp->rchild->parent = tmp;
		tmp->rchild->lchild = tmp->rchild->rchild = nul;
		tmp->rchild->height = 1;
	}
	while(tmp != NULL)
	{
		if(tmp->lchild->height - tmp->rchild->height == 2)
			LBalance(tmp);
		else if(tmp->rchild->height - tmp->lchild->height == 2)
			RBalance(tmp);
		tmp->height = max(tmp->lchild->height, tmp->rchild->height) + 1;
		tmp = tmp->parent;		
	}
	return 0;			
}

int main()
{
	root = NULL;
	nul = NULL;
	int n;
	scanf("%d",&n);
	int i = 0;
	while(i < n)
	{
		int min;
		int max;
		scanf("%d %d",&min,&max);
		int result = Insert_BST(min, max);
		printf("%d\n",result);
		i++;
	}
	return 0;
}
