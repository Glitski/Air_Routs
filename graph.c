#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

/*
 *  Data Type: node
 *
 *  Description: 
 *  Elemento da lista
 * 
 *  Structure with:
 *      1) Vertice
 *      2) Custo da aresta
 *      3) Ponteiro para o próximo vertice adjacente
 */
struct node {int v; double custo ; link *next;};

/*
 *  Data Type: graph
 *  
 * 
 *  Description: 
 *      Estrutura que representa um grafo geral (no nosso caso representa o backbone)
 * 
 * 
 * Structure with:
 *      1) Numero total de vertices
 *      2) Numero total de arestas
 *      3) Ponteiro para tabela de listas de adjacencias
 *      4) Numero total de conexoes do backbone
 *      5) Custo total do backbone
 */
struct graph{int V; int E; link **adj; int conn; double cusTotal;};

/*
 *  Data Type: edge
 *
 *  Description: 
 *      Estrutura que representa uma aresta
 * 
 * 
 * Structure with:
 *      1) Vertice 1 da aresta
 *      2) Vertice 2 da aresta
 *      3) Custo da aresta
 */
struct edge {int v; int w; double custo;};

/*
 *  Data Type: backbone
 *
 *  Description: 
 *      Estrutura que representa a conectividade no backbone
 * 
 * Structure with:
 *      1) Tabela de id
 *      2) Tabela de size
 */
struct backbone {int *id ; int *sz;};

/*
 *  Function:
 *    NewSorted
 *
 *  Description:
 *    Cria o no de uma lista
 *
 *  Arguments:
 *      -> Vertice a inserir
 *      -> Custo
 *      -> Ponteiro para o proximo no
 *
 *  Return value:
 *      -> Retorna um ponteiro para o no criado
 */
link *NewSorted(int v, double custo, link *head) {

    link *x = (link *) malloc(sizeof(link));    // Cria no da lista

    if (x == NULL) // Erro de alocacao
        exit(0);

    link *aux = head, *aux2;

    x->v = v;       // Atribui valores
    x->custo = custo;

    if (head == NULL) {     // Lista vazia, insere no inicio
        x->next = NULL;
        return x;
    }
   
    aux2 = aux;
    while (aux != NULL && x->v > aux->v) {      // Percorre a lista ate encontrar o lugar certo
        aux2 = aux;
        aux = aux->next;
    }
    x->next = aux;

    if (aux2 == aux)    // Se a lista nao estiver vazia mas o elemento vai para a primeira posição
        return x;
    aux2->next = x;

    return head;
}

/*
 *  Function:
 *    GraphInit
 *
 *  Description:
 *    Inicia o grafo
 *
 *  Arguments:
 *      -> Numero total de vertices
 *      -> Numero total de arestas
 *    
 *
 *  Return value:
 *      -> Pointer para o grafo criado
 */
Graph *GraphInit(int V,int E) {

    Graph *G = (Graph*) malloc(sizeof(struct graph));

    if (G == NULL) // Erro de alocacao
        exit(0);

    G->V = V;       // Valores iniciais do grafo
    G->E = E;
    G->cusTotal = 0;
    G->conn = 0;
    G->adj = (link **) malloc((G->V) * sizeof(link *));

    if (G->adj == NULL) // Erro de alocacao
      exit(0);
    
    for (int i = 0; i < G->V; i++) {
        G->adj[i] = NULL;
    }
    return G;
}

/*
 *  Function:
 *    GraphInsertE
 *
 *  Description:
 *    Introduz uma aresta no grafo
 *
 *  Arguments:
 *      -> Grafo
 *      -> Aresta a inserir
 *   
 *
 *  Return value:
 *      -------
 */

