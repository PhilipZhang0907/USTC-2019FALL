#include<stdio.h>
#include<stdlib.h>
#define N 2000000

typedef struct BST{
	int key;
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

struct BST* Find_Succ(struct BST* tmp)
{
	struct BST* ptr = tmp->rchild;
	if(ptr == nul)
	{
		ptr = tmp;
		while(ptr == ptr->parent->rchild)
			ptr = ptr->parent;
		return ptr->parent;
	}
	else
	{
		while(ptr->lchild != nul)
			ptr = ptr->lchild;
		return ptr;
	}
}

struct BST* Find_Node(struct BST* tree, int key_find)
{
	if(tree->key == key_find)
		return tree;
	else if(tree->key > key_find)
	{
		if(tree->lchild != nul)
			return Find_Node(tree->lchild, key_find);
		else
		{
			struct BST* tmp = tree;
			while(tmp == tmp->parent->lchild)
				tmp = tmp->parent;
			return tmp->parent;
		}
	}		
	else
	{
		if(tree->rchild != nul)
			return Find_Node(tree->rchild, key_find);
		else return tree;
	}
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

void Insert_BST(int key_in)
{
	if(root == NULL)
	{
		root = (struct BST*)malloc(1*sizeof(Bst));
		nul = (struct BST*)malloc(1*sizeof(Bst));
		root->key = key_in;
		root->lchild = root->rchild = nul;
		root->parent = NULL;
		root->height = 1;		
		nul->key = -1;
		nul->lchild = nul->rchild = nul->parent = NULL;
		nul->height = 0;
		//printf("init root %d\n",key_in);
		return; 
	}
	struct BST* tmp = root;
	while((key_in < tmp->key && tmp->lchild != nul) || (key_in >= tmp->key && tmp->rchild != nul))
	{
		if(key_in < tmp->key)
			tmp = tmp->lchild;
		else tmp = tmp->rchild;
	}
	if(key_in < tmp->key)// is tmp's lchild
	{
		tmp->lchild = (struct BST*)malloc(1*sizeof(Bst));
		tmp->lchild->key = key_in;
		tmp->lchild->parent = tmp;
		tmp->lchild->lchild = tmp->lchild->rchild = nul;
		tmp->lchild->height = 1;		
	}
	if(key_in >= tmp->key)// is tmp's rchild
	{
		tmp->rchild = (struct BST*)malloc(1*sizeof(Bst));
		tmp->rchild->key = key_in;
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
	//printf("insert %d\n",key_in);
	return;			
}


void Delete_BST(int key_out)
{
	struct BST* tmp = Find_Node(root, key_out);
	struct BST* ptmp = tmp->parent;
	if(tmp->lchild == nul && tmp->rchild == nul)// leaf
	{
		if(tmp->parent != NULL)
		{
			if(tmp == tmp->parent->lchild)
				tmp->parent->lchild = nul;
			else tmp->parent->rchild = nul;			
		}
		else root = NULL;
		while(ptmp != NULL)
		{
			if(ptmp->lchild->height - ptmp->rchild->height == 2)
				LBalance(ptmp);
			else if(ptmp->rchild->height - ptmp->lchild->height == 2)
				RBalance(ptmp);
			ptmp->height = max(ptmp->lchild->height, ptmp->rchild->height) + 1;
			ptmp = ptmp->parent;		
		}
		free(tmp);
		//printf("delete %d\n",key_out);
		return;
	}
	else
	{
		if(tmp->lchild == nul)
		{
			if(tmp->parent != NULL)
			{
				if(tmp == tmp->parent->lchild)
				{
					tmp->parent->lchild = tmp->rchild;
					tmp->rchild->parent = tmp->parent;
				}
				else
				{
					tmp->parent->rchild = tmp->rchild;
					tmp->rchild->parent = tmp->parent;
				}
			}
			else
			{
				root = tmp->rchild;
				tmp->rchild->parent = NULL;
			}
			while(ptmp != NULL)
			{
				if(ptmp->lchild->height - ptmp->rchild->height == 2)
					LBalance(ptmp);
				else if(ptmp->rchild->height - ptmp->lchild->height == 2)
					RBalance(ptmp);
				ptmp->height = max(ptmp->lchild->height, ptmp->rchild->height) + 1;
				ptmp = ptmp->parent;		
			}
			free(tmp);
			//printf("delete %d\n",key_out);
			return;
		}
		else if(tmp->rchild == nul)
		{
			if(tmp->parent != NULL)
			{
				if(tmp == tmp->parent->lchild)
				{
					tmp->parent->lchild = tmp->lchild;
					tmp->lchild->parent = tmp->parent;
				}
				else
				{
					tmp->parent->rchild = tmp->lchild;
					tmp->lchild->parent = tmp->parent;
				}
			}
			else
			{
				root = tmp->lchild;
				tmp->lchild->parent = NULL;
			}
			while(ptmp != NULL)
			{
				if(ptmp->lchild->height - ptmp->rchild->height == 2)
					LBalance(ptmp);
				else if(ptmp->rchild->height - ptmp->lchild->height == 2)
					RBalance(ptmp);
				ptmp->height = max(ptmp->lchild->height, ptmp->rchild->height) + 1;
				ptmp = ptmp->parent;		
			}
			free(tmp);
			//printf("delete %d\n",key_out);
			return;
		}
		else
		{
			struct BST* ptr = Find_Succ(tmp);
			tmp->key = ptr->key;
			ptmp = ptr;
			if(ptr == tmp->rchild)
			{
				tmp->rchild = ptr->rchild;
				if(ptr->rchild != NULL)
					ptr->rchild->parent = tmp;
			}
			else
			{
				ptr->parent->lchild = ptr->rchild;
				if(ptr->rchild != NULL)
					ptr->rchild->parent = ptr->parent;
			}
			ptmp = ptr->parent;
			while(ptmp != NULL)
			{
				if(ptmp->lchild->height - ptmp->rchild->height == 2)
					LBalance(ptmp);
				else if(ptmp->rchild->height - ptmp->lchild->height == 2)
					RBalance(ptmp);
				ptmp->height = max(ptmp->lchild->height, ptmp->rchild->height) + 1;
				ptmp = ptmp->parent;		
			}
			free(ptr);
			//printf("delete %d\n",key_out);
			return;
		}
	}
}

int Find_KEY(int num)
{
	struct BST* tmp = Find_Node(root, num);
	return tmp->key;
}

int main()
{
	root = NULL;
	nul = NULL;
	char opcode[8];
	int i = 0;
	while(i < N)
	{
		int key;
		scanf("%s", opcode);
		if(opcode[0] == 'I')
		{
			scanf("%d",&key);
			Insert_BST(key);
		}
		else if(opcode[0] == 'D')
		{
			scanf("%d",&key);
			Delete_BST(key);
		}
		else if(opcode[0] == 'F')
		{
			scanf("%d",&key);
			int j = Find_KEY(key);
			printf("%d\n",j);
		}
		else if(opcode[0] == 'E')
			break;
		i++;
	}
	return 0;
}
