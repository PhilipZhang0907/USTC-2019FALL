#include<stdio.h>
#include<stdlib.h>
#define N 1000000
/*
                       ::
                      :;J7, :,                        ::;7:
                      ,ivYi, ,                       ;LLLFS:
                      :iv7Yi                       :7ri;j5PL
                     ,:ivYLvr                    ,ivrrirrY2X,
                     :;r@Wwz.7r:                :ivu@kexianli.
                    :iL7::,:::iiirii:ii;::::,,irvF7rvvLujL7ur
                   ri::,:,::i:iiiiiii:i:irrv177JX7rYXqZEkvv17
                ;i:, , ::::iirrririi:i:::iiir2XXvii;L8OGJr71i
              :,, ,,:   ,::ir@mingyi.irii:i:::j1jri7ZBOS7ivv,
                 ,::,    ::rv77iiiriii:iii:i::,rvLq@huhao.Li
             ,,      ,, ,:ir7ir::,:::i;ir:::i:i::rSGGYri712:
           :::  ,v7r:: ::rrv77:, ,, ,:i7rrii:::::, ir7ri7Lri
          ,     2OBBOi,iiir;r::        ,irriiii::,, ,iv7Luur:
        ,,     i78MBBi,:,:::,:,  :7FSL: ,iriii:::i::,,:rLqXv::
        :      iuMMP: :,:::,:ii;2GY7OBB0viiii:i:iii:i:::iJqL;::
       ,     ::::i   ,,,,, ::LuBBu BBBBBErii:i:i:i:i:i:i:r77ii
      ,       :       , ,,:::rruBZ1MBBqi, :,,,:::,::::::iiriri:
     ,               ,,,,::::i:  @arqiao.       ,:,, ,:::ii;i7:
    :,       rjujLYLi   ,,:::::,:::::::::,,   ,:i,:,,,,,::i:iii
    ::      BBBBBBBBB0,    ,,::: , ,:::::: ,      ,,,, ,,:::::::
    i,  ,  ,8BMMBBBBBBi     ,,:,,     ,,, , ,   , , , :,::ii::i::
    :      iZMOMOMBBM2::::::::::,,,,     ,,,,,,:,,,::::i:irr:i:::,
    i   ,,:;u0MBMOG1L:::i::::::  ,,,::,   ,,, ::::::i:i:iirii:i:i:
    :    ,iuUuuXUkFu7i:iii:i:::, :,:,: ::::::::i:i:::::iirr7iiri::
    :     :rk@Yizero.i:::::, ,:ii:::::::i:::::i::,::::iirrriiiri::,
     :      5BMBBBBBBSr:,::rv2kuii:::iii::,:i:,, , ,,:,:i@petermu.,
          , :r50EZ8MBBBBGOBBBZP7::::i::,:::::,: :,:,::i;rrririiii::
              :jujYY7LS0ujJL7r::,::i::,::::::::::::::iirirrrrrrr:ii:
           ,:  :@kevensun.:,:,,,::::i:i:::::,,::::::iir;ii;7v77;ii;i,
           ,,,     ,,:,::::::i:iiiii:i::::,, ::::iiiir@xingjief.r;7:i,
        , , ,,,:,,::::::::iiiiiiiiii:,:,:::::::::iiir;ri7vL77rrirri::
         :,, , ::::::::i:::i:::i:i::,,,,,:,::i:i:::iir;@Secbone.ii:::
*/ 
typedef struct NODE{
	int num;
	struct NODE* Lchild;
	struct NODE* Rchild;
	struct NODE* DATEsuc;// successor according to date
	struct NODE* parent;
}Node;
struct NODE* ptrOLDEST;// always point at the oldest node
struct NODE* ptrNEWEST;// always point at the newest node
struct NODE* ptrGREATEST;// always point at the greatest node
struct NODE* ptrROOT = NULL;// mark the root of BST
int dataCOUNT = 0;
int lookAHEAD;

struct NODE* FIND_SUCCESSOR(struct NODE* ptr)
{
	struct NODE* ptrTEM = ptr->Rchild;
	while(ptrTEM->Lchild != NULL)
	{
		ptrTEM = ptrTEM->Lchild;
	}
	return ptrTEM;	
}