void GraphInsertE(Graph *G, Edge *e) {
    G->cusTotal += e->custo;
    G->conn ++;

    if (e->v < e->w)        // Como apenas representamos a aresta num dos vertices (para nao a repetir) temos de ver qual o vertice menor
        G->adj[e->v] = NewSorted(e->w, e->custo, G->adj[e->v]);
    else
        G->adj[e->w] = NewSorted(e->v, e->custo, G->adj[e->w]);
    return;
}
/*
 *  Function:
 *    freegraph
 *
 *  Description:
 *    Da free da memoria alocada associada ao grafo
 *
 *  Arguments:
 *      -> Grafo a dar free
 *      
 *
 *  Return value:
 *      ---------------
 */
void freegraph(Graph *G) {
    link *aux, *aux2;
    for (int i = 0; i < G->V; i++) {
        aux = G->adj[i];
        while (aux != NULL) 
        {
            aux2 = aux->next;
            free (aux);
            aux = aux2;
        }
    }
    free(G->adj);
    free(G);
}

/*
 *  Function:
 *    freeBackbone
 *
 *  Description:
 *    Da free da memoria alocada associada a conectividade do backbone
 *
 *  Arguments:
 *      -> Bacbkone a dar free
 *      
 *
 *  Return value:
 *      ---------------
 */
void freeBackbone(Backbone *B)
{
    free(B->id);
    free(B->sz);
    free(B);
}

/*
 *  Function:
 *    freeArestas
 *
 *  Description:
 *    Da free da tabela de arestas
 * 
 * 
 *  Arguments:
 *      ->Arestas a dar free 
 *      
 *
 *  Return value:
 *      ---------------
 */
void freeArestas (Edge ** arestas, int E)
{
    for(int i = 0;i<E;i++)
    {
        free(arestas[i]);
    }

    free (arestas);
}

/*
 *  Function:
 *    verifyEdge
 *
 *  Description:
 *    Verifica se a aresta tem vertices invalidos e se a mesma pertence ao grafo
 *
 *  Arguments:
 *      -> Tabela de arestas
 *      -> v1
 *      -> v2
 *      -> Grafo
 *      
 *
 *  Return value:
 *      True or False
 */
int verifyEdge(Edge **arestas, int v1, int v2, Graph * G) {

    if(!(v1 >=0 && v1<= G->V-1 && v2 >=0 && v2<= G->V-1))       // Vertice invalido ou valido
        return 0;

    for (int i = 0; i < G->E; i++) {        // Ver se pertence a tabela de arestas
        if ((arestas[i]->v == v1 && arestas[i]->w == v2) || (arestas[i]->v == v2 && arestas[i]->w == v1))
            return 1;
    }
    return 0;
}

/*
 *  Function:
 *    BackboneInit
 *
 *  Description:
 *    Inicializa as tabelas relativas a conectividade do backbone
 *
 *  Arguments:
 *      -> Numero de vertces
 *      
 *
 *  Return value:
 *      Backbone criado
 */
Backbone * BackboneInit(int V)
{
    Backbone * B;
    int i;
    B = (Backbone *) malloc(sizeof(Backbone));

    if (B == NULL) // Erro de alocacao
      exit(0);

    B->id= (int *) malloc(V*sizeof(int));

    if (B->id == NULL) // Erro de alocacao
      exit(0);

    B->sz= (int *) malloc(V*sizeof(int));

    if (B->sz == NULL) // Erro de alocacao
      exit(0);

    for (i = 0; i < V; i++) // Inicializa as tabelas
    {
      B->id[i] = i; 
      B->sz[i] = 1;
    }

    return B;
}

/*
 *  Function:
 *    printBackbone
 *
 *  Description:
 *    Da print do backbone
 *
 *  Arguments:
 *      -> Grafo
 *      -> ficheiro de saida
 *
 *  Return value:
 *      --------------
 */
void printBackbone(Graph * G, FILE * fout)
{
    link *aux;
    
    for (int i = 0; i < G->V; i++) {
        aux = G->adj[i];
        while (aux != NULL) {
            fprintf(fout, "%d %d %.2lf\n", i + 1, aux->v + 1, aux->custo);
            aux = aux->next;
        }
    }
    return;
}
