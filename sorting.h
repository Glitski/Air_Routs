#ifndef SORTING_H_INCLUDED
#define SORTING_H_INCLUDED
#include "defs.h"
#include "graph.h"


// Descrição das funções e das estruturas no ficheiro .c

int LessCusto(Item, Item);
int LessAresta(Item , Item );
int partition(Item *, int l, int r, int (*less) (Item, Item));
void quicksort(Item *, int l, int r, int (*less) (Item, Item));
void CWQUinit(int , int ** , int ** );
int CWQUfind(int ,int  , int *, int *, int *);
void CWQUunion(int ,int , int * ,int *,int, int);



#endif // HEADER_H_INCLUDED