void DELETE_NODE(struct NODE* ptr)// PAY ATTENTION TO EVERY POINTER!!! 
{
	ptrOLDEST = ptr->DATEsuc;
	if(ptr == ptrGREATEST)
	{
		if(ptr->Lchild == NULL)
		{
			ptrGREATEST = ptr->parent;
		}
		else
		{
			struct NODE* ptrTEM = ptr->Lchild;
			while(ptrTEM->Rchild != NULL)
				ptrTEM = ptrTEM->Rchild;
			ptrGREATEST = ptrTEM;
		}
	}// ajust ptrGREATEST
	if(ptr->Lchild == NULL && ptr->Rchild == NULL)
	{
		if(ptr->parent == NULL)
		{
			free(ptr);
			return;
		}
		if(ptr == ptr->parent->Lchild)
			ptr->parent->Lchild = NULL;
		if(ptr == ptr->parent->Rchild)
			ptr->parent->Rchild = NULL;
		free(ptr);
		return;
	}// if ptr has no child
	if(ptr->Lchild == NULL)//if ptr only has a right child
	{
		if(ptr->parent == NULL)
		{
			ptrROOT = ptr->Rchild;
			ptr->Rchild->parent = NULL;
			return;
		}
		if(ptr == ptr->parent->Lchild)
		{
			ptr->parent->Lchild = ptr->Rchild;
			ptr->Rchild->parent = ptr->parent;
			free(ptr);
			return;
		}
		if(ptr == ptr->parent->Rchild)
		{
			ptr->parent->Rchild = ptr->Rchild;
			ptr->Rchild->parent = ptr->parent;
			free(ptr);
			return;
		}
	}
	if(ptr->Rchild == NULL)// if ptr only has a left child
	{
		if(ptr->parent == NULL)
		{
			ptrROOT = ptr->Lchild;
			ptr->Lchild->parent = NULL;
			return;
		}
		if(ptr == ptr->parent->Lchild)
		{
			ptr->parent->Lchild = ptr->Lchild;
			ptr->Lchild->parent = ptr->parent;
			free(ptr);
			return;
		}
		if(ptr == ptr->parent->Rchild)
		{
			ptr->parent->Rchild = ptr->Lchild;
			ptr->Lchild->parent = ptr->parent;
			free(ptr);
			return;
		}
	}
	struct NODE* ptrSUC = FIND_SUCCESSOR(ptr);
	if(ptrSUC == ptr->Rchild)
	{
		ptrSUC->Lchild = ptr->Lchild;
		ptr->Lchild->parent = ptrSUC;		
		if(ptr->parent != NULL)
		{
			if(ptr == ptr->parent->Lchild)
			{
				ptr->parent->Lchild = ptrSUC;
			}
			else ptr->parent->Rchild = ptrSUC;
		}
		else ptrROOT = ptrSUC;
		ptrSUC->parent = ptr->parent;
	}
	else
	{
		if(ptrSUC->Rchild != NULL)
		{
			ptrSUC->parent->Lchild = ptrSUC->Rchild;
			ptrSUC->Rchild->parent = ptrSUC->parent;
		}
		ptrSUC->Lchild = ptr->Lchild;
		ptr->Lchild->parent = ptrSUC;
		ptrSUC->Rchild = ptr->Rchild;
		ptr->Rchild->parent = ptrSUC;
		if(ptr->parent != NULL)
		{
			if(ptr == ptr->parent->Lchild)
			{
				ptr->parent->Lchild = ptrSUC;
			}
			else ptr->parent->Rchild = ptrSUC;
		}
		else ptrROOT = ptrSUC;
		ptrSUC->parent = ptr->parent;
	}
	free(ptr);
	return;
}

void INSERT_NODE(struct NODE* root, int value)
{
	if(ptrROOT == NULL)
	{
		ptrROOT = (struct NODE*)malloc(1*sizeof(Node));
		ptrROOT->num = value;
		ptrROOT->Lchild = NULL;
		ptrROOT->Rchild = NULL;
		ptrROOT->DATEsuc = NULL;
		ptrROOT->parent = NULL;
		ptrNEWEST = ptrROOT;
		ptrOLDEST = ptrROOT;
		ptrGREATEST = ptrROOT;
		return;
	}
	struct NODE* ptrTEM = ptrROOT;
	while((value<ptrTEM->num && ptrTEM->Lchild!=NULL)||(value>=ptrTEM->num && ptrTEM->Rchild!=NULL))
	{
		if(value < ptrTEM->num)
		{
			ptrTEM = ptrTEM->Lchild;
		}
		else if(value >= ptrTEM->num)
		{
			ptrTEM = ptrTEM->Rchild;
		}
	}
	if(value < ptrTEM->num)
	{
		ptrTEM->Lchild = (struct NODE*)malloc(1*sizeof(Node));
		ptrTEM->Lchild->parent = ptrTEM;
		ptrTEM->Lchild->num = value;
		ptrTEM->Lchild->Lchild = ptrTEM->Lchild->Rchild = NULL;
		ptrTEM->Lchild->DATEsuc = NULL;
		ptrNEWEST->DATEsuc = ptrTEM->Lchild;
		ptrNEWEST = ptrTEM->Lchild;
	}
	if(value >= ptrTEM->num)
	{
		ptrTEM->Rchild = (struct NODE*)malloc(1*sizeof(Node));
		ptrTEM->Rchild->parent = ptrTEM;
		ptrTEM->Rchild->num = value;
		ptrTEM->Rchild->Lchild = ptrTEM->Rchild->Rchild = NULL;
		ptrTEM->Rchild->DATEsuc = NULL;
		ptrNEWEST->DATEsuc = ptrTEM->Rchild;
		ptrNEWEST = ptrTEM->Rchild;
		if(ptrGREATEST == ptrTEM)
		{
			ptrGREATEST = ptrTEM->Rchild;
		}
	}
	dataCOUNT++;
	if(dataCOUNT > lookAHEAD-1)
	{
		DELETE_NODE(ptrOLDEST);
	}
	return;
}

int main()
{
	int n;
	scanf("%d %d",&n,&lookAHEAD);
	int i = 0;
	int value = -1;
	while(i < n)
	{
		scanf("%d",&value);
		INSERT_NODE(ptrROOT, value);
		if(dataCOUNT>=lookAHEAD-1 && dataCOUNT<=n)
			printf("%d ",ptrGREATEST->num);
		i++;
	}
	return 0;
}
