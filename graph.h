#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

// Descrição das funções e das estruturas no ficheiro .c

typedef struct node link;
typedef struct edge Edge;
typedef struct graph Graph;
typedef struct backbone Backbone;

link *NewSorted(int, double, link *);
Graph *GraphInit(int,int);
void GraphInsertE (Graph *, Edge *);
void freegraph(Graph *);
int verifyEdge(Edge **, int, int, Graph *);
Backbone * BackboneInit(int);
void freeBackbone(Backbone *);
void freeArestas(Edge ** ,int);
void printBackbone(Graph *, FILE *);



#endif // HEADER_H_INCLUDED