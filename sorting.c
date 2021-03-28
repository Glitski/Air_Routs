#include <stdio.h>
#include <stdlib.h>
#include "sorting.h"

/*
 *  Data Type: edge
 *
 *  Description: Structure with:
 *      1) Vertice 1 da aresta
 *      2) Vertice 2 da aresta
 *      3) Custo da aresta
 */
struct edge {int v; int w; double custo;};

/*
 *  Function:
 *    LessCusto
 *
 *  Description:
 *    Diz qual a aresta que tem menor custo
 *
 *  Arguments:
 *      -> Aresta A
 *      -> Aresta B
 *    
 *
 *  Return value:
 *      -> True(1) or False(0)
 */
int LessCusto(Item a, Item b)
{
   
   if ((((Edge *) a)->custo) < (((Edge *) b)->custo))
      return 1;
   else
      return 0;
}

/*
 *  Function:
 *    LessAresta
 *
 *  Description:
 *    Diz qual a aresta que tem menor ordem para dar os prints
 *
 *  Arguments:
 *      -> Aresta A
 *      -> Aresta B
 *    
 *
 *  Return value:
 *      -> True(1) or False(0)
 */
int LessAresta(Item a, Item b)
{
   Edge * e1= (Edge *) a,* e2 = (Edge *) b;
   
   if (e1->v < e2->v)
      return 1;
   
   if(e1->v == e2->v)
   {
      if(e1->w < e2->w)
         return 1;
      else
         return 0;
      
   }

   return 0;
}

/*
 *  Function:
 *    partition
 *
 *  Description:
 *    Usada no quicksort, divide uma tabela assegurando que os valores superiores a um valor pivo estão na parte superior e os inferiores na parte inferior
 *
 *  Arguments:
 *      -> Tabela a ser ordenada
 *      -> Valor de inicio
 *      -> Valor de fim
 *       -> Funcao de comparacao a utilizar
 *
 *  Return value:
 *      -> valor do indice pivo
 */
int partition(Item a[], int l, int r, int (*less) (Item, Item))
{
   int i,j;
   Item v,aux;

   v =  a[r];
   i = l-1;
   j=r;
   
   for(;;)
   {
      while (less(a[++i],v));

      while (less(v,a[--j]))
      {
         if(j==l) break;
      }
            
      if (i>= j) break;

      aux = a[i] ;
      a[i] = a[j];
      a[j] = aux;
        
   }

   aux = a[i] ;
   a[i] = a[r];
   a[r] = aux;

   return i;
}

/*
 *  Function:
 *    quicksort
 *
 *  Description:
 *    Implementa o algoritmo de ordenacao quicksort
 *
  *  Arguments:
 *      -> Tabela a ser ordenada
 *      -> Valor de inicio
 *      -> Valor de fim
 *       -> Funcao de comparacao a utilizar
 *
 *  Return value:
 *      -> None
 */
void quicksort(Item * a, int l, int r, int (*less) (Item, Item))
{
   int i;
   if(r<=l) return;
   i = partition(a,l,r,less);
   quicksort(a,l,i-1,less);
   quicksort(a,i+1,r,less);
}

/*
 *  Function:
 *    CWQUinit
 *
 *  Description:
 *    Inicializa as tabelas necessarias ao uso do Compressed Weighted Quick Union
 *
  *  Arguments:
 *      -> Tamanho da tabela
 *      -> tabela de id
 *      -> tabela de size
 *
 *  Return value:
 *      -> None
 */
void CWQUinit(int V, int ** id, int ** sz)
{
   int i;
   *id = (int *) malloc(V*sizeof(int));

   if (id == NULL) // Erro de alocacao
      exit(0);

   *sz = (int *) malloc(V*sizeof(int));

   if (sz == NULL) // Erro de alocacao
      exit(0);

   for (i = 0; i < V; i++) 
   {
      (*id)[i] = i; 
      (*sz)[i] = 1;
   }

   return;
}

/*
 *  Function:
 *    CWQUfind
 *
 *  Description:
 *    Diz se 2 vertices estao conectados
 *
  *  Arguments:
 *      -> vertice 1
 *      -> vertice 2
 *      -> tabela de id
 *      -> raiz do vertice 1
 *      -> raiz do vertice 2
 *
 *  Return value:
 *      -> None
 */
int CWQUfind(int v1,int v2, int * id,int *r1,int*r2)
{
   int i,j;

   for (i = v1; i != id[i]; i = id[i]);
   for (j = v2; j != id[j]; j = id[j]);

   *r1 = i;
   *r2 = j;
   if (i == j)
      return 1;
   else
      return 0;
}

/*
 *  Function:
 *    CWQUunion
 *
 *  Description:
 *    Une 2 vertices
 *
  *  Arguments:
 *      -> vertice 1
 *      -> vertice 2
 *      -> tabela de id
 *       -> tabelad de size
 *       -> raiz do vertice 1
 *       -> raiz do vertice 2
 *
 *  Return value:
 *      -> None
 */
void CWQUunion(int v1,int v2, int * id,int *sz,int r1,int r2)
{
   int i=r1,j=r2,t,x;

   if (sz[i] < sz[j]) 
   {
      id[i] = j; sz[j] += sz[i]; t = j;
   } 
   else 
   {
      id[j] = i; sz[i] += sz[j]; t = i;
   }

   for (i = v1; i != id[i]; i = x) 
   {
      x = id[i];
      id[i] = t;
   }
   for (j = v2; j != id[j]; j = x) 
   {
      x = id[j]; 
      id[j] = t;
   }

   return;
}
