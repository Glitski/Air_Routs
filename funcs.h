#ifndef FUNCS_H_INCLUDED
#define FUNCS_H_INCLUDED
#include "graph.h"
#include "sorting.h"

// Descrição das funções no ficheiro .c

int validateHeader(FILE *, FILE *, int *, int *, int *, int *, char *);
FILE *openFile(char *);
void verify(int, char **);

Edge ** Le_arestas(int, FILE *);
void funcao_A1(Graph *, Edge **, FILE *,int);
void funcao_B1(Graph *,Edge **, int ,int, FILE *);
void funcao_C1(Graph * , Edge **, FILE * ,int,int);
void funcao_D1(Graph *, Edge **, int, FILE *);
void funcao_E1(Graph *, Edge **, FILE *);
void InitConect(Backbone * ,Graph *, int, int);
void ResetConnect(Backbone * , int );


#endif